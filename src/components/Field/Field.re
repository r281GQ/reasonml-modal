[@react.component]
let make = (~name, ~onChange, ~value) => {
  <div>
    <label htmlFor=name> name->React.string </label>
    <input name value onChange />
  </div>;
};