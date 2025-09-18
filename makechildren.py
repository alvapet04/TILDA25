from bintreeFile import Bintree
from linkedQFile import LinkedQ


class ParentNode:
    def __init__(self, word, parent = None):
        self.word = word
        self.parent = parent

def alla_ord ():
    svenska = Bintree()
    gamla = Bintree()
    with open("word3.txt", "r", encoding = "utf-8") as ordfil:
        for rad in ordfil:
            ordet = rad.strip()                # Ett trebokstavsord per rad
            if ordet not in svenska:
                svenska.put(ordet)             # in i sökträdet
    return svenska, gamla

def make_children(start_kö, ordträd, gamla, slutord):
    alfabetet = "abcdefghijklmnopqrstuvwxyzåäö"
    startord = start_kö.dequeue()
    gamla.put(startord.word)
    for a in range(len(startord.word)):
        for symbol in alfabetet:
            vårt_ord =list(startord.word)
            vårt_ord[a] = symbol
            vårt_ord = "".join(vårt_ord)
            if vårt_ord in ordträd and vårt_ord not in gamla:
                    #if vårt_ord == slutord:
                      #   ord = (ParentNode(slutord, startord))   
                     #    return ord   
                    gamla.put(vårt_ord)
                    vårt_ord = ParentNode(vårt_ord, startord)
                    start_kö.enqueue(vårt_ord)       
    return start_kö
                
                 
def fråga_om_ord():
    startord = input(f"Vilket är ditt startord?").strip()
    slutord = input(f"Vilket är ditt slutord?").strip()
    return startord, slutord


def utskrift(start_kö, svenska, slutord, gamla):
        if start_kö.isEmpty():
             print(f"Det finns ingen väg till {slutord}")
             return None
        ord = start_kö.first.value
        if  ord.word == slutord:
                print (f"Det finns en väg till {slutord}")
                return start_kö.first.value
        else:
            start_kö = make_children(start_kö, svenska, gamla, slutord)
            return utskrift(start_kö, svenska, slutord, gamla)
    

def hela_utskriften(slutord):
    if slutord != None:
        hela_utskriften(slutord.parent)
        print(slutord.word)
     

def main():
    svenska, gamla = alla_ord()
    startord, slutord = fråga_om_ord()
    if startord not in svenska:
         print("Ditt startord finns inte i ordlistan!")
         return
    elif slutord not in svenska:
         print("Ditt slutord finns inte i ordlistan!")
         return
    start_kö = LinkedQ()
    startord = ParentNode(startord)
    start_kö.enqueue(startord)
    slutord = utskrift(start_kö, svenska, slutord, gamla)
    if slutord != None:
        hela_utskriften(slutord)
    
    
main()


         



