s = int(input("enter half pyramids height:"))

while s < 0 or s > 23:
    s = int(input("retry half pyramid height:"))

height = s * 2
base = height + 1

for i in range(height):
    print(" "*(base - i - 2), "#"*(i+2))
