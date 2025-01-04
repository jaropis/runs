with open("adamek.csv", "r") as file:
    prev_1 = -1
    prev_2 = -2
    count= 0
    for elem in file:
        elem1, elem2 = elem.split()
        elem1 = float(elem1)
        if elem1 == prev_1 == prev_2:
            count += 1
            print(prev_2, prev_1, elem1)
        prev_2 = prev_1
        prev_1 = elem1
    print(f"full number of 2s: {count}")

