#include <graphicMode.h>
#include <smallFont.h>
#include <normalFont.h>
#include <bigFont.h>

// cw = current window
window cw;

color_t * font_color = &WHITE;  // default font_color
color_t * bg_color = &BLACK;    // default bg_color

static char buffer[64] = { '0' };
static const struct vbe_mode_info_structure * graphicModeInfo = (struct vbe_mode_info_structure *) 0x5C00;
int CHAR_HEIGHT = 16;
int CHAR_WIDTH = 9;
int global_font = 2;

static void getNextPosition();
static void checkSpace();
static void scrollUp();

static uint8_t * getPixelAddress(int i, int j) {
    return (uint8_t *) (graphicModeInfo->framebuffer+3*(graphicModeInfo->width*i+j));
}

static void drawPixel(int i, int j, color_t * color){
    uint8_t * pixel = getPixelAddress(i,j);
    pixel[0] = color->B;
    pixel[1] = color->G;
    pixel[2] = color->R;
  }

// Default window
void initUniqueWindow(){
    cw.current_i = 0;
    cw.current_j = 0;
    cw.start_i = 0;
    cw.start_j = 0;
    cw.width = graphicModeInfo->width / CHAR_WIDTH;
    cw.height = graphicModeInfo->height / CHAR_HEIGHT;
    clearAll();
}

void printCharFormatWithoutScroll(uint8_t c, color_t * charColor, color_t * bgColor){

    // Backspace
    if(c == '\b'){
        if(cw.current_j == 0){
            cw.current_i -= 1;
            cw.current_j = cw.width-1;
            printCharFormatWithoutScroll(c, charColor, bgColor);
            cw.current_i -= 1;
            cw.current_j = cw.width-1;
        } else {
            cw.current_j = (cw.current_j-1) % cw.width;
            printCharFormatWithoutScroll(c, charColor, bgColor);
            cw.current_j = (cw.current_j-1) % cw.width;
        }
        return;
    }

    // Newline
    if(c == '\n'){
        newLine();
        return;
    }

        uint8_t * character;
    if(global_font == 1){
        character = getCharMappingSmallFont(c);
        CHAR_HEIGHT = 11;
        CHAR_WIDTH = 8;
        cw.width = graphicModeInfo->width / CHAR_WIDTH;
        cw.height = graphicModeInfo->height / CHAR_HEIGHT;
    }
    if(global_font == 2){
        character = getCharMappingNormalFont(c);
        CHAR_HEIGHT = 14;
        CHAR_WIDTH = 8;
        cw.width = graphicModeInfo->width / CHAR_WIDTH;
        cw.height = graphicModeInfo->height / CHAR_HEIGHT;
    }
    if(global_font == 3){
        character = getCharMappingBigFont(c);
        CHAR_HEIGHT = 16;
        CHAR_WIDTH = 9;
        cw.width = graphicModeInfo->width / CHAR_WIDTH;
        cw.height = graphicModeInfo->height / CHAR_HEIGHT;
    }
    // Upper left pixel of the current character
    uint16_t write_i = (cw.start_i + cw.current_i) * CHAR_HEIGHT;
    uint16_t write_j = (cw.start_j + cw.current_j) * CHAR_WIDTH;

    uint8_t mask;

    for(int i=0; i < CHAR_HEIGHT; ++i){
        for(int j=0; j < CHAR_WIDTH; ++j){
            mask = 1 << (CHAR_WIDTH - j - 1);
            if(character[i] & mask){
                drawPixel(write_i + i, write_j + j, charColor);
            }
            else{
                drawPixel(write_i + i, write_j + j, bgColor);
            }
        }
    }
    getNextPosition();
}

