let str = ReasonReact.string;
let component = ReasonReact.statelessComponent("Users");

let make = _children => {...component, render: _self => <div> {str("User List page")} </div>};
