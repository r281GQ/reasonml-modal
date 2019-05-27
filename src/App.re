Css.(
  global("body", [0->px->margin, `relative->position, 100.->vh->minHeight])
);

module Modal = {
  module Helper = {
    type overFlow =
      | Hidden
      | Auto;

    let convert =
      fun
      | Hidden => "hidden"
      | Auto => "auto";

    let setBodyOverflow = value =>
      Webapi.Dom.document
      ->Webapi.Dom.Document.asHtmlDocument
      ->Belt.Option.flatMap(Webapi.Dom.HtmlDocument.body)
      ->Belt.Option.flatMap(Webapi.Dom.Element.asHtmlElement)
      ->Belt.Option.mapWithDefault(
          (),
          Webapi.Dom.HtmlElement.setAttribute(
            "style",
            "overflow:" ++ value->convert,
          ),
        );
  };

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
    let make = (~children, ~onOverlayClick, ~onEsc) => {
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

      React.useEffect1(
        () => {
          open Helper;

          setBodyOverflow(Hidden);

          Some(() => setBodyOverflow(Auto));
        },
        [||],
      );

      <div
        onClick={e => {
          e->ReactEvent.Mouse.stopPropagation;
          onOverlayClick(e);
        }}
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
    };
  };

  module ModalContentWrapper = {
    [@react.component]
    let make = (~children) => {
      <div
        onClick={e => e->ReactEvent.Mouse.stopPropagation}
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

  let renderToBody = (~element, ~onOverlayClick, ~onEsc) => {
    let document = Webapi.Dom.document;

    "body"
    ->Webapi.Dom.Document.getElementsByTagName(document)
    ->Webapi.Dom.HtmlCollection.toArray
    ->Belt.Array.get(0)
    ->Belt.Option.mapWithDefault(
        React.null,
        ReactDOMRe.createPortal(
          <Overlay onOverlayClick onEsc>
            <Cover />
            <ModalContentWrapper> element </ModalContentWrapper>
          </Overlay>,
        ),
      );
  };

  [@react.component]
  let make = (~isOpen: bool, ~onOverlayClick, ~onEsc, ~children) => {
    isOpen
      ? renderToBody(~element=children, ~onOverlayClick, ~onEsc) : React.null;
  };
};

[@react.component]
let make = () => {
  let (isOpen, setOpen) = React.useState(() => false);

  <div className=Css.(style([4000->px->height]))>
    <div>
      <button onClick={_e => setOpen(_ => true)}>
        "open the modal"->React.string
      </button>
    </div>
    <div>
      <button onClick={_e => setOpen(_ => true)}>
        "open the modal"->React.string
      </button>
    </div>
    <div>
      <button onClick={_e => setOpen(_ => true)}>
        "open the modal"->React.string
      </button>
    </div>
    <div>
      <button onClick={_e => setOpen(_ => true)}>
        "open the modal"->React.string
      </button>
    </div>
    <Modal
      isOpen
      onOverlayClick={_e => setOpen(_ => false)}
      onEsc={_e => setOpen(_ => false)}>
      <div> "this is the modal content"->React.string </div>
    </Modal>
  </div>;
};