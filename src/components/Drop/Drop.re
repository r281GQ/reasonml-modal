[@react.component]
let make = (~content: React.element, ~children) => {
  let target = React.useRef(Js.Nullable.null);

  let y: React.Ref.t(Js.Nullable.t(Dom.element)) =
    React.useRef(Js.Nullable.null);

  let (isOpen, setOpen) = React.useState(() => false);

  let clonedElement =
    ReasonReact.cloneElement(
      content,
      ~props={
        "ref": ref => {
          React.Ref.setCurrent(target, ref);
        },
        "className":
          Css.(
            style([
              20->px->padding,
              `absolute->position,
              400->px->top,
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
          React.Ref.setCurrent(y, ref);
        },
      },
      [||],
    );

  React.useEffect(() => {
    let logger = d => {
      setOpen(prev => !prev);
      Js.log(d);
    };

    switch (y->React.Ref.current->Js.Nullable.toOption) {
    | Some(p) => p |> Webapi.Dom.Element.addClickEventListener(logger)
    | None => ()
    };

    Some(
      () =>
        switch (y->React.Ref.current->Js.Nullable.toOption) {
        | Some(p) => p |> Webapi.Dom.Element.removeClickEventListener(logger)
        | None => ()
        },
    );
  });

  React.useEffect(() => {
    Webapi.Dom.window
    |> Webapi.Dom.Window.addEventListener("scroll", e => {
         let scrollY = Webapi.Dom.window |> Webapi.Dom.Window.scrollY;

         let innerHeight = Webapi.Dom.window |> Webapi.Dom.Window.innerHeight;

         let distanceFromVPTop =
           target
           ->React.Ref.current
           ->Js.toOption
           ->Belt.Option.map(Webapi.Dom.Element.getBoundingClientRect)
           ->Belt.Option.mapWithDefault(0., Webapi.Dom.DomRect.top);
         ();
       });
    None;
  });

  <React.Fragment>
    clonedElementC
    {isOpen
       ? <div className=Css.(style([`relative->position]))>
           <FocusContainer
             value={
               target: y,
               lockScroll: false,
               lockFocus: true,
               preventTabEscape: false,
             }>
             {"body"
              ->Webapi.Dom.Document.getElementsByTagName(Webapi.Dom.document)
              ->Webapi.Dom.HtmlCollection.toArray
              ->Belt.Array.get(0)
              ->Belt.Option.mapWithDefault(
                  React.null,
                  ReactDOMRe.createPortal(clonedElement),
                )}
           </FocusContainer>
         </div>
       : React.null}
  </React.Fragment>;
};