let saveTokenToStorage = value => Dom.Storage.(localStorage |> setItem("jwt", value));

let getTokenFromStorage = () => Dom.Storage.(localStorage |> getItem("jwt"));

let saveUserToStorage = username => Dom.Storage.(localStorage |> setItem("username", username));

let getUserFromStorage = () => Dom.Storage.(localStorage |> getItem("username"));
