# coding: utf-8


from flask import Flask, request, session, g, redirect, url_for, abort, render_template, flash
from contextlib import closing


app = Flask(__name__)
app.config.from_object(__name__)

@app.route("/")
def inicial():
  return render_template("Initial.html")

@app.route("/meuHistorico")
def bom():
  return render_template("plotbom.html")

@app.route("/deuRuim")
def ruim():
  return render_template("plotruim.html") 

if __name__ == "__main__":
    app.run(debug=True)


