
/*Adressen i minnet där VGA läser i från*/
#define VGA_BAS_ADRESS  0x08000000
/*storleken på skärmen*/
#define VGA_BREDD   320
#define VGA_HÖJD    240
#define SNAKE_PIXEL 16

/*Skapar en pekare till VGA-minnesområdet på adressen som tillhör rätt del i minnet. använder volatile för att när
 vi kompilerar koden ska inte kompilatorn ta bort kod vi vill ha. vi använder void för att vara tydliga med att 
 funktionen inte tar in några argument.*/
volatile unsigned char *skräm_skrivare(unsigned int offset){
    return (volatile unsigned char *)(VGA_BAS_ADRESS + offset);
}

/*Packar ihop varje färg så att alla färger tillsammans blir en byte. 
Är en char stor för att vi kommer retunera en byte.
Retunerar en byte som är gjord av våra MSB för R,G,& B ihopslagna genom bitvisor operatiorn.*/
unsigned char packa_färg( unsigned char röd_stor, unsigned char grön_stor, unsigned char blå_stor){
    unsigned char röd_liten = (röd_stor >> 5) & 0x07;
    unsigned char grön_liten = (grön_stor >> 5) & 0x07;
    unsigned char blå_liten = (blå_stor >> 6) & 0x03;
    return (unsigned char)((röd_liten << 5) | (grön_liten << 2) | blå_liten);
}

/*Färgar en pixel genom att ta in hela RGB-koden och färgar pixeln om den är inom vår skärm. Skapar ett index som 
representerar platsen i VGA-minnet som pixeln ligger på. Använder pekare för att skriva vår färg byte till den pixeln.*/
void färga_pixel( int x, int y, unsigned char röd_stor, unsigned char grön_stor, unsigned char blå_stor){ 
    if (x < 0 || x >= VGA_BREDD || y < 0 || y >= VGA_HÖJD ){
        return;
    }
    unsigned int index = (unsigned int)y * VGA_BREDD + (unsigned int)x;
    volatile unsigned char *skärm_pekare = skräm_skrivare(index);
    skärm_pekare = packa_färg (röd_stor, grön_stor, blå_stor);
}

void start_skärm(void){

}