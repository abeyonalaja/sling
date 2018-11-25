defmodule SlingWeb.UserController do
  use SlingWeb, :controller
  alias Sling.Accounts
  alias Sling.Accounts.User

  def create(conn, %{"user" => user_params}) do
    case Accounts.create_user(user_params) do
      {:ok, user} ->
      conn = Sling.Auth.Guardian.Plug.sign_in(conn, user)
      token = Guardian.Plug.current_token(conn)

      conn
      |> put_status(:created)
      |> render("show.json", user: user, token: token)
      {:error, %Ecto.Changeset{} = changeset} ->
        conn
        |> put_status(:unauthorized)
        |> render( "error.json", changeset: changeset )
    end
  end

end
