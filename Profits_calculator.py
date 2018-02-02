# importing csv module
import csv

# Pair(s) to process
PAIR = "XRPBTC"

# csv file name
#filename = "TradeHistory.csv"
filename = "TestData.csv"

# initializing the titles and rows list
fields = []
rows = []

# reading csv file
with open(filename, 'r') as csvfile:

    # creating a csv reader object
    csvreader = csv.reader(csvfile)

    # extracting field names through first row
    fields = next(csvreader)

    # extracting each data row one by one
    for row in csvreader:
        rows.append(row)

qty = 0.0;
totalSpent = 0.0;
buys = 0;
sells = 0;
    
for row in reversed(rows[:]):
    if PAIR in row and "BUY" in row:
        qty += float(row[4])
        totalSpent += float(row[5])
        buys += 1

        #print(row)
    if PAIR in row and "SELL" in row:
        qty -= float(row[4])
        totalSpent -= float(row[5])
        sells += 1

average = totalSpent/qty;

print("\n")
print("TotalSpent: {} BTC\nbuys: {}\nsells: {}\nqty: {}\naverage: {}".format(totalSpent, buys, sells, qty, round(average,8)))
