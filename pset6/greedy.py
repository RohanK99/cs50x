change = float(input("How much change is owed?: "))

while change < 0:
    change = float(input("How much change is owed?: "))

count = 0

while change >= 25:
    change = change - 25
    count += 1

while change >= 10:
    change = change - 10
    count += 1

while change >= 5:
    change = change - 5
    count += 1

while change >= 1:
    change = change - 1
    count += 1

print(count)