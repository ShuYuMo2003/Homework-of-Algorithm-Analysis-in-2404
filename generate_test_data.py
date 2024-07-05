import random

with open("test_data.txt", "w") as f:
    # push 20 cars.
    for i in range(20):
        f.write(f'''
1
BRAND_{i}
{random.choice(['yellow', 'red', 'blue', 'green', 'black', 'white', 'silver', 'orange', 'purple', 'pink'])}
{random.randint(10, 1000)}
{random.choice(['China', 'USA', 'Germany', 'Japan', 'Korea', 'Italy', 'France', 'UK', 'Russia', 'India'])}
{random.randint(1900, 2024)}
''')

    f.write("7\n")
    f.write("6\n")
