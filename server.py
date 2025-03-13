import threading, logging, sys
from flask import Flask, request

sys.modules["flask.cli"].show_server_banner = lambda *x: None   # disable welcome dialog
logging.getLogger("werkzeug").setLevel(logging.ERROR)           # disable all kind of verbose or warningn
app=Flask(__name__)
accomplished=True
cmd=""

@app.route("/command", methods=["GET"])
def expose_command()->str:
    global cmd, accomplished
    if cmd and accomplished:
        accomplished=False
        return cmd, 200
    return "", 404

@app.route("/result", methods=["POST"])
def print_response()->int:
    global accomplished, cmd
    if request.get_data():                                     # https://docs.python.org/3/library/codecs.html#standard-encodings
        output=request.get_data().decode("utf-8", errors="replace")
        accomplished=True
        cmd=""                            
        print(f"\n{output}$ ", end="") if output != " " else None
        return "", 200
    return "", 400

@app.errorhandler(Exception)
def handle_exception(e):
    return "a", 500

def update_cmd():
    global cmd
    while True:
        cmd = input("$ ")

if __name__ == "__main__":
    t = threading.Thread(target=update_cmd)
    t.daemon=True
    t.start()
    app.run(host="127.0.0.1", port=5001) # ssl_context="adhoc" for self-signed certificate, pyopenssl required
