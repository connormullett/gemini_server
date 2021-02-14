#!/usr/bin/env python3

import socket
import ssl

HOST = 'localhost'
PORT = 1965


def make_connection(request):
  request = request.encode()

  context = ssl.create_default_context()
  context.load_verify_locations('./cert.pem')

  with socket.create_connection((HOST, PORT)) as sock:
    with context.wrap_socket(sock, server_hostname=HOST) as ssock:
      # send request
      ssock.sendall(request)
      # recieve response
      data = ssock.recv(1024)

  return data.decode('utf-8')

def main():
  request = f'gemini://{HOST}:{PORT}/index.gmi\r\n'
  data = make_connection(request)
  print(data)

if __name__ == '__main__':
  main()

