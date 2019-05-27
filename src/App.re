Css.(
  global("body", [0->px->margin, `relative->position, 100.->vh->minHeight])
);

module Modal = {
  module Overlay = {
    [@react.component]
    let make = (~children, ~onClick) => {
      <div
        onClick={e => {
          e->ReactEvent.Mouse.stopPropagation;
          onClick(e);
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
            background(`hex("000")),
            opacity(0.5),
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
        onClick={e => {
          e->ReactEvent.Mouse.stopPropagation;
          "from inside"->Js.log;
        }}
        className=Css.(
          style([
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
          <Overlay onClick=onOverlayClick>
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
    <button onClick={_e => setOpen(_ => true)}>
      "open the modal"->React.string
    </button>
    <Modal
      isOpen
      onOverlayClick={_e => setOpen(_ => false)}
      onEsc={_e => setOpen(_ => false)}>
      <div> "this is the modal content"->React.string </div>
    </Modal>
  </div>;
};