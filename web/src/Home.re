let str = ReasonReact.string;
let component = ReasonReact.statelessComponent("Home");

let make = _children => {...component, render: _self => <div> {str("Home Page")} </div>};
