[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo: string = "./logo.svg";

let str = ReasonReact.string;

type route =
  | Home
  | Users
  | User(int)
  | NotFound;

type state = {route};

type action =
  | ChangeRoute(route);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Home
  | ["users", id] => User(int_of_string(id))
  | ["users"] => Users
  | _ => NotFound
  };

let initialState = () => {route: ReasonReact.Router.dangerouslyGetInitialUrl() |> mapUrlToRoute};
let component = ReasonReact.reducerComponent("App");
let make = _children => {
  ...component,
  reducer,
  initialState,
  render: ({state}) =>
    <div className="App">
      <a href="/"> {str("Home")} </a>
      <a href="/users/1"> {str("User")} </a>
      <a href="/users"> {str("Users")} </a>
      <div>
        {
          switch (state.route) {
          | Home => <Home />
          | User(id) => <UserPage id />
          | Users => <UserListPage />
          | NotFound => <NotFound />
          }
        }
      </div>
    </div>,
  /*<div className="App-header">*/
  /*<img src=logo className="App-logo" alt="logo" />*/
  /*<h2> {ReasonReact.string(message)} </h2>*/
  /*</div>*/
  /*<p className="App-intro">*/
  /*{ReasonReact.string("To get started, edit")}*/
  /*<code> {ReasonReact.string(" src/app.re ")} </code>*/
  /*{ReasonReact.string("and save to reload.")}*/
  /*</p>*/
};
