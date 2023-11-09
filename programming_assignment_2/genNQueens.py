n = 6
with open(str(n) + "_queens.kb", 'w') as f:
    # Assert possible values
    f.write("(or")
    for i in range(1, n+1):
        for j in range(1, n+1):
            f.write(" Q" + str(i) + str(j))
    f.write(")\n")
    # Assert that there could only be one queen in each column, row, diagonal
    for i in range(1, n+1):
        #f.write()
        for j in range(1, n+1):
            f.write("(implies Q" + str(i) + str(j) + " (and ")
            # Check Colunm
            for k in range(1, n+1):
                if (k == i): continue
                f.write("(not Q" + str(k) + str(j) + ") ")
            # Check Row
            for k in range(1, n+1):
                if (k == j): continue
                f.write("(not Q" + str(i) + str(k) + ") ")
            # Check \ diagonal
            k = i
            l = j
            while (k > 1 and l > 1):
                k -= 1
                l -= 1
            while (k < n+1 and l < n+1):
                if (k == i and l == j): 
                    k += 1
                    l += 1
                    continue
                f.write("(not Q" + str(k) + str(l) + ") ")
                k += 1
                l += 1
            # Check / diagonal
            k = i
            l = j
            while (k > 1 and l < n):
                k -= 1
                l += 1
            while (k < n+1 and l > 0):
                if (k == i and l == j):
                    k += 1
                    l -= 1
                    continue
                f.write("(not Q" + str(k) + str(l) + ") ")
                k += 1
                l -= 1
            f.write("))\n")
    # Assert that there is at least one queen in every row
    for i in range(1, n+1):
        f.write("(or")
        for j in range(1, n+1):
            f.write(" Q" + str(i) + str(j))
        f.write(")\n")
    # Assert that there is at least one queen in every column
    for j in range(1, n+1):
        f.write("(or")
        for i in range(1, n+1):
            f.write(" Q" + str(i) + str(j))
        f.write(")\n")