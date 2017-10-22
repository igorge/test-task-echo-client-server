Test task: simple "echo" client/server using old plain sockets.
====


Tested on gcc 7.2.0 (Debian) and Microsoft Visual Studio Community 2017 (Windows 7).


On windows you have to have cmake and visual studio installed, then run build-server.bat and build-client.bat to get echo-server.exe and echo-client.exe in server/buildwin/Debug and client/buildwin/Debug respectively.

On linux use cmake to build binaries.

echo-server.exe listens on hardcoded port 8888.


echo-server.exe usage:
`echo-server.exe`

Log file will be stored in echosrv.log file in current directory, log records will be appended.


echo-client.exe usage:
`echo-client.exe dns-name-or-ip (linear | (random number_of_requests)`


Example:
----

`echo-client.exe 127.0.0.1 linear`
will send SESSION, TIME and END requests to echo-server.

`echo-client.exe 127.0.0.1 random 10`
will send 10 random rquests and END request to echo-server.




