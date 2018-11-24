let str = ReasonReact.string;
let component = ReasonReact.statelessComponent("User");

let make = (~id, _children) => {
  ...component,
  render: _self => <div> {str("User Page" ++ string_of_int(id))} </div>,
};
