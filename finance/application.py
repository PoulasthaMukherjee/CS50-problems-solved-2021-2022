import os

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
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    remaining = cash[0]['cash']
    cash = remaining
    shares = db.execute("SELECT symbol, shares FROM purchases WHERE user_id = ? GROUP BY symbol", session["user_id"])

    for share in shares:
        share.update(lookup(share["symbol"]))
        share.update({"total": share["shares"] * share["price"]})
        cash += share["total"]
        share["price"] = usd(share["price"])
        share["total"] = usd(share["total"])

    return render_template("index.html", remaining=usd(remaining), cash=usd(cash), shares=shares)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("MISSING SYMBOL", 400)

        elif not request.form.get("shares"):
            return apology("MISSING SHARES", 400)

        elif not lookup(request.form.get("symbol")):
            return apology("INVALID SYMBOL", 400)

        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("INVALID SHARES", 400)

        if not shares > 0:
            return apology("INVALID SHARES", 400)

        else:
            quote_sym = lookup(request.form.get("symbol"))
            total = quote_sym['price'] * shares
            cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

            cash = cash[0]['cash']
            cash -= total

            if cash >= 0:
                # update existing purchase if possible
                db.execute("UPDATE purchases SET shares = shares + ? WHERE user_id = ? AND symbol = ?",
                            shares, session["user_id"], quote_sym['symbol'])

                # create new purchase if no successful update
                db.execute("INSERT INTO purchases (user_id, symbol, shares) SELECT ?, ?, ? WHERE (SELECT Changes() = 0)",
                            session["user_id"] , quote_sym['symbol'], shares)

                db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES(?, ?, ?, ?)",
                            session["user_id"] , quote_sym['symbol'], shares, quote_sym['price'])

                db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])

                flash("Bought!")
                return redirect("/")
            else:
                return apology("CAN'T AFFORD", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM history WHERE user_id = ?",session["user_id"])

    for row in history:
        row["price"] = usd(row["price"])

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id,
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
                          username=request.form.get("username"))

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
        if not request.form.get("symbol"):
            return apology("MISSING SYMBOL", 400)

        elif not lookup(request.form.get("symbol")):
            return apology("INVALID SYMBOL", 400)

        else:
            quote_sym = lookup(request.form.get("symbol"))

            return render_template("quoted.html", name=quote_sym['name'], symbol=quote_sym['symbol'], price=usd(quote_sym['price']))

    # User reached route via GET (as by clicking a link or via redirect)
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
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("passwords must match", 400)

        username = request.form.get("username")
        password = generate_password_hash(request.form.get("password"))


        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if len(rows) == 1:
            return apology("username already registered", 400)

        else:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)",
               username, password)

            rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

            # Remember which user has logged in
            session["user_id"] = rows[0]["id"]

            # Redirect user to home page
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # get value from dropdown but don't know how to name dropdowns yet
        print(request.form.get("symbol"))
        shares = db.execute("SELECT symbol, shares FROM purchases WHERE user_id = ? GROUP BY symbol", session["user_id"])

        shares = shares[0]
        print(shares)

        sell = int(request.form.get("shares"))
        available = shares['shares']

        if not sell > available:
            sell_price = lookup(request.form.get("symbol"))
            sell_price = sell_price['price']

            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", sell_price * sell, session["user_id"])


            if sell == available:
                db.execute("DELETE FROM purchases WHERE user_id = ? AND symbol = ?", session["user_id"], shares['symbol'])
            else:
                db.execute("UPDATE purchases SET shares = shares - ? WHERE user_id = ? AND symbol = ?",
                            sell, session["user_id"], shares['symbol'])

            sell = 0 - sell
            db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES(?, ?, ?, ?)",
                        session["user_id"] , shares['symbol'], sell, sell_price)
            flash("Sold!")
            return redirect("/")

        else:
            return apology("NOT ENOUGH SHARES", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        shares = db.execute("SELECT DISTINCT symbol FROM purchases WHERE user_id = ?", session["user_id"])
        options = []
        for share in shares:
            options.append(share['symbol'])

        return render_template("sell.html", options=options)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)