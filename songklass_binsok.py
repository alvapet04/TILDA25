import timeit


#Skriven av oss
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

#Skriven av ChatGPT

def binsok(lista, sokt_artist):
    """
    Binärsökning i en sorterad lista av Track-objekt (sorterad på artist).
    Returnerar index om hittad, annars -1.
    """
    low = 0
    high = len(lista) - 1    
    while low <= high:
        mid = (low + high) // 2
        if lista[mid].artistnamn == sokt_artist:
            return mid
        elif lista[mid].artistnamn < sokt_artist:
            low = mid + 1
        else:
            high = mid - 1
    return -1

# Skriven av oss, med inspo av ChatGPT

def main():
    låt_listan = läsa_in_fil("unique_tracks.txt")
    låt_listan = låt_listan[0:1000000]

    n = len(låt_listan)
    print("Antal element =", n)

    sista = låt_listan[n-1]
    testartist = sista.artistnamn

    linjtid = timeit.timeit(stmt = lambda: binsok(låt_listan, testartist), number = 10000)
    print("Binärsökningen tog", round(linjtid, 4) , "sekunder")

main()

# Har tidskomplexiteten O(logn)
# Vi söker efter artist, som troligen dyker upp på flera ställen i filen
# Därför stämmer inte detta riktigt