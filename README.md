
# Basic Gemini Server

## Requires
- openssl for TLS usage.
- uriparser

Server is being tested with `Kristall` gemini browser.

## Warning
This server is no way shape or form secure. This is a toy implementation.
If exposing this server on a network, use at your own risk.

## Generate self signed cert
`openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -subj '/CN=localhost'`

## Building
`make`

