import os

from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("    postgres://pdlqfnpkehjffb:c7b388NH9ZsXBsvDgfeJuCJ5GPxzsjc6sEBqdd9905d0bf820c32a714a2541765@ec2-34-202-88-122.compute-1.amazonaws.com:5432/d493met7lh4tq0")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

balance = 0

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # stocks = db.execute("SELECT hsymbol FROM history WHERE huser=:huser GROUP BY hsymbol",huser=session["user_id"])
    balance = db.execute("SELECT cash FROM users WHERE id=:user_id",user_id=session["user_id"])
    my_balance=round(balance[0]['cash'],2)
    all_list = db.execute("SELECT hsymbol, hcname, sum(hshares) total_shares, hprice FROM history WHERE huser=:huser GROUP BY hsymbol HAVING sum(hshares) > 0",huser=session["user_id"])

    stock_price = {}
    price = 0
    for keys in all_list:
        stock_price[keys["hsymbol"]] = lookup(keys["hsymbol"])
        price += stock_price[keys["hsymbol"]]["price"] * keys["total_shares"]
    return render_template("index.html",usd=usd,stock=stock_price, dataa=all_list, balance=my_balance, price=price)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        balance = db.execute("SELECT cash FROM users WHERE id=:user_id",user_id=session["user_id"])
        shares = int(request.form.get("shares"))
        symbol_data = lookup(request.form.get("symbol"))
        if symbol_data == None:
            return apology("Stock does not exist")
        elif int(shares) < 1:
            return apology("Number of stock should be greater than 0")
        total=symbol_data["price"] * shares
        my_balance=balance[0]['cash']
        if total > my_balance:
            return apology("You do not have enough balance")
        else:
            my_balance = my_balance -total
            db.execute("UPDATE users set cash= :cash WHERE id = :user_id", cash=my_balance, user_id = session["user_id"])
            now = datetime.now()
            db.execute("INSERT INTO history (huser, hsymbol, hshares, hprice, htime, hcname) VALUES (:huser, :hsymbol, :hshares, :hprice, :htime, :hcname)", huser=session["user_id"],hsymbol=symbol_data["symbol"],hshares=shares,hprice=symbol_data["price"],htime=now.strftime("%m/%d/%Y, %H:%M:%S"),hcname=symbol_data["name"])
            # new_balance = balance - symbol_data["price"] * int(shares)
            return redirect("/")
    else:

        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    hist = db.execute("SELECT hsymbol, hshares, hprice, htime FROM history WHERE huser=:huser",huser=session["user_id"])
    return render_template("history.html",history = hist, usd=usd)

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Add cash"""
    if request.method == "POST":
        mycash = float(request.form.get("cash"))
        balance = db.execute("SELECT cash FROM users WHERE id=:user_id",user_id=session["user_id"])
        my_balance=balance[0]['cash']
        mycash = mycash + my_balance
        db.execute("UPDATE users set cash= :cash WHERE id = :user_id", cash=mycash, user_id = session["user_id"])
        return redirect("/")

    else:
        balance = db.execute("SELECT cash FROM users WHERE id=:user_id",user_id=session["user_id"])
        my_balance=round(balance[0]['cash'],2)
        return render_template("cash.html",balance=my_balance)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username").lower())

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol_data = lookup(request.form.get("symbol"))
        symbol_data["price"] = usd(symbol_data["price"])
        if symbol_data == None:
            return apology("Stock does not exist")
        else:
            return render_template("quoted.html",symbol_data=symbol_data)
    else:

        return render_template("quote.html")




@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
        # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        #check if user entered same password twice
        elif request.form.get("password") != request.form.get("confirm_password"):
            return apology("Confirmed password does not match", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username").lower())

        # Ensure username is unique
        if len(rows) == 1 : # not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Username exsists, try another one", 403)

        # Save username and password in the database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)", username=request.form.get("username".lower()), password=generate_password_hash(request.form.get("password"), "sha256"))

        # Remember which user has logged in
        #session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    balance = db.execute("SELECT cash FROM users WHERE id=:user_id",user_id=session["user_id"])
    my_stocks = db.execute("SELECT hsymbol FROM history WHERE huser=:huser GROUP BY hsymbol HAVING sum(hshares) > 0",huser=session["user_id"])

    if request.method == "POST":
        shares = int(request.form.get("shares"))
        symbol_data = lookup(request.form.get("symbol"))
        total=symbol_data["price"] * shares
        my_balance=balance[0]['cash']
        my_balance = my_balance +total
        if symbol_data == None:
            return apology("Stock does not exist (INVALID STOCK)")
        if int(shares) < 1:
            return apology("Number of stock should be greater than 0")
        # elif shares > stock in my_stocks
        stock = db.execute("SELECT SUM(hshares) total_shares FROM history WHERE huser = :huser AND hsymbol = :hsymbol GROUP BY hsymbol",huser=session["user_id"], hsymbol=request.form.get("symbol"))

        if stock[0]["total_shares"] <= 0 or stock[0]["total_shares"] < shares:
            return apology("INVALID STOCK")

        db.execute("UPDATE users set cash= :cash WHERE id = :user_id", cash=my_balance, user_id = session["user_id"])
        now = datetime.now()
        db.execute("INSERT INTO history (huser, hsymbol, hshares, hprice, htime, hcname) VALUES (:huser, :hsymbol, :hshares, :hprice, :htime, :hcname)", huser=session["user_id"],hsymbol=symbol_data["symbol"],hshares=-1 *shares,hprice=symbol_data["price"],htime=now.strftime("%m/%d/%Y, %H:%M:%S"),hcname=symbol_data["name"])
        # new_balance = balance - symbol_data["price"] * int(shares)
        return redirect("/")

    else:
        return render_template("sell.html",stocks=my_stocks)



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
