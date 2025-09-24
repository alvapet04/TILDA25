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

# Skriven av ChatGPT
def linsok(låt_listan, testartist):
    for i, track in enumerate(låt_listan):
        if track.artistnamn == testartist:
            return i
    return 

# Skriven av oss, med inspo av ChatGPT

def main():
    låt_listan = läsa_in_fil("unique_tracks.txt")
    låt_listan = låt_listan[0:250000]

    n = len(låt_listan)
    print("Antal element =", n)

    sista = låt_listan[n-1]
    testartist = sista.artistnamn

    linjtid = timeit.timeit(stmt = lambda: linsok(låt_listan, testartist), number = 10000)
    print("Linjärsökningen tog", round(linjtid, 4) , "sekunder")

main()

# Har tidskomplexiteten O(n)
# Vi söker efter artist, som troligen dyker upp på flera ställen i filen
# Därför stämmer inte detta riktigt