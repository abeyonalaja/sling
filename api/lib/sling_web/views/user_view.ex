defmodule SlingWeb.UserView do
  use SlingWeb, :view
  alias SlingWeb.UserView

  def render("show.json", %{user: user, token: token}) do
    %{
      data: render_one(user, UserView, "user.json"),
      meta: %{
        token: token
      }
    }
  end


  def render("user.json", %{user: user}) do
    %{id: user.id, username: user.username}
  end

  def render("error.json", %{changeset: changeset}) do
    %{errors: translate_errors(changeset)}
  end

  def translate_errors(changeset) do
    Ecto.Changeset.traverse_errors(changeset, &translate_error/1)
  end
end
