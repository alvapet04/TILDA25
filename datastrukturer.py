from array import array
#from arrayQFile import ArrayQ
from linkedQFile import LinkedQ

# Detta är en funktion som som skapar en länkad lista samt lägger till alla element från en input i den länkade listan. 
# Den retunerar den länkade listan.
def kort_ordning():
    kort_array = LinkedQ()
    ordning  = input().split()
    for tal in ordning:
          tal = (tal)
          kort_array.enqueue(tal)
    return kort_array

# Detta är en funktion som tar in den länkade listan och genomför algoritmen.
# Sedan skriver den ut ordningen på hur korten läggs ut.
def trollformel(kort_array):
        utlagda_kort = LinkedQ()
        while not kort_array.isEmpty():
            kort_array.enqueue(kort_array.dequeue())
            utlagda_kort.enqueue(kort_array.dequeue())
        print (utlagda_kort)


# Detta är main funktionen som anropar de andra två funktionerna.
def main():
      dina_kort = kort_ordning()
      trollformel(dina_kort)
     

main()
      
      


          

