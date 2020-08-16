from cs50 import SQL

db = SQL("sqlite:///finance.db")

# balance = db.execute("SELECT cash FROM users WHERE username=:username",username="ashu")
# print(balance[0]['cash'])
my_stocks = db.execute("SELECT hsymbol, sum(hshares) total_shares FROM history WHERE huser=:huser GROUP BY hsymbol HAVING sum(hshares) > 0",huser=2)
print(my_stocks)