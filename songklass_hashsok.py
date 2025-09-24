import timeit

#skriven av oss
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
def build_dict(lista):
    """
    Bygger en dict där nyckeln är artist och värdet är en lista med Track-objekt.
    """
    d = {}
    for track in lista:
        if track.artistnamn not in d:
            d[track.artistnamn] = []
        d[track.artistnamn].append(track)
    return d

def hashtabell_sok(ht, sokt_artist):
    """
    Sökning i hashtabellen.
    Returnerar objektet om det finns, annars None.
    """
    return ht.get(sokt_artist, None)

# Skriven av oss, med inspo av ChatGPT

def main():
    låt_listan = läsa_in_fil("unique_tracks.txt")
    låt_listan = låt_listan[0:1000000]
    n = len(låt_listan)
    print("Antal element =", n)

    sista = låt_listan[n-1]
    testartist = sista.artistnamn
    låt_listan=build_dict(låt_listan)

    linjtid = timeit.timeit(stmt = lambda: hashtabell_sok(låt_listan, testartist), number = 10000)
    print("Hashsökningen tog", round(linjtid, 4) , "sekunder")

main()

# Har tidskomplexiteten O(1)
# Den hittar nyckeln direkt