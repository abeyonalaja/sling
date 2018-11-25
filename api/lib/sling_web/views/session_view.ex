defmodule SlingWeb.SessionView do
  use SlingWeb, :view

  def render("show.json", %{user: user, token: token}) do
    %{
      data: render_one(user, SlingWeb.UserView, "user.json"),
      meta: %{
        token: token
      }
    }
  end

  def render("delete.json", _) do
    %{ok: true}
  end

  def render("error.json", %{error: error}) do
    %{errors: %{error: error}}
  end

end
