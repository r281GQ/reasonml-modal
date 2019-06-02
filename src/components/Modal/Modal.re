Css.(
  global("body", [0->px->margin, `relative->position, 100.->vh->minHeight])
);

module Cover = {
  [@react.component]
  let make = () => {
    <div
      className=Css.(
        style([
          `fixed->position,
          0->px->top,
          0->px->left,
          0->px->right,
          0->px->bottom,
          500->zIndex,
          100.->vh->height,
          100.->vw->width,
          background(`hex("000")),
          opacity(0.5),
        ])
      )
    />;
  };
};

module Overlay = {
  [@react.component]
  let make =
    React.forwardRef((~children, ~onOverlayClick, ~onEsc, ref_) => {
      // let overlayRef = React.useRef(Js.Nullable.null);

      let escapeHandler = event =>
        switch (event->Webapi.Dom.KeyboardEvent.key) {
        | "Escape" => onEsc()
        | _ => ()
        };

      React.useEffect1(
        () => {
          Webapi.Dom.window
          |> Webapi.Dom.Window.addKeyDownEventListener(escapeHandler);

          Some(
            () =>
              Webapi.Dom.window
              |> Webapi.Dom.Window.removeKeyDownEventListener(escapeHandler),
          );
        },
        [||],
      );

      <div
        ref=?{
          ref_->Js.Nullable.toOption->Belt.Option.map(ReactDOMRe.Ref.domRef)
        }
        onClick=onOverlayClick
        className=Css.(
          style([
            `fixed->position,
            0->px->top,
            0->px->left,
            0->px->right,
            0->px->bottom,
            999->zIndex,
            100.->vh->height,
            100.->vw->width,
            display(`flex),
            flexDirection(`column),
            justifyContent(`center),
          ])
        )>
        children
      </div>;
    });
};

module ModalContentWrapper = {
  [@react.component]
  let make = (~children) => {
    <div
      onClick=ReactEvent.Mouse.stopPropagation
      className=Css.(
        style([
          999->zIndex,
          background(`hex("fff")),
          maxWidth(`percent(50.)),
          marginLeft(`auto),
          marginRight(`auto),
        ])
      )>
      children
    </div>;
  };
};

let renderToBody = (~element, ~onOverlayClick, ~onEsc, ~overlayRef) => {
  let document = Webapi.Dom.document;

  "body"
  ->Webapi.Dom.Document.getElementsByTagName(document)
  ->Webapi.Dom.HtmlCollection.toArray
  ->Belt.Array.get(0)
  ->Belt.Option.mapWithDefault(
      React.null,
      ReactDOMRe.createPortal(
        <FocusContainer
          value={
            lockFocus: true,
            lockScroll: true,
            target: overlayRef,
            preventTabEscape: true,
          }>
          <Overlay
            onOverlayClick onEsc ref={overlayRef->ReactDOMRe.Ref.domRef}>
            <Cover />
            <ModalContentWrapper> element </ModalContentWrapper>
          </Overlay>
        </FocusContainer>,
      ),
    );
};

[@react.component]
let make = (~isOpen: bool, ~onOverlayClick, ~onEsc, ~children) => {
  let overlayRef = React.useRef(Js.Nullable.null);

  isOpen
    ? renderToBody(~element=children, ~onOverlayClick, ~onEsc, ~overlayRef)
    : React.null;
};