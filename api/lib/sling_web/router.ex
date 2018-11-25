defmodule SlingWeb.Router do
  use SlingWeb, :router

  pipeline :api do
    plug :accepts, ["json"]
  end

  pipeline :auth do
    plug(Sling.Auth.Pipeline)
  end

  scope "/api", SlingWeb do
    pipe_through :api
    post("/users", UserController, :create )
  end

end
