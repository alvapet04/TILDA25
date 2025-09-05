

class Node():

    def __init__(self, x):
        self.value = x
        self.left = None 
        self.right = None

class Bintree():
    def __init__ (self):
        self.root = None

    def put(self,newvalue):
        # Sorterar in newvalue i trädet
        self.root = putta(self.root,newvalue)

    def __contains__(self,value):
        # True om value finns i trädet, False annars
        return finns(self.root,value)

    def write(self):
        # Skriver ut trädet i inorder
        skriv(self.root)
        print("\n")


def putta(p, newvalue):
     # Funktion som gör själva jobbet att stoppa in en ny nod
    if not p:
        return Node(newvalue)
    
    elif newvalue > p.value:
            p.right = putta(p.right, newvalue)

    elif newvalue < p.value:
            p.left = putta(p.left, newvalue)
    return p
    
        

def finns(p,value):
     # Funktion som gör själva jobbet att söka efter ett värde
    while p != None:
        if value == p.value:
            return True
        elif value > p.value:
            p = p.right
        else:
            p = p.left
    
    return False

def skriv(p):
     # Funktion som gör själva jobbet att skriva ut trädet
    if not p:
        return 
    skriv(p.left)
    print(p.value)
    skriv(p.right)




