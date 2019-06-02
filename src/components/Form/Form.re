[@react.component]
let make = (~onSubmit, ~children) => {
  <form
    onSubmit={e => {
      e->ReactEvent.Form.preventDefault;
      onSubmit();
    }}>
    children
  </form>;
};