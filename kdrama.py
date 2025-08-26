import csv

class Kdrama():
    def __init__(self, name, rating, actors, viewship, genre, director, writer, year, episodes, network):
        self.name = name
        self.rating = float(rating)
        self.actors = actors
        self.viewship = float(viewship)
        self.genre = genre
        self.director = director
        self.writer = writer
        self.year = int(year)
        self.episodes = int(episodes)
        self.network = network

    def __str__(self):
        return f"Filmen heter {self.name}, skådespelarna är {self.actors}, filmen släpptes {self.year}."
    
    def __lt__(self, other):
        if self.rating < other.rating:
            return True
        else:
            return False
        
    def no_of_episodes(self):
        if self.episodes > 20:
            return True
        else:
            return False

    def finns_på_samma_nätverk(self, kdrama_listan):
        print(f"{self.name} finns på samma nätverk som")
        for film in kdrama_listan:
            if self.name != film.name:
                if self.network == film.network:
                    print(film.name)

def läsa_in_fil(infilnamn):
    kdrama_listan=[]
    with open(infilnamn, "r", encoding="utf_8") as infil: 
        csvfile = csv.reader(infil, delimiter = ",", quotechar='"')
        next(csvfile)
        for rad in csvfile:
                name=rad[0]
                rating=rad[1]
                actors=rad[2]
                viewship=rad[3]
                genre=rad[4]
                director=rad[5]
                writer=rad[6]
                year=rad[7]
                episodes=rad[8]
                network=rad[9]
                ny_kdrama=Kdrama(name, rating, actors, viewship, genre, director, writer, year, episodes, network)
                kdrama_listan.append(ny_kdrama)
    return kdrama_listan

def popularitetsrankning(kdrama_listan):
    maximum=0
    for rad in kdrama_listan:
        if rad.rating > maximum:
            maximum=rad.rating
            populärastfilm= rad.name
    print(f"Det populäraste kdramat är {populärastfilm}.")

def tjugoepisoder(kdrama_listan):
    tjugoepisoderlistan=[]
    for rad in kdrama_listan:
        if rad.no_of_episodes():
            tjugoepisoderlistan.append(rad)
    print(f"{len(tjugoepisoderlistan)} av {len(kdrama_listan)} har fler än 20 episoder")

def söka_efter_år(kdrama_listan):
    sökt_år=int(input("Vilket år vill du se en film från?"))
    print(f"Dessa filmer släpptes {sökt_år}:")
    for rad in kdrama_listan:
        if rad.year == sökt_år:
            print(rad)

def main():
    kdrama_listan=läsa_in_fil("kdrama.csv")
    for serie in kdrama_listan:
        print(serie)

    popularitetsrankning(kdrama_listan)
    tjugoepisoder(kdrama_listan)
    kdrama_listan[0].finns_på_samma_nätverk(kdrama_listan)
    söka_efter_år(kdrama_listan)

main()