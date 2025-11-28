
/*Adressen i minnet där VGA läser i från*/
#define VGA_BAS_ADRESS  0x08000000
/*storleken på skärmen*/
#define VGA_BREDD   320
#define VGA_HÖJD    240
/*Storlekten på vår egen pixel*/
#define SNAKE_PIXEL 16
/*vår spelbrädda börjar vid dessa x och y*/
#define SPELBRÄDDAN_X   64
#define SPELBRÄDDAN_Y   44

/*Delay för att spakarna inte ska ge upprepade interupts*/
extern void delay(int);
extern void enable_interupt();
extern void disable_interupt();

/**/
int timoutcount = 0;

/*Minnesadresser för klockan*/
#define TIMERBAS    0x04000020
#define TIMERSTATUS ((volatile unsigned int*)(TIMERBAS + 0x0))
#define TIMERCONTROLL ((volatile unsigned int*)(TIMERBAS + 0x4))
#define TIMERPERIODL ((volatile unsigned int*)(TIMERBAS + 0x8))
#define TIMERPERIODH ((volatile unsigned int*)(TIMERBAS +0xC))

/*Minnesadresser för switchar*/
#define SWITCHBAS   0x04000010
#define SWITCHINTERUPT  ((volatile unsigned int*)(SWITCHBAS + 0x8))
#define SWITCHEDGECAPTURE ((volatile unsigned int*)(SWITCHBAS + 0xC))

/*Minnesadresser för knappen*/
#define BUTTONBAS   0x040000D0
#define BUTTONINTERUPT  ((volatile unsigned int*)(BUTTONBAS + 0x8))
#define BUTTONEDGECAPTURE ((volatile unsigned int*)(BUTTONBAS + 0xC))

/*Hårdkodade variabler som används till olika saker*/
int gameover;    

int poäng;

int riktning;

int äppletspositionx;

int äppletspositiony;

int maskenshuvudx;

int maskenshuvudy;

static int seed = 1234567890;

/*Arrayer för att hålla koll på maskens tidigare positioner*/
int maskenssvansx[144];

int maskenssvansy[144]; 

int masklängd;

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

/*tar in en sökning efter en mall till äpplet och färgar ett område utifrån en angiven position med 
hjälp av den färg som angivits. Så länge pixeln som ska färgas finns på skärmen. */
void rita_äpple( int x, int y, int index, unsigned char röd_stor, unsigned char grön_stor, unsigned char blå_stor, int skala) {
    int rad, kolumn;
    unsigned int linje;
    unsigned char färg = packa_färg(röd_stor, grön_stor, blå_stor);

    for (rad = 0; rad < 16; rad ++){
        linje = äpple16x16[index][rad];
        for (kolumn = 0; kolumn < 16; kolumn ++){
            if (linje & (0x8000 >> kolumn)) {
                for (int dy = 0; dy < skala; dy ++){
                    for ( int dx = 0; dx < skala; dx ++){
                        int px = x + kolumn * skala + dx;
                        int py = y + rad * skala +dy;
                        if (px >= 0 && px < VGA_BREDD && py >= 0 && py < VGA_HÖJD){
                        volatile unsigned char *skärm_pekare = skräm_skrivare(py * VGA_BREDD +px);
                        *skärm_pekare = färg;
                        }
                    }
                }
            }
        }
    }
}

/*Anger vilka färger som ska färga vilken mall till äpplet.*/
void färga_äpple( int x, int y, int skala){
    rita_äpple(x, y, 0, 0, 0, 0, skala);
    rita_äpple(x, y, 1, 142, 83, 31, skala);
    rita_äpple(x, y, 2, 109, 61, 18, skala);
    rita_äpple(x, y, 3, 66, 221, 48, skala);
    rita_äpple(x, y, 4, 85, 107, 47, skala);
    rita_äpple(x, y, 5, 234, 28, 28, skala);
    rita_äpple(x, y, 6, 252, 127, 106, skala);
}

