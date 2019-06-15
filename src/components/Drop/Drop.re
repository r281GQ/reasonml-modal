let identity = x => x;

let doc = Webapi.Dom.document;

let fakeBody = Webapi.Dom.Document.createElement("div", doc);

let body: Dom.element =
  "body"
  ->Webapi.Dom.Document.getElementsByTagName(doc)
  ->Webapi.Dom.HtmlCollection.toArray
  ->Belt.Array.get(0)
  ->Belt.Option.mapWithDefault(fakeBody, identity);

[@react.component]
let make = (~content: React.element, ~children) => {
  let target: React.Ref.t(Js.Nullable.t(Dom.element)) =
    React.useRef(Js.Nullable.null);

  let drop: React.Ref.t(Js.Nullable.t(Dom.element)) =
    React.useRef(Js.Nullable.null);

  let (targetState: Js.Nullable.t(Dom.element), setTargetState) =
    React.useState(() => Js.Nullable.null);

  let (isOpen, setOpen) = React.useState(() => false);
  let clonedElement =
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
              0->px->top,
              Css.red->backgroundColor,
            ])
          ),
      },
      [||],
    );

  let clonedElementC =
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
      let scrollY = Webapi.Dom.window |> Webapi.Dom.Window.scrollY;

      let innerHeight = Webapi.Dom.window |> Webapi.Dom.Window.innerHeight;

      let distanceFromVPTop =
        targetElement
        ->Webapi.Dom.Element.getBoundingClientRect
        ->Webapi.Dom.DomRect.top;

      let distanceFromVPBottom =
        targetElement
        ->Webapi.Dom.Element.getBoundingClientRect
        ->Webapi.Dom.DomRect.bottom;

      let distanceFromVPTopy =
        dropElement
        ->Webapi.Dom.Element.getBoundingClientRect
        ->Webapi.Dom.DomRect.top;

      // distanceFromVPTop->Js.log;

      // distanceFromVPTopy->Js.log;

      let u = (distanceFromVPTop -. distanceFromVPTopy)->int_of_float;

      // Js.log(u - 18);
      Js.log(distanceFromVPTopy);

      let i = (distanceFromVPTopy +. 18.)->int_of_float;

      // "i"->Js.log;
      i->Js.log;

      "bottom"->Js.log;
      distanceFromVPBottom->Js.log;

      if (distanceFromVPTopy +. 18. >= 0.) {
        // "ww"->Js.log;
        target
        ->React.Ref.current
        ->Js.toOption
        ->Belt.Option.map(
            Webapi.Dom.Element.setAttribute(
              "style",
              "transform: translateY(" ++ i->Js.Int.toString ++ "px)",
            ),
          )
        ->ignore;
      } else {
        ();
      };
      if (distanceFromVPTopy +. 18. < 0. && distanceFromVPTop > 0.) {
        "run"->Js.log;
        targetElement
        |> Webapi.Dom.Element.setAttribute(
             "style",
             "transform: translateY(" ++ "0" ++ "px)",
           );
      } else {
        ();
      };
    // ();

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
    clonedElementC
    {isOpen
       ? ReactDOMRe.createPortal(
           <FocusContainer
             value={
               target,
               lockScroll: false,
               lockFocus: true,
               preventTabEscape: false,
             }>
             clonedElement
           </FocusContainer>,
           body,
         )
       : React.null}
  </React.Fragment>;
};