let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("NotFound");

let make = _children => {...component, render: _self => <div> {str("NOT FOUND")} </div>};
