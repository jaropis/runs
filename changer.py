# changes the data to the format used by hraexplorer
all = []
with open("adamek.csv", "r") as file:
    for elem in file:
        all.append(elem)

with open("adamekRR.csv", "w") as file:
    file.write("RR\tannot\n")
    for elem in all:
        elem1, elem2 = elem.split()
        elem1 = float(elem1) * 1000
        file.write(f"{elem1}\t{elem2}\n")

