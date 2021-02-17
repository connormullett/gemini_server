
# Basic Gemini Server

## Requires
- openssl for TLS usage.
- uriparser
- libconfig

All requirements are easy google searches and use the following convention for installing with the exception of libconfig, which uses cmake.

```
git clone <lib git source tree>
cd lib
./configure
make
sudo make install
```

For libconfig, use `cmake .` before `make && sudo make install`.

Server is being tested with `Kristall` gemini browser.

## Warning
This server is no way shape or form secure. This is a toy implementation.
If exposing this server on a network, use at your own risk.

## Certificates
Gemini requires TLS by default. The good news is you don't need to use a CA and can generate certificates yourself. Use the following command to generate a public and private key.

`openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -subj '/CN=localhost'`

## Configuration

server.cfg contains defaults for every value. The defaults are enough to start running locally out of the box.

## Building
`make`

