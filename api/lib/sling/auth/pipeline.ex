defmodule Sling.Auth.Pipeline do
  use Guardian.Plug.Pipeline, otp_app: :sling,
                              module: Sling.Auth.Guardian,
                              error_handler: Sling.Auth.ErrorHandler

  plug(Guardian.Plug.VerifyHeader)
  plug(Guardian.Plug.EnsureAuthenticated)
  plug(Guardian.Plug.LoadResource)
end
