from linkedQFile import LinkedQ
import unittest


#Ärver all från Exception
class Syntaxfel(Exception):
    pass

#<molekyl> ::= <atom> | <atom><num>
def molekylisen (molekylkö):
    atom(molekylkö)
    if not molekylkö.isEmpty():
        tal(molekylkö)

#<atom>  ::= <LETTER> | <LETTER><letter>
def atom(molekylkö):
    stor_bokstav(molekylkö)
    liten_bokstav(molekylkö)


#<LETTER>::= A | B | C | ... | Z
def stor_bokstav(molekylkö):
    stor = molekylkö.peek()
    if stor.isalpha() and stor.isupper():
        stor = molekylkö.dequeue()
        return
    raise Syntaxfel ("Saknad stor bokstav")


#<letter>::= a | b | c | ... | z
def liten_bokstav(molekylkö):
    liten = molekylkö.peek()
    if liten.isalpha() and liten.islower():
        liten = molekylkö.dequeue()



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
        tecken = molekylkö.dequeue()
        if tecken.isdigit():
            tömma_kön(molekylkö)
            return
        raise Syntaxfel ("Inte ett tal")
        
def skrivut(molekylkö):
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
        molekylisen(molekylkö)
        return "Formeln är syntaktiskt korrekt"
    except Syntaxfel as fel:
        return str (fel) + " vid radslutet "+ str(molekylkö)

def main():
    molekylen = input()
    while molekylen != "#":
        molekylkö = lagra_molekyl(molekylen)
        resultat = kolla_molekylen(molekylkö)
        print (resultat)
        molekylen = input()

if __name__ == '__main__':
    main()