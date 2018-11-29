let show = ReasonReact.string;

type state = {
  email: string,
  password: string,
  hasValidationError: bool,
  errorList: list(string),
};

type action =
  | Login(bool, list(string))
  | EmailUpdate(string)
  | PasswordUpdate(string)
  | LoginPending;

let updateEmil = event => EmailUpdate(ReactEvent.Form.target(event)##value);

let updatePassword = event => PasswordUpdate(ReactEvent.Form.target(event)##target);

let errorDisplayList = state =>
  List.filter(errorMessage => String.length(errorMessage) > 0, state.errorList)
  |> List.mapi((acc, errorMessage) =>
       <ul className="error-messages" key={string_of_int(acc)}> <li> {show(errorMessage)} </li> </ul>
     );

let loginUser = (event, {ReasonReact.state, send}) => {
  ReactEvent.Mouse.preventDefault(event);
  let reduceByAuthResult = (_status, jsonPayload) =>
    jsonPayload
    |> Js.Promise.then_(json => {
         let newUser = JsonRequests.checkForErrors(json);
         let updateState =
           switch (newUser) {
           | Some(errors) => {...state, hasValidationError: true}
           | None =>
             let loggedIn = JsonRequests.parseNewUser(json);
             Effects.saveTokenToStorage(loggedIn.user.token);
             Effects.saveUserToStorage(loggedIn.user.username);
           /*DirectorRe.setRoute(route, "/home");*/
           };
         let callLoginReducer = _payload => Login((updatedState.hasValidationError, updatedState.errorList));
         send(callLoginReducer, ()) |> Js.Promise.resolve;
       });
  JsonRequests.authenticateUser(reduceByAuthResult, Encode.user(state)) |> ignore;
  reduce(_ => LoginPending, ());
};
