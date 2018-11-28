let apiUrlBase = "http://localhost:4000/api";

type ulrMap =
  | Authenticate
  | Register
  | CurrentUser
  | UpdateUser;

let mapUrl = url =>
  switch (url) {
  | Authenticate => "/users/login"
  | Register => "/users"
  | CurrentUser
  | UpdateUser => "/user"
  };
