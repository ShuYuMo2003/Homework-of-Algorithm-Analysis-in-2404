import random

possible_car_id = (
'''BRA6763438
BRA9890980
BRA4784724
BRA7647216
BRA5114764
BRA6071201
BRA4257176
BRA5487735
BRA6787078
BRA9129502
BRA8135039
BRA3690212
BRA9638875
BRA3902870
BRA7310644
BRA5374409
BRA2748154
BRA2092129
BRA2934296
BRA7072983''').split('\n')

with open("test_data.txt", "w") as f:
    # push 20 cars.
    for i in range(20):
        f.write(f'''
1
BRAND_{random.randint(1, 5)}
{random.choice(['yellow', 'red', 'blue', 'green', 'black', 'white', 'silver', 'orange', 'purple', 'pink'])}
{random.randint(10, 1000)}
{random.choice(['China', 'USA', 'Germany', 'Japan', 'Korea', 'Italy', 'France', 'UK', 'Russia', 'India'])}
{random.randint(1900, 2024)}
''')

    f.write("7\n")
    f.write("6\n")

    for i in range(40):
        f.write(f'''
10
CUS_{random.randint(1, 5)}
{''.join(random.sample('abcdefghijklmnopqrstuvwxyz', 5))}
{random.choices(possible_car_id)[0]}
{random.randint(1, 20)}
{random.randint(2000, 2024)} {random.randint(1, 12)} {random.randint(1, 30)}
{random.choice(['No', 'HighDiscount', 'LowDiscount', 'Free'])}
''')
