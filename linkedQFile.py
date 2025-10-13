from array import array

# Detta är en klass som är mallen för alla objekt i den länkade listan.
class Node():

    def __init__(self, x, next=None):
        self.value = x
        self.next = next

# Denna klass skapar de länkade listorna.
class LinkedQ():

# Init-metoden sätter attributen till none
    def __init__(self):
        self.first = None
        self.last = None

# Str-metoden gör så att objektet den länkade listan kan skrivas ut.    
    def __str__(self):
        z = self.first
        slutkort = ""
        while z != None:
            slutkort += str(z.value) + ""
            z = z.next
        return slutkort

# metoden enqueue lägger till en ny nod i den länkade listan även om listan är tom (som det bestämdes att de skulle va i init-metoden)
    def enqueue(self, x):
          if self.isEmpty():
            self.first = Node(x)
            self.last = self.first
          else:
            self.last.next = Node(x)
            self.last = self.last.next

# metoden dequeue tar ut det första elementet i den länkade listan och retunerar det ut plockade elementet.
    def dequeue(self):
          y = self.first.value 
          self.first = self.first.next 
          return y

# metoden isEmpty kollar om en länkad lista är tom.
    def isEmpty(self):
          if self.first == None:
                return True
          else:
                return False
          
    def peek(self):
        if not self.isEmpty():
            return self.first.value
        else:
            return ""
          


