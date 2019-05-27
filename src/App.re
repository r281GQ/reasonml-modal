module Modal = {
  let renderToBody = reactElement => {
    let document = Webapi.Dom.document;

    "body"
    ->Webapi.Dom.Document.getElementsByTagName(document)
    ->Webapi.Dom.HtmlCollection.toArray
    ->Belt.Array.get(0)
    ->Belt.Option.mapWithDefault(
        React.null,
        ReactDOMRe.createPortal(reactElement),
      );
  };

  [@react.component]
  let make = (~isOpen: bool) => {
    isOpen
      ? renderToBody(<div> "this is the modal"->React.string </div>)
      : React.null;
  };
};

[@react.component]
let make = () => {
  let (isOpen, setOpen) = React.useState(() => false);

  <div>
    <button onClick={_e => setOpen(prevState => !prevState)}>
      "open the modal"->React.string
    </button>
    <Modal isOpen />
  </div>;
};