from linkedQFile import LinkedQ
from molgrafik import Molgrafik
from hashtest import *
import unittest


PERIODISKA_SYSTEMET= ("H   He  Li  Be  B   C   N   O   F   Ne  Na  Mg  Al  Si  P   S   Cl  Ar  K   Ca  Sc  Ti  V   Cr \
Mn  Fe  Co  Ni  Cu  Zn  Ga  Ge  As  Se  Br  Kr  Rb  Sr  Y   Zr  Nb  Mo  Tc  Ru  Rh  Pd  Ag  Cd \
In  Sn  Sb  Te  I   Xe  Cs  Ba  La  Ce  Pr  Nd  Pm  Sm  Eu  Gd  Tb  Dy  Ho  Er  Tm  Yb  Lu  Hf\
Ta  W   Re  Os  Ir  Pt  Au  Hg  Tl  Pb  Bi  Po  At  Rn  Fr  Ra  Ac  Th  Pa  U   Np  Pu  Am  Cm\
Bk  Cf  Es  Fm  Md  No  Lr  Rf  Db  Sg  Bh  Hs  Mt  Ds  Rg  Cn  Fl  Lv").split()

class Ruta:
    def __init__(self, atom="( )", num=1):
        self.atom = atom
        self.num = num
        self.next = None
        self.down = None

#Ärver all från Exception
class Syntaxfel(Exception):
    pass

#<formel>::= <mol> \n
def formel (molekylkö):
    molekylen = mol(molekylkö)
    return molekylen

#<mol>   ::= <group> | <group><mol>
def mol (molekylkö, räknare = 0):
    molekylens = grupp(molekylkö)
    if not molekylkö.isEmpty():
        if molekylkö.peek() == ")" and räknare: 
            return molekylens
        molekylens.next = mol(molekylkö, räknare)
    return molekylens

#<group> ::= <atom> |<atom><num> | (<mol>) <num>
def grupp(molekylkö):
    if molekylkö.peek() == "(":
        ruta = Ruta()
        molekylkö.dequeue()
        ruta.down = mol(molekylkö, 1)
        if molekylkö.peek() == ")":
            molekylkö.dequeue()
            if molekylkö.peek().isdigit():
                ruta.num = int(tal(molekylkö))
                return ruta
            raise Syntaxfel ("Saknad siffra")
        else:
            raise Syntaxfel("Saknad högerparentes")
    elif (molekylkö.peek() == ")") or (molekylkö.peek().isdigit()):
        raise Syntaxfel ("Felaktig gruppstart")
    else:
        ruta = Ruta()
        ruta.atom = atom(molekylkö)
        if molekylkö.peek().isdigit():
            ruta.num = int(tal(molekylkö))
        return ruta
    

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
            return namn
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
            siffersträng = tömma_kön(molekylkö)
            if siffersträng:
                return siffra + siffersträng
            return siffra
        elif siffra.isdigit() and int(siffra) == 1:
            if molekylkö.isEmpty() or not molekylkö.peek().isdigit():
                raise Syntaxfel ("För litet tal")
            siffersträng = tömma_kön(molekylkö)
            if siffersträng:
                return siffra + siffersträng
            return siffra
        raise Syntaxfel ("För litet tal")
    
def tömma_kön(molekylkö):
    if not molekylkö.isEmpty():
        if molekylkö.peek().isdigit():
            siffra = molekylkö.dequeue()
            siffersträng = tömma_kön(molekylkö)
            if siffersträng:
                return siffra + siffersträng
            return siffra

def vikt(vår_formel, atomhash):
    totvikt = 0
    if vår_formel.down != None:
        vikten = vikt(vår_formel.down, atomhash)
        vikten *= vår_formel.num
        totvikt += vikten
    else:
        atom_vikt = atomhash.search(vår_formel.atom).vikt
        atom_vikt *= vår_formel.num
        totvikt += atom_vikt

    if vår_formel.next:
        viktisen = vikt(vår_formel.next, atomhash)
        totvikt += viktisen

    return totvikt

        
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
        vår_formel = formel(molekylkö)
        atomhash = lagraHashtabell(skapaAtomlista())
        print (vikt(vår_formel, atomhash))
        mg = Molgrafik()
        mg.show(vår_formel)
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