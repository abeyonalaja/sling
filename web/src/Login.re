let show = ReasonReact.string;

type state = {
  email: string,
  password: string,
  hasValidationError: bool,
  errorList: list(string),
};

type action =
  | Login((bool, list(string)))
  | EmailUpdate(string)
  | PasswordUpdate(string)
  | LoginPending;

module Encode = {
  let encodeUserCredentials = creds => {
    open! Json.Encode;
    object_([("email", string(creds.email)), ("password", string(creds.password))]);
  };

  let user = topLevelUser => Json.Encode.(object_([("user", encodeUserCredentials(topLevelUser))]));

  let currentUser = (username, bio) => Json.Encode.[("username", string(username)), ("bio", string(bio))];
};

/*let updateEmail = (event, self) => EmailUpdate(ReactEvent.Form.target(event)##value);*/
let updateEmail = (event, self) => Js.log("ffff");

let updatePassword = event => PasswordUpdate(ReactEvent.Form.target(event)##target);

let errorDisplayList = state =>
  List.filter(errorMessage => String.length(errorMessage) > 0, state.errorList)
  |> List.mapi((acc, errorMessage) =>
       <ul className="error-messages" key={string_of_int(acc)}> <li> {show(errorMessage)} </li> </ul>
     );

let loginUser = (event, {ReasonReact.state, send}) => {
  ReactEvent.Mouse.preventDefault(event);
  /*let {ReasonReact.state, send} = self;*/
  let reduceByAuthResult = (_status, jsonPayload) =>
    jsonPayload
    |> Js.Promise.then_(json => {
         let newUser = JsonRequests.checkForErrors(json);
         let updatedState =
           switch (newUser) {
           | Some(errors) => {
               ...state,
               hasValidationError: true,
               errorList: errors |> JsonRequests.convertErrorsToList,
             }
           | None =>
             let loggedIn = JsonRequests.parseNewUser(json);
             Effects.saveTokenToStorage(loggedIn.user.token);
             Effects.saveUserToStorage(loggedIn.user.username);
             {...state, hasValidationError: false};
           };
         ();
         let callLoginReducer = _payload => Login((updatedState.hasValidationError, updatedState.errorList));
         send(callLoginReducer) |> Js.Promise.resolve;
       });
  JsonRequests.authenticateUser(reduceByAuthResult, Encode.user(state)) |> ignore;
  send(_ => LoginPending) |> ignore;
};
let change = (event, self) => Js.log("Clicked");

let componet = ReasonReact.reducerComponent("Login");

let make = _children => {
  ...componet,
  initialState: () => {email: "", password: "", hasValidationError: false, errorList: []},
  reducer: (action, state) =>
    switch (action) {
    | EmailUpdate(value) => ReasonReact.Update({...state, email: value})
    | PasswordUpdate(value) => ReasonReact.Update({...state, password: value})
    | Login((hasError, errorList)) => ReasonReact.Update({...state, hasValidationError: hasError, errorList})
    | LoginPending => ReasonReact.NoUpdate
    },

  render: self => {
    let {ReasonReact.state, send} = self;
    <div className="auth-page">
      <div className="container page">
        <div className="row">
          <div className="col-md-6 offset-md-3 col-xs-12">
            <h1 className="text-xs-center"> {show("Sign in")} </h1>
            <p
              className="text-xs-center"
              /*<a href="#" onClick={goToRegister(router)}> {show("Need an account?")} </a>*/
            />
            {
              if (state.hasValidationError) {
                Array.of_list(errorDisplayList(state)) |> ReasonReact.array;
              } else {
                ReasonReact.null;
              }
            }
            <form>
              <fieldset className="form-group">
                <input
                  type_="text"
                  className="form-control form-control-lg"
                  placeholder="Email"
                  value={state.email}
                  onChange={e => send(EmailUpdate(ReactEvent.Form.target(e)##value))}
                />
              </fieldset>
              <fieldset className="form-group">
                <input
                  type_="password"
                  className="form-control form-control-lg"
                  placeholder="Password"
                  value={state.password}
                  onChange={self.handle(change)}
                />
              </fieldset>
            </form>
          </div>
        </div>
      </div>
      <button
        onClick={
          event => {
            let user = loginUser(event);
            ();
          }
        }
        className="btn btn-lg btn-primary pull-xs-right">
        {show("Sign in")}
      </button>
    </div>;
  },
};
