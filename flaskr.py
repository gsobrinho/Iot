# coding: utf-8

import sqlite3
from flask import Flask, request, session, g, redirect, url_for, abort, render_template, flash
from contextlib import closing
<<<<<<< HEAD
from ploter import dataHandler
=======

DATABASE = '/tmp/flaskr.db'
>>>>>>> 9cd5d429b117a21455a52bfb61114b0c1a88e0e3

app = Flask(__name__)
app.config.from_object(__name__)

def conectar_bd():
    return sqlite3.connect(app.config['DATABASE'])

def criar_bd():
    with closing(conectar_bd()) as bd:
        with app.open_resource('esquema.sql') as sql:
            bd.cursor().executescript(sql.read())
        bd.commit()



@app.route("/")

def inicial():
	return render_template("Initial.html")

@app.route("/meuHistorico")
def bom():
	hand = dataHandler("horas/horas.txt")
	hand.dataPloter("static/image","blue")
	return render_template("plotbom.html")

@app.route("/deuRuim")
def ruim():
	hand = dataHandler("horas/hot.txt")
	hand.dataPloter("static/image","blue")
	return render_template("plotruim.html")

if __name__ == "__main__":
	app.run(debug=True)