/*Skapar en mall för dem olika bokstäder som används*/
const unsigned int bokstäver8x8[13][8]= {
    /*M*/
    {0x00, 0x66, 0xDD, 0xC9, 0xD5, 0xDD, 0xDD, 0x66},
    /*A*/
    {0x00, 0x1C, 0x32, 0x6D, 0x61, 0x6D, 0x6D, 0x36},
    /*S*/
    {0x00, 0x1E, 0x31, 0x6E, 0x32, 0x3D, 0x62, 0x3C},
    /*K*/
    {0x00, 0x36, 0x6D, 0x6A, 0x64, 0x6A, 0x6D, 0x36},
    /*E*/
    {0x00, 0x3E, 0x61, 0x6E, 0x62, 0x6E, 0x61, 0x3E},
    /*N*/
    {0x00, 0x36, 0x6D, 0x65, 0x69, 0x6D, 0x6D, 0x36}, 
    /*P*/
    {0x00, 0x3C, 0x62, 0x6D, 0x62, 0x6C, 0x68, 0x30},
    /*R*/
    {0x00, 0x3C, 0x62, 0x6D, 0x62, 0x6A, 0x6D, 0x36},
    /*B*/
    {0x00, 0x3C, 0x62, 0x6D, 0x62, 0x6D, 0x62, 0x3C},
    /*U*/
    {0x00, 0x36, 0x6D, 0x6D, 0x6D, 0x6D, 0x32, 0x1C}, 
    /*T*/
    {0x00, 0x7E, 0xC1, 0x76, 0x34, 0x34, 0x34, 0x18},
    /*O*/
    {0x00, 0x1C, 0x32, 0x6D, 0x6D, 0x6D, 0x32, 0x1C},
    /*_*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
};

/*tar in en sökning efter en mall till texten "masken" och färgar ett område utifrån en angiven position med 
hjälp av den färg som angivits. Så länge pixeln som ska färgas finns på skärmen. */
void rita_bokstäver( int x, int y, int index, unsigned char röd_stor, unsigned char grön_stor, unsigned char blå_stor, int skala) {
    int rad, kolumn;
    unsigned int linje;
    unsigned char färg = packa_färg(röd_stor, grön_stor, blå_stor);

    for (rad = 0; rad < 8; rad ++){
        linje = bokstäver8x8[index][rad];
        for (kolumn = 0; kolumn < 8; kolumn ++){
            if (linje & (0x80 >> kolumn)) {
                for (int dy = 0; dy < skala; dy ++){
                    for ( int dx = 0; dx < skala; dx ++){
                        int px = x + kolumn * skala + dx;
                        int py = y + rad * skala +dy;
                        if (px >= 0 && px < VGA_BREDD && py >= 0 && py < VGA_HÖJD){
                        volatile unsigned char *skärm_pekare = skräm_skrivare(py * VGA_BREDD +px);
                        *skärm_pekare = färg;
                        }
                    }
                }
            }
        }
    }
}

/*Anger vilka färger som ska färga vilka mallar*/
void färga_bokstäver_masken( int x, int y, int skala){
    /*Röd*/
    rita_bokstäver(x, y, 0, 255, 0, 0, skala);
    x += 8*skala;
    /*Orange*/
    rita_bokstäver(x, y, 1, 255, 128, 0, skala);
    x += 8*skala;
    /*Gul*/
    rita_bokstäver(x, y, 2, 255, 255, 0, skala);
    x += 8*skala;
    /*Grön*/
    rita_bokstäver(x, y, 3, 0, 204, 0, skala);
    x += 8*skala;
    /*Blå*/
    rita_bokstäver(x, y, 4, 0, 0, 255, skala);
    x += 8*skala;
    /*Lila*/
    rita_bokstäver(x, y, 5, 204, 0, 204, skala);
}

void färga_bokstäver_button( int x, int y, int skala){
    int text_array[21] = {6, 7, 4, 2, 2, 12, 8, 9, 10, 10, 11, 5, 12, 10, 11, 12, 2, 10, 1, 7, 10};
    for (int i = 0; i < sizeof(text_array) / sizeof(int); i ++){
        /*Svart*/
        rita_bokstäver(x, y, text_array[i], 255, 255, 255, skala);
        x += 8*skala;
        }
        
}

/*Skapar en mall för dem olika bokstäder som används*/
const unsigned int siffror8x8[10][8]= {
    /*0*/
    {0x00, 0x1C, 0x32, 0x6D, 0x6D, 0x6D, 0x32, 0x1C},
    /*1*/
    {0x00, 0x0C, 0x1A, 0x32, 0x1A, 0x1A, 0x31, 0x1E},
    /*2*/
    {0x00, 0x1C, 0x32, 0x6D, 0x3A, 0x36, 0x61, 0x3E},
    /*3*/
    {0x00, 0x1C, 0x32, 0x6D, 0x3A, 0x6D, 0x32, 0x1C},
    /*4*/
    {0x00, 0x0C, 0x1A, 0x32, 0x6A, 0x61, 0x3A, 0x0C},
    /*5*/
    {0x00, 0x3E, 0x61, 0x6E, 0x62, 0x3D, 0x62, 0x3C}, 
    /*6*/
    {0x00, 0x1C, 0x32, 0x6C, 0x62, 0x6D, 0x32, 0x1C},
    /*7*/
    {0x00, 0x3C, 0x62, 0x3D, 0x0D, 0x1A, 0x1A, 0x0C},
    /*8*/
    {0x00, 0x1C, 0x32, 0x6D, 0x32, 0x6D, 0x32, 0x1C},
    /*9*/
    {0x00, 0x1C, 0x32, 0x6D, 0x31, 0x1D, 0x1A, 0x0C}, 
};

/*tar in en sökning efter en mall till texten "masken" och färgar ett område utifrån en angiven position med 
hjälp av den färg som angivits. Så länge pixeln som ska färgas finns på skärmen. */
void rita_siffror( int x, int y, int index, unsigned char röd_stor, unsigned char grön_stor, unsigned char blå_stor, int skala) {
    int rad, kolumn;
    unsigned int linje;
    unsigned char färg = packa_färg(röd_stor, grön_stor, blå_stor);

    for (rad = 0; rad < 8; rad ++){
        linje = siffror8x8[index][rad];
        for (kolumn = 0; kolumn < 8; kolumn ++){
            if (linje & (0x80 >> kolumn)) {
                for (int dy = 0; dy < skala; dy ++){
                    for ( int dx = 0; dx < skala; dx ++){
                        int px = x + kolumn * skala + dx;
                        int py = y + rad * skala +dy;
                        if (px >= 0 && px < VGA_BREDD && py >= 0 && py < VGA_HÖJD){
                        volatile unsigned char *skärm_pekare = skräm_skrivare(py * VGA_BREDD +px);
                        *skärm_pekare = färg;
                        }
                    }
                }
            }
        }
    }
}

void färga_poäng(){
    int x, y;
    for ( y = 0; y < 40; y++){
        for(x = 232; x < 272 ; x++){
            unsigned char röd_stor = 96;
            unsigned char grön_stor = 198;
            unsigned char blå_stor = 27;
            färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
        }
    }
    if (poäng >= 100){
        rita_siffror(152, 4, 1, 0, 0, 0, 3);
    }
    if (poäng >=10){
        rita_siffror(200, 4, (poäng/10)%10, 0, 0, 0, 3);
    }
    rita_siffror(248, 4, poäng%10, 0, 0, 0, 3);       
}

/*Avlutar starsskärmskoden med en funktion som sätter ihop den*/
void startskärm(){
    int x, y;
    for ( y = 0; y < VGA_HÖJD; y++){
        for(x = 0; x < VGA_BREDD ; x++){
            unsigned char röd_stor = 96;
            unsigned char grön_stor = 198;
            unsigned char blå_stor = 27;
            färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
        }
    }
    färga_äpple(125, 40, 4);
    färga_bokstäver_masken(60,100, 4);
    färga_bokstäver_button(75, 150, 1);   
}

/*Ritar ut vår schakbräddes liknande spelplan med kant. vi har valt att färga jämna rutor med en färg och udda med en annan
för att kunna skapa mönstret.*/
void rita_spelbrädda(void){
    int x, y;
    for ( y = 40; y < VGA_HÖJD; y++){
        for(x = 60; x < VGA_BREDD - 60; x++){
            unsigned char röd_stor = 0;
            unsigned char grön_stor = 0;
            unsigned char blå_stor = 0;
            färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
        }
    }
    
    for (int i = 0; i < 6; i++){
        /*Färgar udda rader*/
        for ( y = SPELBRÄDDAN_Y + i * SNAKE_PIXEL * 2; y < SPELBRÄDDAN_Y + SNAKE_PIXEL*(1+i*2); y++){
            for (int j = 0; j < 6; j++){
                /*färgar udda kolumner */
                for ( x = SPELBRÄDDAN_X + j * SNAKE_PIXEL * 2; x < SPELBRÄDDAN_X + SNAKE_PIXEL*(1+j*2); x++){
                    unsigned char röd_stor = 155;
                    unsigned char grön_stor = 87;
                    unsigned char blå_stor = 13;
                    färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
                }
                /*färgar jämna kolumner*/
                for ( x = SPELBRÄDDAN_X + SNAKE_PIXEL *(1+j*2); x < SPELBRÄDDAN_X + SNAKE_PIXEL*(2+j*2); x++){
                    unsigned char röd_stor = 117;
                    unsigned char grön_stor = 66;
                    unsigned char blå_stor = 11;
                    färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
                }
            }
        }
        /*färgar jämna rader*/
        for ( y = SPELBRÄDDAN_Y + SNAKE_PIXEL *(1+i*2); y < SPELBRÄDDAN_Y + SNAKE_PIXEL*(2+i*2); y++){
            for (int j = 0; j < 6; j++){
                /*färgar udda kolumner*/
                for ( x = SPELBRÄDDAN_X + j * SNAKE_PIXEL * 2; x < SPELBRÄDDAN_X + SNAKE_PIXEL*(1+j*2); x++){
                    unsigned char röd_stor = 117;
                    unsigned char grön_stor = 66;
                    unsigned char blå_stor = 11;
                    färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
                }
                /*färgar jämna kolumner*/
                for ( x = SPELBRÄDDAN_X + SNAKE_PIXEL *(1+j*2); x < SPELBRÄDDAN_X + SNAKE_PIXEL*(2+j*2); x++){
                    unsigned char röd_stor = 155;
                    unsigned char grön_stor = 87;
                    unsigned char blå_stor = 13;
                    färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
                }
            }
        }
    }
}

/*Skapar en constant mall för äpplets olika färgers pixlars koordinater*/
const unsigned int maskhuvud16x16[2][16]= {
    {0x07E0,0x1FF8,0x3FFC,0x7FFE,0x7FFE,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000},
    {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFFFF}   
};

/*tar in en sökning efter en mall till äpplet och färgar ett område utifrån en angiven position med 
hjälp av den färg som angivits. Så länge pixeln som ska färgas finns på skärmen. */
void rita_maskhuvud( int x, int y, int index, unsigned char röd_stor, unsigned char grön_stor, unsigned char blå_stor) {
    int rad, kolumn;
    unsigned int linje;
    unsigned char färg = packa_färg(röd_stor, grön_stor, blå_stor);

    for (rad = 0; rad < 16; rad ++){
        linje = maskhuvud16x16[index][rad];
        for (kolumn = 0; kolumn < 16; kolumn ++){
            if (linje & (0x8000 >> kolumn)) {
                        switch (riktning)
                        {
                        /*Maskens huvud uppåt*/
                        case 1:{
                            int px = x + kolumn;
                            int py = y + rad;
                            if (px >= 0 && px < VGA_BREDD && py >= 0 && py < VGA_HÖJD){
                                volatile unsigned char *skärm_pekare = skräm_skrivare(py * VGA_BREDD +px);
                                *skärm_pekare = färg;
                                }
                            break;
                        }
                        /*Maskens huvud åt höger*/
                        case 2:{
                            int px = x + (SNAKE_PIXEL - 1 - rad);
                            int py = y  +  (SNAKE_PIXEL - 1 - kolumn);
                            if (px >= 0 && px < VGA_BREDD && py >= 0 && py < VGA_HÖJD){
                                volatile unsigned char *skärm_pekare = skräm_skrivare(py * VGA_BREDD +px);
                                *skärm_pekare = färg;
                                }
                            break;
                        }
                        /*Maskens huvud går nedåt*/
                        case 3:{
                            int px = x + (SNAKE_PIXEL - 1 - kolumn);
                            int py = y + (SNAKE_PIXEL - 1 - rad);
                            if (px >= 0 && px < VGA_BREDD && py >= 0 && py < VGA_HÖJD){
                                volatile unsigned char *skärm_pekare = skräm_skrivare(py * VGA_BREDD +px);
                                *skärm_pekare = färg;
                                }
                            break;
                        }
                        /*Maskenns huvud går år vänster*/
                        case 4:{
                            int px = x + rad;
                            int py = y + kolumn;
                            if (px >= 0 && px < VGA_BREDD && py >= 0 && py < VGA_HÖJD){
                                volatile unsigned char *skärm_pekare = skräm_skrivare(py * VGA_BREDD +px);
                                *skärm_pekare = färg;
                                }
                            break;
                        }
                        
                        default:
                            break;
                        }
                    }
                }
            }
        
        }


/*Anger vilka färger som ska färga vilken mall till maskens huvud.*/
void färga_maskhuvud( int x, int y){
    rita_maskhuvud(x, y, 0, 255, 163, 177);
    rita_maskhuvud(x, y, 1, 216, 106, 122);

}

/*färgar de olika delarna till en bit av maskens kropp så att den sedan blir randig.*/
/*kollar vilken riktning som masken har för att veta åt vilket håll maskens kropps bitar ska ligga åt*/
void rita_maskkropp(int maskkroppx, int maskkroppy){
    int x, y;
    /*mörkrosa*/
    for ( y= SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL); y < SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL) + SNAKE_PIXEL; y++){
        for(x = SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL); x < SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL) + SNAKE_PIXEL; x++){
            unsigned char röd_stor = 216;
            unsigned char grön_stor = 106;
            unsigned char blå_stor = 122;
            färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
        }
    }
    /*om den är upp eller ner så ska vi använda x och y i denna ordning*/
    if (riktning ==1|| riktning ==3){
        /*rosa*/
        for ( y = SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL) + 1; y < SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL) + SNAKE_PIXEL - 1; y++){
            for(x = SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL); x < SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL) + SNAKE_PIXEL; x++){
                unsigned char röd_stor = 255;
                unsigned char grön_stor = 163;
                unsigned char blå_stor = 177;
                färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
            }
        }       
    }
    /*om den ligger på sidan så ska vi byra plast på x och y*/
    else if (riktning==2|| riktning==4){
        /*rosa*/
        for ( y = SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL); y < SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL) + SNAKE_PIXEL; y++){
            for(x = SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL) + 1; x < SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL) + SNAKE_PIXEL -1 ; x++){
                unsigned char röd_stor = 255;
                unsigned char grön_stor = 163;
                unsigned char blå_stor = 177;
                färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
            }
        } 
        
    }  
}