void printCharFormat(uint8_t c, color_t * charColor, color_t * bgColor){
   
    // Backspace
    if(c == '\b'){
      if(cw.current_j == 0){
          cw.current_i -= 1;
          cw.current_j = cw.width-1;
          printCharFormat(' ', charColor, bgColor);
          cw.current_i -= 1;
          cw.current_j = cw.width-1;
      } else {
        cw.current_j = (cw.current_j-1) % cw.width;
        printCharFormat(' ', charColor, bgColor);
        cw.current_j = (cw.current_j-1) % cw.width;
      }
      return;
  }
  
    checkSpace();

    // Newline
    if(c == '\n'){
        newLine();
        return;
    }
    uint8_t * character;
    if(global_font == 1){
        character = getCharMappingSmallFont(c);
        CHAR_HEIGHT = 11;
        CHAR_WIDTH = 8;
        cw.width = graphicModeInfo->width / CHAR_WIDTH;
        cw.height = graphicModeInfo->height / CHAR_HEIGHT;
    }
    if(global_font == 2){
        character = getCharMappingNormalFont(c);
        CHAR_HEIGHT = 14;
        CHAR_WIDTH = 8;
        cw.width = graphicModeInfo->width / CHAR_WIDTH;
        cw.height = graphicModeInfo->height / CHAR_HEIGHT;
    }
    if(global_font == 3){
        character = getCharMappingBigFont(c);
        CHAR_HEIGHT = 16;
        CHAR_WIDTH = 9;
        cw.width = graphicModeInfo->width / CHAR_WIDTH;
        cw.height = graphicModeInfo->height / CHAR_HEIGHT;
    }
    // Upper left pixel of the current character
    uint16_t write_i = (cw.start_i + cw.current_i) * CHAR_HEIGHT;
    uint16_t write_j = (cw.start_j + cw.current_j) * CHAR_WIDTH;

    uint8_t mask;

    for(int i=0; i < CHAR_HEIGHT; ++i){
        for(int j=0; j < CHAR_WIDTH; ++j){
            mask = 1 << (CHAR_WIDTH - j - 1);
            if(character[i] & mask){
                drawPixel(write_i + i, write_j + j, charColor);
            }
            else{
                drawPixel(write_i + i, write_j + j, bgColor);
            }
        }
    }
    getNextPosition();
}

static void getNextPosition(){
    cw.current_i += ((cw.current_j + 1) == cw.width ) ? 1:0;
    cw.current_j = (cw.current_j + 1) % cw.width;
}

static void checkSpace(){
    if(cw.current_i == cw.height){
        scrollUp();
    }
}

static void scrollUp(){
    for(int i=1; i < cw.height * CHAR_HEIGHT; ++i){

        uint8_t * start = getPixelAddress(cw.start_i + i, cw.start_j);
        uint8_t * next = getPixelAddress(cw.start_i + CHAR_HEIGHT + i, cw.start_j);

        for(int j=0; j < cw.width * CHAR_WIDTH * 3 ; ++j){
            start[j] = next[j];
        }
    }
    cw.current_i -= 1;
}

void printChar(uint8_t c){
    printCharFormat(c, &WHITE, &BLACK);
}


void print(const char * string){
    for (int i=0; string[i] != 0; ++i){
       printChar(string[i]);
    }
}

void newLine(){
    cw.current_j = 0;
    cw.current_i += 1;
}

void restartCursor(){
    cw.current_i = 0;
    cw.current_j = 0;
}

void clearAll(){
    cw.current_i = 0;
    cw.current_j = 0;
    for(int i=0; i < (cw.height) ; ++i ){
        for(int j=0; j < (cw.width) ; ++j){
            printCharFormat(' ', &WHITE, &BLACK);
        }
    }
    cw.current_i = 0;
    cw.current_j = 0;
}


void printDec(uint64_t value){
	  printBase(value, 10);
}

void printHex(uint64_t value){
	  printBase(value, 16);
}

void printBin(uint64_t value){
	  printBase(value, 2);
}

void printBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    print(buffer);
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
    char *p = buffer;
    char *p1;
    char *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do{
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }
    while (value /= base);

    // Terminate string in buffer.
    *p = 0;
    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2){
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return digits;
}


void printRegisterFormat(uint64_t reg){
	
    uint64_t aux = reg;
    uint64_t count =  16;
    
    while(aux){
        aux = aux >> 4;
        --count;
    }

    for(int i=0; i < count ;i++){
       printChar('0');
    }
  
    if(reg){
       printHex(reg);
    }
}


void paintPixel(color_t color, uint32_t position){
    char c=' ';
    cw.current_i = position / cw.width;
    cw.current_j = position % cw.width;
    printCharFormatWithoutScroll(c , &BLACK , &color );
}