defmodule Sling.Repo do
  use Ecto.Repo,
    otp_app: :sling,
    adapter: Ecto.Adapters.Postgres
end
