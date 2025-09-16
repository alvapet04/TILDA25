from bintreeFile import Bintree
from linkedQFile import LinkedQ


class Node():

    def __init__(self, x, next=None):
        self.value = x
        self.next = next

def alla_ord ():
    svenska = Bintree()
    gamla = Bintree()
    with open("word3.txt", "r", encoding = "utf-8") as ordfil:
        for rad in ordfil:
            ordet = rad.strip()                # Ett trebokstavsord per rad
            if ordet not in svenska:
                svenska.put(ordet)             # in i sökträdet
    return svenska, gamla

def make_children(start_kö, ordträd, gamla):
    alfabetet = "abcdefghijklmnopqrstuvwxyzåäö"
    startord = start_kö.dequeue()
    gamla.put(startord)
    for a in range(len(startord)):
        for symbol in alfabetet:
            vårt_ord =list(startord)
            vårt_ord[a] = symbol
            vårt_ord = "".join(vårt_ord)
            if vårt_ord in ordträd and vårt_ord not in gamla:
                    gamla.put(vårt_ord)
                    start_kö.enqueue(vårt_ord)       
    return start_kö
                
                 
def fråga_om_ord():
    startord = input(f"Vilket är ditt startord?")
    slutord = input(f"Vilket är ditt slutord?")
    return startord, slutord

def utskrift(start_kö, svenska, slutord, gamla):
        if start_kö.isEmpty():
             print(f"Det finns ingen väg till {slutord}")
             return
        word = start_kö.first.value
        if  word == slutord:
                print (f"Det finns en väg till {slutord}")
                return 
        else:

            start_kö = make_children(start_kö, svenska, gamla)
            utskrift(start_kö, svenska, slutord, gamla)
    

def main():
    svenska, gamla = alla_ord()
    startord, slutord = fråga_om_ord()
    start_kö = LinkedQ()
    start_kö.enqueue(startord)
    utskrift(start_kö, svenska, slutord, gamla)
    
main()


         