/*vi måste rita tillbaka spelbräädan efteratt masken varit där, kollar om det är en jämn eller udda ruta*/
void rita_tillbaka_spelbrädda(int maskkroppx, int maskkroppy){
    int x, y;
    if ( (maskkroppx + maskkroppy)%2 == 0){
        for ( y = SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL); y < SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL) + SNAKE_PIXEL; y++){
            for(x = SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL); x < SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL) + SNAKE_PIXEL; x++){
                unsigned char röd_stor = 155;
                unsigned char grön_stor = 87;
                unsigned char blå_stor = 13;
                färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
            }
        }
    }
    else if ( (maskkroppx + maskkroppy)%2 == 1){
        for ( y = SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL); y < SPELBRÄDDAN_Y + (maskkroppy*SNAKE_PIXEL) + SNAKE_PIXEL; y++){
            for(x = SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL); x < SPELBRÄDDAN_X + (maskkroppx*SNAKE_PIXEL) + SNAKE_PIXEL; x++){
                unsigned char röd_stor = 117;
                unsigned char grön_stor = 66;
                unsigned char blå_stor = 11;
                färga_pixel(x, y, röd_stor, grön_stor, blå_stor);
            }
        }
    } 
}

/*sätter ihop maskens funktioner*/
void mask(){
    rita_maskkropp(maskenssvansx[0], maskenssvansy[0]);
    rita_tillbaka_spelbrädda( maskenssvansx[masklängd], maskenssvansy[masklängd] );
    färga_maskhuvud(SPELBRÄDDAN_X + (maskenshuvudx*SNAKE_PIXEL), SPELBRÄDDAN_Y + (maskenshuvudy*SNAKE_PIXEL));
}

