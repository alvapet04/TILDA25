from array import array

class Node():

    def __init__(self, x, next=None):
        self.value = x
        self.next = next


class LinkedQ():
    def __init__(self):
        self.first = None
        self.last = None
    
    def __str__(self):
        z = self.first
        slutkort = ""
        while z != None:
            slutkort += str(z.value) + " "
            z = z.next
        return slutkort
   
    def enqueue(self, x):
          if self.isEmpty():
            self.first = Node(x)
            self.last = self.first
          else:
            self.last.next = Node(x)
            self.last = self.last.next
    
    def dequeue(self):
          y = self.first.value 
          self.first = self.first.next 
          return y
    
    def isEmpty(self):
          if self.first == None:
                return True
          else:
                return False
          

import unittest
#from linkedQFile import LinkedQ

class TestQueue(unittest.TestCase):

    def test_isEmpty(self):
        #isEmpty ska returnera True för tom kö, False annars
        q = LinkedQ()
        self.assertTrue(q.isEmpty(), "isEmpty på tom kö")
        q.enqueue(17)
        self.assertFalse(q.isEmpty(), "isEmpty på icke-tom kö")

    def test_order(self):
        #Kontrollerar att kö-ordningen blir rätt
        q = LinkedQ()
        q.enqueue(1)
        q.enqueue(2)
        q.enqueue(3)
        self.assertEqual(q.dequeue(), 1)
        self.assertEqual(q.dequeue(), 2)
        self.assertEqual(q.dequeue(), 3)

if __name__ == "__main__":
    unittest.main()
