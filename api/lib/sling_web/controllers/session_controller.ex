defmodule SlingWeb.SessionController do
  use SlingWeb, :controller

  alias Sling.Accounts
  alias Sling.Auth.Guardian

  def create(conn, params) do
    case authenticate(params) do
      {:ok, user} ->
        conn = Sling.Auth.Guardian.Plug.sign_in(conn, user)
        token = Guardian.Plug.current_token(conn)

        conn
        |> put_status(:created)
        |> render("show.json", user: user, token: token)
      {:error, _} ->
        conn
        |> put_status(:unauthorized)
        |> render("error.json", error: "email or password")
    end
  end

  def delete(conn, _) do
    conn
    |> Sling.Auth.Guardian.sign_out()
    |> put_status(:no_content)
    |> render("delete.json")
  end

  def authenticate(%{"email" => email, "password" => password}) do
    Accounts.authenticate_by_email_and_pass(email, password)
  end

  def authenticate(_), do: :error

end