unsigned long long int slumpar_tal(){
    seed = (5005 *seed + 181 );
    return seed %12;
}

void slumpar_äpple(){
    for ( int försök = 0; försök < 1000; försök++){
        äppletspositionx = slumpar_tal()%12;
        äppletspositiony = slumpar_tal()%12;
        if (äppletspositionx == maskenshuvudx && äppletspositiony == maskenshuvudy){
        continue;
        }
        int krock = 0;
        for ( int i = 0; i < masklängd; i++ ){
        if (äppletspositionx == maskenssvansx[i] && äppletspositiony == maskenssvansy[i]){
            krock = 1;
            break;;
            }
        }
        if (!krock) return;
    }   
}

/**/
void svanslogik(){
    int tidigarex = maskenssvansx[0];
    int tidigarey = maskenssvansy[0];
    int ännutidigarex, ännutidigarey;
    maskenssvansx[0] = maskenshuvudx;
    maskenssvansy[0] = maskenshuvudy;
    for ( int i =1; i<masklängd; i++ ){
        ännutidigarex = maskenssvansx[i];
        ännutidigarey = maskenssvansy[i];
        maskenssvansx[i] = tidigarex;
        maskenssvansy[i] = tidigarey;
        tidigarex = ännutidigarex;
        tidigarey = ännutidigarey;
    }
}

