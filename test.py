from bintreeFile import Bintree

svenska = Bintree()
with open("word3.txt", "r", encoding = "utf-8") as svenskfil:
    for rad in svenskfil:
        ordet = rad.strip()                # Ett trebokstavsord per rad
        if ordet in svenska:
            print(ordet, end = " ") 
        else:
            svenska.put(ordet)             # in i sökträdet
print("\n")


engelska = Bintree()
with open("engelska.txt", "r", encoding = "utf-8") as engelskafil:
    for rad in engelskafil:
        raden = rad.split()
                        # Ett trebokstavsord per rad
        for ordet in raden:
            ordet = ordet.strip("\"").strip(".").strip(",").strip(" ").strip("\"").strip("!").lower()
            if ordet in engelska:
                x=1   
            else:
                engelska.put(ordet)
                if ordet in svenska:
                    print(ordet, end = " ")
                            
print("\n")
engelska.write()
