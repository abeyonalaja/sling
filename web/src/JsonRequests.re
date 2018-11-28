
open Config;
open Modeles;

let make_headers = (token: option(string)) => {
  let content_type = ("content-type", "application/json");
  switch (token) {
  | None => [|content_type|]
  | Some(t) => [|content_type, ("authorization", "Token " ++ t)|]
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

let parseNewUser = (responseText) => {
  let json = Js.Json.parseExn(responseText);
  let possibleErrors = Json.Decode.(json |> optional(field("errors", parseErrors)));
  switch possibleErrors {
  | Some(errors) => parseErrorResp(errors)
  | None => parseNormalResp(json)
  }
};

let toJson = (listedElements) => listedElements |> Js.Dict.fromList |> Js.Json.object_;

type newUserResponse =
  | Succeed(user)
  | Failed(user);

let parseUser = (json) =>
  Json.Decode.{
    id: json |> field("id", int),
    email: json |> field("email", string),
    createdAt: json |> field("createdAt", string),
    updatedAt: json |> field("updatedAt", string),
    username: json |> field("username", string),
    bio: json |> optional(field("bio",string)),
    image: json |> optional(field("image",string)),
    token: json |> field("token", string)
  };


let parseNormalResp = (json) => {
  user: Json.Decode.(json |> field("user", parseUser)),
  errors: None
};

let parseErrors = (json) =>
    Json.Decode.{
      email: json |> optional(field("email", array(string))),
      password: json |> optional(field("password", array(string))),
      username: json |> optional(field("username", array(string)))
    }

let parseEmptyDefaultError = () => {
  id: 0,
  email: "",
  createdAt: "",
  updatedAt: "",
  username: "",
  token: ""
};

let parseErrorResp = (errors) => {user: parseEmptyDefaultError(), errors};

let hasErrors = (checkId) =>
  switch checkId {
  | Some(_resp) => true
  | None => false
  };

type responseType =
  | Error(Js.Json.t)
  | User(Js.Json.t);

let getUserGraph = (responseText) => {
  let user =
    responseText
    |> Js.Json.parseExn
    |> Js.Json.decodeObject
    |> Js.Option.andThen([@bs] ((prop) => Js.Dict.get(prop, "user")));
  switch user {
  | Some(json) => json
  | None => Js.Json.parseExn({j|{}|j})
  }

let checkForErrors = responseText =>
  responseText
  |> Js.Json.parseExn
  |> Js.Json.decodeObject
  |> Js.Option.andThen((. prop) => Js.Dict.get(prop, "errors"));

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

let registerNewUser = (registerFunc, jsonData) => {
  open Js.Promise;
  let request = makeInit(Post, None, Some(jsonData));
  Bs_fetch.(
    fetchWithInit(apiUrlBase ++ mapUrl(Config.Register), request)
    |> then_(response => registerFunc(Response.status(response), Response.text(response)) |> resolve)
  );
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

let updateUser = (updateUserFunc, jsonData, token) =>
  sendRequest(Put, token, Some(jsonData), updateUserFunc, constructUrl(Config.UpdateUser));

let getCurrentUser = (getUserFunc, token) =>
  sendRequest(Get, token, None, getUserFunc, constructUrl(Config.CurrentUser))