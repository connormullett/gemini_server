
# Known Issues
Things that need to be fixed

- Navigating to a directory crashes server.
  Can possibly cache all possible routes from `content_root`,
	or check if path leads to file/directory. Finding all
	possible routes prior to server load would be a very nice
	optimization.
- Requests can exit `content_root`. Delimit requests if starting
	with `../`, recursively

