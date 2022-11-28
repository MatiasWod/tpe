#include <_play.h>
#include <userstdlib.h>


#define STARTING_PLAYER1    3352
#define STARTING_PLAYER2    3432


static uint32_t currentPlayer1Pos = STARTING_PLAYER1; // cambiarlo
static uint32_t currentPlayer2Pos = STARTING_PLAYER2; //cambiarlo
static uint32_t playersPaintedPixels[AREA] = {0};//Array que nos sirve para guardar el recorrido de los jugadores

/**
 *         0 if no player crashed
 *         1 if Colision was caused by player 1
 *         2 if Colision was caused by player 2
 *         3 if both crashed
 */
static uint32_t crash = 0;
static uint32_t collisionState1= 0;
static uint32_t wrongKey = 0;
char lastLetter1 = 'd';
char lastLetter2 = 'j';

//80 * 12 = 960
//960 * 2 = 1920
//20 * 2 = 40
//1920 + 40 = 1960 -> primera pos player1
//1920 + 120 = 2040 -> primera pos player2

void play(unsigned int fd){
    int savefont=getFont();
    setFont(2);
    clearScreen();
    char letter[1] = {0};
    int flag=1;
    while(flag){
        letter [0]=read_char();
        wrongKey=0;
        if(letter[0] <= 0){
            drawMovement(lastLetter1,1);
            drawMovement(lastLetter2,2);
        }
        if(letter[0] > 0){
            if( letter[0]=='w' || letter[0]=='a' || letter[0]=='s' || letter[0]=='d'){
                drawMovement(letter[0],1);
                drawMovement(lastLetter2,2);
                if (!wrongKey)
                    lastLetter1 = letter[0];
            }
            else if( letter[0]=='j' || letter[0]=='k' || letter[0]=='l' || letter[0]=='i'){
                drawMovement(lastLetter1,1);
                drawMovement(letter[0],2);
                if (!wrongKey)
                    lastLetter2 = letter[0];
            }
            letter[0] = 0;

        }
        if(crash!=0){
            endGame();
            setFont(savefont);
            flag=0;
        }
        sleepMiliseconds(55);
    }
}

void endGame(){
    clearScreen();
    if(crash==1){
        char str[13] = "Gano el azul";
        sprint(STDOUT,str);
    }
    else if(crash==2){
        char str[13] = "Gano el rojo";
        sprint(STDOUT,str);
    }
    else {
        char str[7] = "Empate";
        sprint(STDOUT,str);
    }

    for( int i=0 ; i<AREA ; i++ ){
        playersPaintedPixels[i]=0;
    }
    currentPlayer1Pos = STARTING_PLAYER1;
    currentPlayer2Pos = STARTING_PLAYER2;
    lastLetter1 = 'd';
    lastLetter2 = 'j';
    crash = 0;
    collisionState1= 0;
    wrongKey=0;
    for (int i=HEIGHT;i<AREA;i++)
        paint(BLACK,i);
    sleepMiliseconds(1000);
    clearScreen();
    restartCursor();

}

void drawMovement(char c, int player){
    int firstColumn = 0;
    int lastColumn=0;
    if(player==1) {
        color_t color=RED;
        if ((lastLetter1=='a' && c=='d') || (lastLetter1=='w' && c=='s') || (lastLetter1=='s' && c=='w') || (lastLetter1=='d' && c=='a')){
            c=lastLetter1;
            wrongKey=1;
        }
        if( currentPlayer1Pos % (WIDTH) == 0)
            firstColumn = 1;
        if( currentPlayer1Pos % (WIDTH) == 127)
            lastColumn = 1;
        if(c=='d' || c=='D'){
            currentPlayer1Pos += 1;
            paint(color,currentPlayer1Pos);
        }
        else if(c=='a' || c=='A'){
            currentPlayer1Pos -= 1;
            paint(color,currentPlayer1Pos);
        }
        else if(c=='w' || c=='W'){
            currentPlayer1Pos -= 1 * WIDTH;
            paint(color,currentPlayer1Pos);
        }
        else if(c=='s' || c=='S'){
            currentPlayer1Pos += 1 * WIDTH;
            paint(color,currentPlayer1Pos);
        }
        if((( currentPlayer1Pos % (WIDTH) == 127) && firstColumn == 1 )|| ((currentPlayer1Pos % (WIDTH) == 0) && lastColumn == 1)){
            crash+=1;
        }
        collisionState1=checkCollision(1);
        playersPaintedPixels[currentPlayer1Pos] = 1;
    }
    else{
        color_t color=BLUE;
        if ((lastLetter2=='j' && c=='l') || (lastLetter2=='i' && c=='k') || (lastLetter2=='k' && c=='i') || (lastLetter2=='l' && c=='j')){
            c=lastLetter2;
            wrongKey=1;

        }
        if( currentPlayer2Pos % (WIDTH) == 0)
            firstColumn = 1;
        if( currentPlayer2Pos % (WIDTH) == 127)
            lastColumn = 1;
        if(c=='l' || c=='L'){
            currentPlayer2Pos += 1;
            paint(color,currentPlayer2Pos);
        }
        else if(c=='j' || c=='J'){
            currentPlayer2Pos -= 1;
            paint(color,currentPlayer2Pos);
        }
        else if(c=='i' || c=='I'){
            currentPlayer2Pos -= 1 * WIDTH;
            paint(color,currentPlayer2Pos);
        }
        else if(c=='k' || c=='K'){
            currentPlayer2Pos += 1 * WIDTH;
            paint(color,currentPlayer2Pos);
        }
        checkCollision(2);
        playersPaintedPixels[currentPlayer2Pos] = 1;
    }
}


int checkCollision(int player){
    if(player==1){
        if(playersPaintedPixels[currentPlayer1Pos]==1 || AREA<currentPlayer1Pos || 0==currentPlayer1Pos%WIDTH){
            crash += 1;
            return 1;
        }
    }
    if(player==2){
        if(playersPaintedPixels[currentPlayer2Pos]==1  || AREA<currentPlayer2Pos|| 0==currentPlayer2Pos%WIDTH){
            if(!collisionState1)
                drawMovement(lastLetter1,1);
            crash += 2;
            return 2;
        }
    }
    return 0;
}