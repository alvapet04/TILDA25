from array import array
#from arrayQFile import ArrayQ
from linkedQFile import LinkedQ

def kort_ordning():
    kort_array = LinkedQ()
    ordning  = input("vilken ordning vill du ka korten i?").split()
    for tal in ordning:
          tal = (tal)
          kort_array.enqueue(tal)
    return kort_array

def trollformel(kort_array):
        utlagda_kort = LinkedQ()
        while not kort_array.isEmpty():
            kort_array.enqueue(kort_array.dequeue())
            utlagda_kort.enqueue(kort_array.dequeue())
        print (utlagda_kort)



def main():
      dina_kort = kort_ordning()
      trollformel(dina_kort)
     

main()
      
      


          

