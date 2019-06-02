module LoginForm = {
  [@react.component]
  let make = (~onSuccess) => {
    let (name, setName) = React.useState(() => "");
    let (password, setPassword) = React.useState(() => "");

    <Form onSubmit={e => onSuccess()}>
      <Field
        name="name"
        value=name
        onChange={e => {
          let value = e->ReactEvent.Form.target##value;

          setName(_prev => value);
        }}
      />
      <Field
        name="password"
        value=password
        onChange={e => {
          let value = e->ReactEvent.Form.target##value;

          setPassword(_prev => value);
        }}
      />
      <button type_="submit"> "submit"->React.string </button>
    </Form>;
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
    <Drop content={<div id="000"> "content"->React.string </div>}>
      <button id="111"> "test"->React.string </button>
    </Drop>
  </div>;
  // <Modal
  //   isOpen
  //   onOverlayClick={_e => setOpen(_ => false)}
  //   onEsc={_e => setOpen(_ => false)}>
  //   <LoginForm onSuccess={() => setOpen(_ => false)} />
  // </Modal>
};