/**/
void handle_interrupt(unsigned cause) {
    /*Om anledningen till interupten är klockan så förflyttar sig masken i spelet*/
    print("IRQ: %x\n", cause);

    if (cause == 0x10){
        *TIMERSTATUS = 1;
        if (timoutcount%10 == 0){
            switch (riktning)
            {
            /*Maskens huvud gå uppåt på spelplanen*/
            case 1:
                maskenshuvudy --;
                break;
            /*Maskens huvud gå åt höger på spelplanen*/
            case 2:
                maskenshuvudx ++;
                break;
            /*Maskens huvud gå nedåt på spelplanen*/
            case 3:
                maskenshuvudy ++;
                break;
            /*Maskens huvud gå åt vänster på spelplanen*/
            case 4:
                maskenshuvudx --;
                break;
            
            default:
                break;
            }
            mask();
            rita_tillbaka_spelbrädda( maskenssvansx[masklängd - 1], maskenssvansy[masklängd - 1] );
            if (maskenshuvudx < 0 || maskenshuvudx>= 12 || maskenshuvudy < 0 || maskenshuvudy >= 12){
                gameover = 1;
                *TIMERCONTROLL = 0b0;
                startskärm();

            }

            for ( int i = 0; i < masklängd; i++){
                if ( maskenssvansx[i] == maskenshuvudx && maskenssvansy[i] == maskenshuvudy){
                    gameover = 1;
                    *TIMERCONTROLL = 0b0;
                    startskärm();
                    }
            }

            if ((maskenshuvudx == äppletspositionx) && (maskenshuvudy == äppletspositiony)){
                poäng++;
                färga_poäng();
                masklängd++;
                slumpar_äpple();
                färga_äpple(äppletspositionx*SNAKE_PIXEL + SPELBRÄDDAN_X, äppletspositiony*SNAKE_PIXEL + SPELBRÄDDAN_Y, 1);
            }
            svanslogik();
            
            
        }
        timoutcount++;
    }
    if (cause == 0x11){
        switch (*SWITCHEDGECAPTURE)
        {
            /*Gå höger*/
        case 0b1:{
            if (riktning != 4){
                riktning = 2;
            }
            break;
        }
            /*Gå vänster*/
        case 0b10:{
            if (riktning != 2){
                riktning = 4;
            }
            break;
        }
            /*Gå ner*/
        case 0b100:{
            if (riktning != 1){
                riktning = 3;
            }
            break;
        }
            /*Gå upp*/
        case 0b1000:{
            if (riktning != 3){
                riktning = 1;
            }
            break;
        }
        default:
            break;
        }
    
        delay(50);
        *SWITCHEDGECAPTURE = 0xF;
    }

    if (cause == 0x12){
        *TIMERSTATUS = 1;
        *TIMERCONTROLL = 0b111;
        *SWITCHINTERUPT = 0xF;
        *BUTTONEDGECAPTURE = 1;
        masklängd = 2;
        int x = 6;
        int y = 5;
        riktning = 4; 
        maskenshuvudx = x;
        maskenshuvudy = y;
        for (int i = 0; i <= masklängd; i++) {
            maskenssvansx[i] = maskenshuvudx;
            maskenssvansy[i] = maskenshuvudy;
        }
        gameover = 0;
        poäng = 0;
        rita_spelbrädda();
        färga_äpple(272, 0, 3);
        mask();
        slumpar_äpple();
        färga_äpple((äppletspositionx*SNAKE_PIXEL) + SPELBRÄDDAN_X, (äppletspositiony*SNAKE_PIXEL) + SPELBRÄDDAN_Y, 1);
        //enable_interupt();
    }
}

int main (void){
    enable_interupt();
    startskärm();
    *TIMERPERIODL = 2999999 & 0xFFFF;
    *TIMERPERIODH = (2999999 >> 16) & 0xFFFF;
    *BUTTONINTERUPT = 0x01;
    


    return 0;
   
}