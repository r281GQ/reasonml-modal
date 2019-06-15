let identity = x => x;

let doc = Webapi.Dom.document;

let fakeBody = Webapi.Dom.Document.createElement("div", doc);

let body: Dom.element =
  "body"
  ->Webapi.Dom.Document.getElementsByTagName(doc)
  ->Webapi.Dom.HtmlCollection.toArray
  ->Belt.Array.get(0)
  ->Belt.Option.mapWithDefault(fakeBody, identity);

let top = (element: Dom.element) =>
  element
  ->Webapi.Dom.Element.getBoundingClientRect
  ->Webapi.Dom.DomRect.top
  ->int_of_float;

let bottom = (element: Dom.element) =>
  element
  ->Webapi.Dom.Element.getBoundingClientRect
  ->Webapi.Dom.DomRect.bottom
  ->int_of_float;

[@react.component]
let make = (~content: React.element, ~children) => {
  let target: React.Ref.t(Js.Nullable.t(Dom.element)) =
    React.useRef(Js.Nullable.null);

  let drop: React.Ref.t(Js.Nullable.t(Dom.element)) =
    React.useRef(Js.Nullable.null);

  let (targetState: Js.Nullable.t(Dom.element), setTargetState) =
    React.useState(() => Js.Nullable.null);

  let (isOpen, setOpen) = React.useState(() => false);

  let targetElement =
    ReasonReact.cloneElement(
      content,
      ~props={
        "ref": ref1 => {
          React.Ref.setCurrent(target, ref1);
          setTargetState(_ => ref1);
        },
        "className":
          Css.(
            style([
              20->px->padding,
              `fixed->position,
              0->px->Css.top,
              Css.red->backgroundColor,
            ])
          ),
      },
      [||],
    );

  let dropElement =
    ReasonReact.cloneElement(
      children,
      ~props={
        "ref": ref => {
          React.Ref.setCurrent(drop, ref);
        },
      },
      [||],
    );

  React.useEffect(() => {
    let closeHandler = e => {
      let hasClickHappenedInDropElements =
        switch (
          drop->React.Ref.current->Js.Nullable.toOption,
          target->React.Ref.current->Js.Nullable.toOption,
        ) {
        | (Some(dropElement), Some(targetElement)) =>
          let hasOccuredInDropElement =
            e
            ->Webapi.Dom.MouseEvent.target
            ->Webapi.Dom.EventTarget.unsafeAsElement
            ->Webapi.Dom.Element.contains(dropElement);

          let hasOccuredInTargetElement =
            e
            ->Webapi.Dom.MouseEvent.target
            ->Webapi.Dom.EventTarget.unsafeAsElement
            ->Webapi.Dom.Element.contains(targetElement);

          hasOccuredInDropElement || hasOccuredInTargetElement;
        | (_, _) => false
        };

      !hasClickHappenedInDropElements ? setOpen(_ => false) : ();
    };

    body |> Webapi.Dom.Element.addClickEventListenerUseCapture(closeHandler);

    Some(
      () =>
        body
        |> Webapi.Dom.Element.removeClickEventListenerUseCapture(closeHandler),
    );
  });

  React.useEffect(() => {
    let clickHandler = _e => {
      setOpen(prev => !prev);
    };

    switch (drop->React.Ref.current->Js.Nullable.toOption) {
    | Some(dropElement) =>
      dropElement |> Webapi.Dom.Element.addClickEventListener(clickHandler)
    | None => ()
    };

    Some(
      () =>
        switch (drop->React.Ref.current->Js.Nullable.toOption) {
        | Some(dropElement) =>
          dropElement
          |> Webapi.Dom.Element.removeClickEventListener(clickHandler)
        | None => ()
        },
    );
  });

  let update = (targetRef, dropRef) => {
    switch (
      targetRef->React.Ref.current->Js.toOption,
      dropRef->React.Ref.current->Js.toOption,
    ) {
    | (Some(targetElement), Some(dropElement)) =>
      let viewPortHeight = Webapi.Dom.window |> Webapi.Dom.Window.innerHeight;

      let targetTop = targetElement |> top;

      let targetBottom = targetElement |> bottom;

      let targetHeight = targetBottom - targetTop;

      let dropTop = dropElement |> top;

      let dropBottom = dropElement |> bottom;

      let i = dropTop - targetHeight;

      let isDropBottomBelowViewPort =
        dropBottom + targetHeight >= viewPortHeight;

      let isDropBottomAboveViewPort = dropBottom < 0;

      let isDropTopBelowViewPort = dropTop >= viewPortHeight;

      switch (
        isDropBottomBelowViewPort,
        isDropBottomAboveViewPort,
        isDropTopBelowViewPort,
      ) {
      | (true, _, true) =>
        targetElement
        |> Webapi.Dom.Element.setAttribute(
             "style",
             "transform: translateY("
             ++ (viewPortHeight - targetHeight)->Js.Int.toString
             ++ "px)",
           )
      | (true, _, _) =>
        targetElement
        |> Webapi.Dom.Element.setAttribute(
             "style",
             "transform: translateY(" ++ i->Js.Int.toString ++ "px)",
           )
      | (_, true, _) =>
        targetElement
        |> Webapi.Dom.Element.setAttribute(
             "style",
             "transform: translateY(" ++ "0" ++ "px)",
           )
      | (_, _, _) =>
        targetElement
        |> Webapi.Dom.Element.setAttribute(
             "style",
             "transform: translateY(" ++ dropBottom->Js.Int.toString ++ "px)",
           )
      };

    | (_, _) => ()
    };
  };

  React.useEffect(() => {
    let scrollHander = _e => update(target, drop);

    Webapi.Dom.window
    |> Webapi.Dom.Window.addEventListener("scroll", scrollHander);

    Some(
      () =>
        Webapi.Dom.window
        |> Webapi.Dom.Window.removeEventListener("scroll", scrollHander),
    );
  });

  React.useEffect1(
    () => {
      update(target, drop);

      None;
    },
    [|targetState|],
  );

  <React.Fragment>
    dropElement
    {isOpen
       ? ReactDOMRe.createPortal(
           <FocusContainer
             value={
               target,
               lockScroll: false,
               lockFocus: true,
               preventTabEscape: false,
             }>
             targetElement
           </FocusContainer>,
           body,
         )
       : React.null}
  </React.Fragment>;
};