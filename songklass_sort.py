import timeit

# Skriven av oss
class Låtar():
    def __init__(self, trackid, låtid, artistnamn, låttitel):
        self.trackid = trackid
        self.låtid = låtid
        self.artistnamn = artistnamn
        self.låttitel = låttitel

    def __str__(self):
        return f"Låten heter {self.låttitel}, artisten är {self.artistnamn}."
    
    def __lt__(self, other):
        if self.artistnamn < other.artistnamn:
            return True
        else:
            return False

        
def läsa_in_fil(infilnamn):
    låt_listan=[]
    with open(infilnamn, "r", encoding="utf_8") as infil: 
        for rad in infil:
                rad=rad.split("<SEP>")
                trackid=rad[0]
                låtid=rad[1]
                artistnamn=rad[2]
                låttitel=rad[3].strip("\n")
                ny_låt=Låtar(trackid, låtid, artistnamn, låttitel)
                låt_listan.append(ny_låt)
    
    return låt_listan

# Från Lindas föreläsning 8

def bubbelsortera(data):
    n = len(data)
    bytt = True
    while bytt:
        bytt = False
        for i in range(n-1):
            if data[i] > data[i+1]:
                data[i], data[i+1] = data[i+1], data[i]
                bytt = True

# Från Lindas föreläsning 8

def quicksort(data):
    sista = len(data) - 1
    qsort(data, 0, sista)

def qsort(data, low, high):
    pivotindex = (low+high)//2
    # flytta pivot till kanten
    data[pivotindex], data[high] = data[high], data[pivotindex]  
    
    # damerna först med avseende på pivotdata
    pivotmid = partitionera(data, low-1, high, data[high]) 
    
    # flytta tillbaka pivot
    data[pivotmid], data[high] = data[high], data[pivotmid]       
    
    if pivotmid-low > 1:
        qsort(data, low, pivotmid-1)
    if high-pivotmid > 1:
        qsort(data, pivotmid+1, high)

def partitionera(data, v, h, pivot):
    while True:
        v = v + 1
        while data[v] < pivot:
            v = v + 1
        h = h - 1
        while h != 0 and data[h] > pivot:
            h = h - 1
        data[v], data[h] = data[h], data[v]
        if v >= h: 
            break
    data[v], data[h] = data[h], data[v]
    return v

# Skriven av oss, med inspo av ChatGPT

def main():
    filename = "unique_tracks.txt"    
    lista = läsa_in_fil(filename)
    lista = lista[0:100]
    n = len(lista)

    print("Antal element =", n)    

    lista1=lista.copy()
    lista2=lista.copy()

    bin_tid = timeit.timeit(stmt=lambda: quicksort(lista1),number=1  )
    print("Quiksort tog", round(bin_tid, 4), "sekunder")


    bin_tid = timeit.timeit(stmt=lambda: bubbelsortera(lista2),number=1  )
    print("Bubblesort tog", round(bin_tid, 4), "sekunder")

if __name__ == "__main__":
    main()

# Quicksort har tidskomplexiteten O(nlogn)
# Bubble sort har tidskomplexiteten O(n²)
# Bubblesorts tidskomplexitet kommer alltså att växa snabbare