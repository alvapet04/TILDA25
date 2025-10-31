
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
    *skärm_pekare = packa_färg (röd_stor, grön_stor, blå_stor);
}

void start_skärm(void){

}

/*Skapar en constant mall för äpplets olika färgers pixlars koordinater*/
const unsigned int äpple16x16[7][16]= {
    {0x0000,0x0200,0x0500,0x05F0,0x0D08,0x1304,0x20C4,0x20C3,0x2008,0x2008,0x2008,0x1010,0x0920,0x06C0,0x0000,0x0000},
    {0x0000,0x0000,0x0200,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000},
    {0x0000,0x0000,0x0000,0x0200,0x0200,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000},
    {0x0000,0x0000,0x0000,0x0000,0x00F0,0x0030,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000},
    {0x0000,0x0000,0x0000,0x0000,0x0000,0x00C8,0x0038,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000},
    {0x0000,0x0000,0x0000,0x0000,0x0000,0x0C00,0x1800,0x10C0,0x11F0,0x1FF0,0x1FF0,0x0FE0,0x06C0,0x0000,0x0000,0x0000},
    {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0700,0x0F00,0x0E00,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}
};


void rita_äpple( int y, int x, int index, unsigned char röd_stor, unsigned char grön_stor, unsigned char blå_stor) {
    int rad, kolumn;
    unsigned int linje;
    unsigned char färg = packa_färg(röd_stor, grön_stor, blå_stor);

    for (rad = 0; rad < 16; rad ++){
        linje = äpple16x16[index][rad];
        for (kolumn = 0; kolumn < 16; kolumn ++){
            if (linje & (0x8000 >> kolumn)) {
                int px = x + kolumn;
                int py = y + rad;
                if (px >= 0 && px < VGA_BREDD && py >= 0 && py < VGA_HÖJD){
                    volatile unsigned char *skärm_pekare = skräm_skrivare(py * VGA_BREDD +px);
                    *skärm_pekare = färg;
                }
            }
        }
    }
}

void färga_äpple( int x, int y){
    rita_äpple(x, y, 0, 0, 0, 0);
    rita_äpple(x, y, 1, 142, 83, 31);
    rita_äpple(x, y, 2, 109, 61, 18);
    rita_äpple(x, y, 3, 66, 221, 48);
    rita_äpple(x, y, 4, 85, 107, 47);
    rita_äpple(x, y, 5, 234, 28, 28);
    rita_äpple(x, y, 6, 252, 127, 106);
}

void handle_interrupt(void) {
    // Hantera avbrott här
}

int main (void){
    färga_äpple(40, 64);
    return 0;
}