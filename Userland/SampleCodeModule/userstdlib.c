#include <stdint.h>
#include <userstdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define IS_DIGIT(x) (((x) >= '0' && (x) <= '9'))

extern int  sys_write(uint64_t fd, char * buffer, uint64_t size);
extern int  sys_read(uint64_t fd, char * buffer, uint64_t size);
extern void sys_time(char * buffer);
extern void sys_clear();
extern void sys_restartCursor();
extern void sys_uniqueWindow();
extern int  sys_printmem(uint64_t * mem_address);
extern void sys_date(char * buffer);
extern void sys_infoReg();
extern void sys_paint(color_t color, uint32_t position);
extern int sys_seconds_elapsed();
extern int sys_miliseconds_elapsed();
extern int sys_set_font(int fontNumber);
extern int sys_get_font();

void paint(color_t color, uint32_t position){
    sys_paint(color, position);
}

void infoReg(){
  sys_infoReg();
}



void uniqueWindow(){
  sys_uniqueWindow();
}

int _strlen(const char * str){
    int i=0;
    while( *(str + (i++)) );
    return i-1;
}

int strcmp(char * s1, char * s2) {
    int i;
    for (i = 0; s1[i] && s1[i] == s2[i]; i++);
    return s1[i] - s2[i];
}

int sprint(uint8_t fd, char * str){
  return sys_write(fd, str, _strlen(str));
}

int put_char(uint8_t fd, char c){
  return sys_write(fd, &c, 1);
}

void get_time(char * buffer){
  sys_time(buffer);
}

void get_date(char * buffer){
  sys_date(buffer);
}

int get_char(){
    char c;
    int returnval;
    do{
      returnval = sys_read(STDIN, &c, 1);
    }while(returnval!=1);

	  return c;
}

int read_char(){
    char c;
    sys_read(STDIN, &c, 1);
    return c;
}

void clearScreen(){
  sys_clear();
}

void restartCursor(){
  sys_restartCursor();
}

int printMem(uint64_t * value){
  return sys_printmem(value);
}

int atoi(char *str){
    int answer = 0;
    int neg = 1;
    if(*str == '-'){
        neg = -1;
        str++;
    }
    while(*str){
        if(!IS_DIGIT(*str)){
            return 0;
        }
        answer = 10*answer + (*str - '0');
        str++;
    }
    return neg * answer;
}

char * my_strcpy( char * destination, char * source){
	return my_strncpy(destination, source, _strlen(source) + 1);
}

char * my_strncpy( char * destination, char * source, int size){
	int i = 0;
	char * ret = destination;
	while(*source && i < size){
		*(destination++) = *(source++);
		i++;
	}
	*destination = '\0';
	return ret;
}

void my_printf(const char * frmt, ...){
  // Module 1: Initializing Myprintf's arguments using stdarg.h
  va_list arg;   // declares a variable which we use to manipulating the argument list contaning variable arugments
  va_start(arg, frmt);   //initialize arg with function's last fixed argument, i.e. format

  const char *aux;

  uint64_t i;
  unsigned u;
  char *s;

  for(aux=frmt; *aux != '\0'; aux++){
    while(*aux != '%'){
      if(*aux == '\0'){
        va_end(arg);
        return;
      }
      put_char(STDOUT, *aux);
      aux++;
    }
    aux++;

    // Module 2: Fetching and executing arguments
    // va_arg() fetches the next argument from the argument list, where the 2nd parameter is the data type expected
    // ONLY accept char*, unsigned int, int or double
    switch(*aux){
      case 'c' :  i = va_arg(arg, int);  // Fetch char argument
                  put_char(STDOUT, i);
                  break;
      case 'd' :  i = va_arg(arg, int);   // Fetch Decimal/Integer argument
                  if(i < 0){
                    i = -i;
                    put_char(STDOUT, '-');
                  }
                  sprint(1, convert(i,10));
                  break;
      case 'o':   i = va_arg(arg, unsigned int);   // Fetch Octal representation
                  sprint(1, convert(i,8));
                  break;
      case 's':   s = va_arg(arg, char *);   // Fetch string
                  sprint(1, s);
                  break;
      case 'u':   u = va_arg(arg, unsigned int);   // Fetch Unsigned decimal integer
                  sprint(1, convert(u,10));
                  break;
      case 'x':   u = va_arg(arg, unsigned int);   // Fetch Hexadecimal representation
                  sprint(1, convert(u,16));
                  break;
      case '%':   put_char(STDOUT, '%');
                  break;
    }
  }
  //Module 3: Closing argument list to necessary clean-up
  va_end(arg);
}

char *convert(unsigned int num, int base){
  static char Representation[] = "0123456789ABCDEF";
  static char buff[33];
  char *ptr;

  ptr = &buff[sizeof(buff)-1];
  *ptr = '\0';

  do{
    *--ptr = Representation[num % base];
    num /= base;
  }while(num != 0);

  return(ptr);
}


int seconds_elapsed(){
  return sys_seconds_elapsed();
 
}
int miliseconds_elapsed(){
  return sys_miliseconds_elapsed();
}

void sleepMiliseconds(int miliseconds){
    int miliseconds_e = miliseconds_elapsed();
    for (int i = miliseconds_elapsed(); i < miliseconds + miliseconds_e ; i = miliseconds_elapsed());
    
}

void sleep(int seconds){
   int seconds_e = seconds_elapsed();
    for (int i = seconds_elapsed(); i < seconds_e + seconds; i = seconds_elapsed());
}

void setFont(int fontNumber){
  sys_set_font(fontNumber);
}

int getFont(){
  return sys_get_font();
}