import csv

class HashNode:
    #Noder till klassen Hashtable 
    def __init__(self, key = "", data = None):
        #key är nyckeln som anvands vid hashningen
        #data är det objekt som ska hashas in
        self.key = key
        self.data = data

#Fyll i kod här nedan för att initiera hashtabellen

class Hashtable:

   def __init__(self, size):
      #size: hashtabellens storlek
      self.size = size
      self.tabell =[[]for _ in range(size)]

   def store(self, key, data):
        #key är nyckeln
         #data är objektet som ska lagras
         #Stoppar in "data" med nyckeln "key" i tabellen.
        try:
            self.search(key)
            for hashNode in self.tabell[self.hashfunction(key)]:
                if hashNode.key == key:
                    hashNode.data = data
        except KeyError:
            lagrat = HashNode(key, data)
            self.tabell[self.hashfunction(key)].append(lagrat)
            

   def search(self, key):
        #key är nyckeln
            #Hamtar det objekt som finns lagrat med nyckeln "key" och returnerar det.
            #Om "key" inte finns ska det bli KeyError
        for hashNode in self.tabell[self.hashfunction(key)]:
            if hashNode.key == key:
                return hashNode.data
        else:
            raise KeyError

   def hashfunction(self, key):
    #key är nyckeln
    #Beräknar hashfunktionen för key""
    sträng =""
    for bokstav in key:
       sträng += str(ord(bokstav))
    return int(sträng) % self.size
   


       

       