import csv


class DictHash ():
    def __init__(self, ordlista = []):
        self.ordbok = dict(ordlista)

    def store (self, nyckel, data):
        self.ordbok[nyckel] = data
    
    def search (self, nyckel):
        if nyckel in self.ordbok:
            data = self.ordbok[nyckel]
            return data
        return
    def __contains__(self, nyckel):
        if nyckel in self.ordbok:
            return True
        else: 
            return False
        
def läsa_in_fil(infilnamn):
    kdrama_hash = DictHash()
    with open(infilnamn, "r", encoding="utf_8") as infil: 
        csvfile = csv.reader(infil, delimiter = ",", quotechar='"')
        next(csvfile)
        for rad in csvfile:
                name=rad[0]
                rating=rad[1]
                kdrama_hash.store(name, rating)         
    return kdrama_hash 


def main ():
    kdrama_hash = läsa_in_fil("Lab 7/kdrama.csv")
    print(kdrama_hash.search("Legend of the Blue Sea"))
    print(kdrama_hash.search("Harry Potter"))
    nyckel = "Legend of the Blue Sea"
    if nyckel in kdrama_hash:
        print("hej")

main()