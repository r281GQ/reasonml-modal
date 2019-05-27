module Modal = {
  [@react.component]
  let make = (~isOpen: bool) => {
    isOpen ? <div> "this is the modal"->React.string </div> : React.null;
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