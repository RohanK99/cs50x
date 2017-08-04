from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
import datetime
import time 

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    # stock symbols
    stockPortfolio = db.execute("SELECT * FROM portfolio WHERE id=:id", id=session["user_id"])
    
    # cash 
    stockValue = 0
    
    # iterative over stocks to update portfolio
    for stocks in stockPortfolio:
        symbol=stocks["symbol"]
        stock=lookup(stocks["symbol"])
        shares=stocks["shares"]
        total = shares*stock["price"]
        stockValue += total
        db.execute("UPDATE portfolio SET price=:price, total=:total WHERE id=:id AND symbol=:symbol", price=usd(stock["price"]), total=usd(total), id=session["user_id"], symbol=symbol)
    
    # store cash
    liquidCash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
    
    # update cash
    portfolioValue = stockValue + liquidCash[0]["cash"]
    
    newPortfolio = db.execute("SELECT * FROM portfolio where id=:id", id=session["user_id"])
    
    # print portfolio
    return render_template("index.html", stocks=newPortfolio, cash=usd(liquidCash[0]["cash"]), total=usd(portfolioValue))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    if request.method == "POST":
        
        if not request.form.get("symbol"):
            return apology("missing symbol")
        if not request.form.get("shares"):
            return apology("missing shares")
        
        # check if valid stock
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("invalid stock")
            
        # check if valid number of shares
        shares = int(request.form.get("shares"))
        if shares < 0:
            return apology("invalid shares")
        
        money = db.execute("SELECT cash FROM users where id=:id", id=session["user_id"])
        
        # check if enough money to buy stock
        if not money or float(money[0]["cash"]) < stock["price"]*shares:
            return apology("can't afford")
            
        # update user money
        db.execute("UPDATE users SET cash = cash - :purchase WHERE id = :id", id=session["user_id"], purchase=stock["price"]*int(shares))
        
        # update history
        db.execute("INSERT INTO history (symbol, shares, price, id) VALUES(:symbol, :shares, :price, :id)", symbol=stock["symbol"], shares=shares, price=usd(stock["price"]), id=session["user_id"])
        
        print("testing log")
        
        # select shares
        userShares = db.execute("SELECT shares FROM portfolio WHERE id = :id AND symbol=:symbol", id=session["user_id"], symbol=stock["symbol"])
        
        # insert data into database
        if not userShares:
            db.execute("INSERT INTO portfolio (name, shares, price, total, symbol, id) VALUES(:name, :shares, :price, :total, :symbol, :id)", name=stock["name"], shares=shares, price=usd(stock["price"]), total=usd(shares * stock["price"]), symbol=stock["symbol"], id=session["user_id"])
            
        else:
            totalShares = userShares[0]["shares"] + shares
            db.execute("UPDATE portfolio SET shares=:shares WHERE id=:id AND symbol=:symbol", shares=totalShares, id=session["user_id"], symbol=stock["symbol"])
        
        return redirect(url_for("index"))
        
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    history = db.execute("SELECT * FROM history WHERE id=:id", id=session["user_id"])
    
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        
        # make sure stock field is filled
        if not request.form.get("symbol"):
            return apology("missing symbol")
        
        # store stock data in stock
        stock = lookup(request.form.get("symbol"))
        
        if not stock:
            return apology("invalid symbol")
        
        # return rendering of stock value 
        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        
        # make sure username field is filled
        if not request.form.get("username"):
            return apology("missing username")
        
        # make sure password field is filled
        if not request.form.get("password"):
            return apology("missing password")
        
        # make sure passwordcheck field is filled
        if not request.form.get("passwordcheck"):
            return apology("confirm your password!")
        
        # check whether password is equal to password check
        if request.form.get("password") != request.form.get("passwordcheck"):
            return apology("passwords do not match")
    
        # add user info to users datatable
        registrant = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=pwd_context.hash(request.form.get("password")))
    
        if not registrant:
            return apology("username is already in use")
            
        # remember if user has logged in
        session["user_id"] = registrant
        
        # on success return to homepage
        return redirect(url_for("index"))
    
    else:
        return render_template("register.html")
 
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        
        # check conditions
        if not request.form.get("symbol"):
            return apology("missing symbol")
        if not request.form.get("shares"):
            return apology("missing shares")
            
        # check if valid stock
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("invalid stock")
            
        # check if valid number of shares
        shares = int(request.form.get("shares"))
        if shares < 0:
            return apology("invalid shares")
            
        # select shares from portfolio
        userShares = db.execute("SELECT shares FROM portfolio WHERE id=:id AND symbol=:symbol", id=session["user_id"], symbol=stock["symbol"])
        
        if userShares[0]["shares"] < shares or not userShares:
            return apology("not enough shares")
        
        # update history
        db.execute("INSERT INTO history (symbol, shares, price, id) VALUES(:symbol, :shares, :price, :id)", symbol=stock["symbol"], shares=-shares, price=usd(stock["price"]), id=session["user_id"])
        
        # update cash
        db.execute("UPDATE users SET cash = cash + :sale WHERE id=:id", sale=stock["price"]*float(shares), id=session["user_id"])
        
        # change share count
        totalShares = userShares[0]["shares"] - shares

        # update database
        if totalShares == 0:
            db.execute("DELETE FROM portfolio WHERE id=:id AND symbol=:symbol", id=session["user_id"], symbol=stock["symbol"])
        else:
            db.execute("UPDATE portfolio SET shares=:shares WHERE id=:id AND symbol=:symbol", shares=totalShares, id=session["user_id"], symbol=stock["symbol"])
            
        return redirect(url_for("index"))
    else:
        return render_template("sell.html")
        
@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    
    if request.method == "POST":
        
        # make sure password field is filled
        if not request.form.get("newpassword"):
            return apology("missing newpassword")
        
        # make sure passwordcheck field is filled
        if not request.form.get("passwordcheck"):
            return apology("confirm your password!")
        
        # check whether newpassword is equal to password check
        if request.form.get("newpassword") != request.form.get("passwordcheck"):
            return apology("passwords do not match")

        # add user info to users datatable
        registrant = db.execute("UPDATE users SET hash=:hash WHERE id=:id", hash=pwd_context.hash(request.form.get("newpassword")), id=session["user_id"])
        
        # on success return to homepage
        return redirect(url_for("index"))
    
    else:
        return render_template("password.html")
