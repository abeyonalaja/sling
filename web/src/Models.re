type user = {
  id: int,
  email: string,
  username: string,
  token: string,
};

type errorDetails = {
  email: option(array(string)),
  password: option(array(string)),
  username: option(array(string)),
};

type registeredUser = {
  errors: option(errorDetails),
  user,
};
