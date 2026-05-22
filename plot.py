import csv
import numpy as np
import matplotlib.pyplot as plt

sizes = []
linear = []
bst = []
rbt = []
hash_table = []
multimap = []

with open('timings.csv', newline='', encoding='utf-8') as f:
    reader = csv.reader(f, delimiter=';')
    next(reader)

    for row in reader:
        sizes.append(int(row[0]))
        linear.append(float(row[1]))
        bst.append(float(row[2]))
        rbt.append(float(row[3]))
        hash_table.append(float(row[4]))
        multimap.append(float(row[5]))

x = np.arange(len(sizes))

plt.figure(figsize=(12, 6))

plt.plot(x, linear, marker='o', label='Linear')
plt.plot(x, bst, marker='o', label='BST')
plt.plot(x, rbt, marker='o', label='RBT')
plt.plot(x, hash_table, marker='o', label='HashTable')
plt.plot(x, multimap, marker='o', label='Multimap')

plt.title('Сравнение методов поиска')
plt.xlabel('Размер массива')
plt.ylabel('Время (мс)')

plt.xticks(x, sizes, rotation=45)

plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()

collision_sizes = []
collisions = []

with open('collisions.csv', newline='', encoding='utf-8') as f:
    reader = csv.reader(f, delimiter=';')
    next(reader)

    for row in reader:
        collision_sizes.append(int(row[0]))
        collisions.append(int(row[1]))

x2 = np.arange(len(collision_sizes))

plt.figure(figsize=(12, 6))

plt.plot(x2, collisions, marker='o')

plt.title('Количество коллизий')
plt.xlabel('Размер массива')
plt.ylabel('Кол-во коллизий')

plt.xticks(x2, collision_sizes, rotation=45)

plt.grid(True)

plt.tight_layout()
plt.show()