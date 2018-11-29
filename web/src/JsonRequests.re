open Config;
open Models;

let make_headers = (token: option(string)) => {
  let content_type = ("content-type", "application/json");
  switch (token) {
  | None => [|content_type|]
  | Some(t) => [|content_type, ("authorization", "Token " ++ t)|]
  };
};

let checkForErrors = responseText =>
  responseText
  |> Js.Json.parseExn
  |> Js.Json.decodeObject
  |> Js.Option.andThen((. prop) => Js.Dict.get(prop, "errors"));

type newUserResponse =
  | Succeed(user)
  | Failed(user);

let parseUser = json =>
  Json.Decode.{
    id: json |> field("id", int),
    email: json |> field("email", string),
    username: json |> field("username", string),
    token: json |> field("token", string),
  };

let parseErrors = json =>
  Some(
    Json.Decode.{
      email: json |> optional(field("email", array(string))),
      password: json |> optional(field("password", array(string))),
      username: json |> optional(field("username", array(string))),
    },
  );

let parseNormalResp = json => {user: Json.Decode.(json |> field("user", parseUser)), errors: None};

let parseEmptyDefaultError = () => {id: 0, email: "", username: "", token: ""};

let parseErrorResp = errors => {user: parseEmptyDefaultError(), errors};

let parseNewUser = responseText => {
  let json = Js.Json.parseExn(responseText);
  let possibleErrors = Json.Decode.(json |> optional(field("errors", parseErrors)));
  switch (possibleErrors) {
  | Some(errors) => parseErrorResp(errors)
  | None => parseNormalResp(json)
  };
};

let convertErrorsToList = errorJson => {
  let decodedJson = Js.Json.decodeObject(errorJson);
  switch (decodedJson) {
  | Some(errorList) =>
    let errorKeys = Js.Dict.keys(errorList);
    let errorValues = Js.Dict.values(errorList);
    Array.mapi(
      (acc, errorField) => {
        let validationError = errorValues[acc];
        let frontCaps = String.capitalize(errorField);
        {j|$frontCaps $validationError|j};
      },
      errorKeys,
    )
    |> Array.to_list;
  | None => []
  };
};

let makeInit = (method, token, data: option(Js.Json.t)) => {
  let defaultInit =
    Bs_fetch.RequestInit.make(~method_=method, ~headers=Bs_fetch.HeadersInit.makeWithArray @@ make_headers(token));
  switch (data) {
  | None => defaultInit()
  | Some(d) => defaultInit(~body=Bs_fetch.BodyInit.make @@ Js.Json.stringify(d), ())
  };
};

let constructUrl = url => apiUrlBase ++ mapUrl(url);

let sendRequest = (requestMethod, token, jsonData, actionFunc, url) => {
  open Js.Promise;
  let request = makeInit(requestMethod, token, jsonData);
  Bs_fetch.(
    fetchWithInit(url, request)
    |> then_(response => actionFunc(Response.status(response), Response.text(response)) |> resolve)
  );
};

let authenticateUser = (loginFunc, jsonData) =>
  sendRequest(Post, None, Some(jsonData), loginFunc, constructUrl(Config.Authenticate));
