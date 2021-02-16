
# Basic Gemini Server

Requires openssl for TLS usage.

Server is being tested with `Kristall` gemini browser.

## Generate self signed cert
`openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -subj '/CN=localhost'`

## Building
`make`

