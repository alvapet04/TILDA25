from linkedQFile import LinkedQ
import unittest


PERIODISKA_SYSTEMET= ("H   He  Li  Be  B   C   N   O   F   Ne  Na  Mg  Al  Si  P   S   Cl  Ar  K   Ca  Sc  Ti  V   Cr \
Mn  Fe  Co  Ni  Cu  Zn  Ga  Ge  As  Se  Br  Kr  Rb  Sr  Y   Zr  Nb  Mo  Tc  Ru  Rh  Pd  Ag  Cd \
In  Sn  Sb  Te  I   Xe  Cs  Ba  La  Ce  Pr  Nd  Pm  Sm  Eu  Gd  Tb  Dy  Ho  Er  Tm  Yb  Lu  Hf\
Ta  W   Re  Os  Ir  Pt  Au  Hg  Tl  Pb  Bi  Po  At  Rn  Fr  Ra  Ac  Th  Pa  U   Np  Pu  Am  Cm\
Bk  Cf  Es  Fm  Md  No  Lr  Rf  Db  Sg  Bh  Hs  Mt  Ds  Rg  Cn  Fl  Lv").split()

COUNTER = 0

#Ärver all från Exception
class Syntaxfel(Exception):
    pass

#<formel>::= <mol> \n
def formel (molekylkö):
    mol(molekylkö)

#<mol>   ::= <group> | <group><mol>
def mol (molekylkö, räknare = 0):
    grupp(molekylkö)
    if not molekylkö.isEmpty():
        if molekylkö.peek() == ")" and räknare: 
            return
        mol(molekylkö, räknare)

#<group> ::= <atom> |<atom><num> | (<mol>) <num>
def grupp(molekylkö):
    if molekylkö.peek() == "(":
        molekylkö.dequeue()
        mol(molekylkö, 1)
        if molekylkö.peek() == ")":
            molekylkö.dequeue()
            if molekylkö.peek().isdigit():
                tal(molekylkö)
                return
            raise Syntaxfel ("Saknad siffra")
        else:
            raise Syntaxfel("Saknad högerparentes")
    elif (molekylkö.peek() == ")") or (molekylkö.peek().isdigit()):
        raise Syntaxfel ("Felaktig gruppstart")
    else:
        atom(molekylkö)
        if molekylkö.peek().isdigit():
            tal(molekylkö)
    
        
    

#<molekyl> ::= <atom> | <atom><num>
#def molekylisen (molekylkö):
    #atom(molekylkö)
    #if not molekylkö.isEmpty():
        #tal(molekylkö)

#<atom>  ::= <LETTER> | <LETTER><letter>
def atom(molekylkö):
    stor = stor_bokstav(molekylkö)
    liten = liten_bokstav(molekylkö)
    if liten:
        namn = stor + liten
    else:
        namn = stor
    for element in  PERIODISKA_SYSTEMET:
        if element == namn:
            return
    raise Syntaxfel ("Okänd atom")


#<LETTER>::= A | B | C | ... | Z
def stor_bokstav(molekylkö):
    stor = molekylkö.peek()
    if stor.isalpha() and stor.isupper():
        stor = molekylkö.dequeue()
        return stor
    raise Syntaxfel ("Saknad stor bokstav")


#<letter>::= a | b | c | ... | z
def liten_bokstav(molekylkö):
    liten = molekylkö.peek()
    if liten.isalpha() and liten.islower():
        liten = molekylkö.dequeue()
        return liten



#<num>   ::= 2 | 3 | 4 | ...
def tal(molekylkö):
    if not molekylkö.isEmpty():
        siffra = molekylkö.dequeue()
        if (siffra.isdigit() and int(siffra) >= 2):
            #siffra = molekylkö.dequeue()
            tömma_kön(molekylkö)
            return
        elif siffra.isdigit() and int(siffra) == 1:
            #siffra = molekylkö.dequeue()
            if molekylkö.isEmpty() or not molekylkö.peek().isdigit():
                raise Syntaxfel ("För litet tal")
            tömma_kön(molekylkö)
            return
        raise Syntaxfel ("För litet tal")
    
def tömma_kön(molekylkö):
    if not molekylkö.isEmpty():
        if molekylkö.peek().isdigit():
            molekylkö.dequeue()
            tömma_kön(molekylkö)
            return
        
def skrivut(molekylkö):
    if not molekylkö.isEmpty():
        print ( " ", end = "") 
    while not molekylkö.isEmpty():
        tecken = molekylkö.dequeue()
        print ( tecken, end = "")
    print()

def lagra_molekyl(molekylen):
    molekylkö = LinkedQ()
    for tecken in molekylen:
        molekylkö.enqueue(tecken)
    return molekylkö

def kolla_molekylen(molekylkö):
    try:
        formel(molekylkö)
        return "Formeln är syntaktiskt korrekt"
    except Syntaxfel as fel:
        return str (fel) + " vid radslutet"+ str(molekylkö)

def main():
    molekylen = input()
    while molekylen != "#":
        molekylkö = lagra_molekyl(molekylen)
        resultat = kolla_molekylen(molekylkö)
        print (resultat)
        molekylen = input()

if __name__ == '__main__':
    main()