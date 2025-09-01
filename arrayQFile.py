from array import array

class ArrayQ():

    def __init__(self):
            self.__magi = array('b',[]) 
    
    def __str__(self):
            korten = ""
            for tal in self.__magi:
                korten += (f"{tal} ")
            return korten
        
    
    def enqueue(self, x):
          self.__magi.append(x)
    
    
    def dequeue(self):
          return self.__magi.pop(0)
    
    def isEmpty(self):
          trollstav, kort = self.__magi.buffer_info()
          if kort <= 0:
                return True
          else:
                return False
          
def main():
      print ("ArrayQ")

if __name__ == "__main__":
      main()