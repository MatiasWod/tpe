#include <shell.h>
#include <userstdlib.h>
#include <_play.h>


typedef void (*void_function)(void);

typedef struct{
  void_function action;
  char * name;
}command;

// usertdlib.asm
extern void divideByZero(void);
extern void invalidOp(void);

static int execute_command(char * command);
static void help(void);
static void time(void);
static void print_mem(void);

static void setSmallFont(void);
static void setNormalFont(void);
static void setBigFont(void);

static command valid_commands[] = {{&help,"help"}, {&time,"time"}, {&play, "play"},
         {&divideByZero, "dividezero"}, {&invalidOp, "invalidop"}, {&print_mem, "printmem"}, {&infoReg, "inforeg"}, {&setSmallFont,"setsmallfont"}, {&setNormalFont,"setnormalfont"},
         {&setBigFont,"setbigfont"}, {0,0}};

void welcome_message(void){
  my_printf("Bienvenido a Userland \n");
  help();
}

void wait_command(void){

  char c = 0;
	char command[128];
	int i = 0;

  // Read the command until the user presses enter
  while((c=get_char()) != '\n'){
    // Just delete what the user has written
    if(c == '\b' && i>0){
      put_char(STDOUT, c);
      command[--i] = 0;

    }
    else if(c != '\b'){
      put_char(STDOUT, c);
      command[i++] = c;
    }
	}
  sprint(STDERR, "\n");
	command[i] = 0;

  // Check if de command is valid.
  // Execute it if its valid
  if(!execute_command(command)){
    //If not valid, show mensage
    sprint(STDERR, "Comando invalido \n");
    sprint(STDERR, command);
    sprint(STDERR, "\n");
  }
}

int execute_command(char * command){

  for(int i=0 ; valid_commands[i].name  ; ++i){
    if(strcmp(command,valid_commands[i].name) == 0){
      valid_commands[i].action();
      return 1;
    }
  }

  return 0;
}

void print_mem(void){
  signed int counter = 0;
  char c;
  uint64_t value = 0;
  my_printf("Ingrese una direccion de memoria en formato hexadecimal: ");

  uint8_t num[32];

  while((c=get_char()) != '\n' && counter < 32){
    if(( c >= '0' && c <= '9')) {
       num[counter++] = (c - '0');
    }
    else if(( c >= 'A' ) && ( c <= 'F' )) {
       num[counter++] = (c - 'A' + 10);
    }
    else if(( c >= 'a') && ( c <= 'f')) {
        num[counter++] = (c - 'a' + 10);
    }
    else if(c == '\b') {
      if(counter > 0){
        put_char(STDOUT, c);
        num[--counter] = 0;
      }
    }
    else {
      num[counter++] = 16 + c;
    }
    if(c != '\b'){
      put_char(STDOUT, c);
    }
  }

  if(counter > 16){
    char * msg_error = "La direccion ingresada no puede tener mas de 64 bits (16 digitos hexa).";
    put_char(STDOUT, '\n');
    sprint(STDERR, msg_error);
    put_char(STDOUT, '\n');
    return;
  }

  for(int i=0 ; i < counter ; ++i){
    if(num[i] < 16){
      value = (value<<4) + num[i];
    }
    else{
      char * msg_error2 = "Formato hexa invalido.";
      put_char(STDOUT, '\n');
      sprint(STDERR, msg_error2);
      put_char(STDOUT, '\n');
      return;
    }
  }

  put_char(STDOUT, '\n');
  char * msg_error3 = "Acceso a memoria invalido.";
  if(printMem((uint64_t *)value) == -1){
    sprint(STDERR, msg_error3);
    put_char(STDOUT, '\n');
  }
}

void help(void){
    my_printf("Los comandos disponibles son:\n");

    //help
    my_printf("   'help'       - Despliega un listado de todos los comandos disponibles.\n");

    //time
	  my_printf("   'time'       - Despliega el dia y hora del sistema.\n");

    //inforeg
    my_printf("   'inforeg'    - Imprime el valor de todos los registros. \n");
    my_printf("                  Se debe presionar '-' para guardar el estado de los reistros. \n");

    //printmem
    my_printf("   'printmem'   - Realiza un volcado de memoria de 32 bytes a partir de la direccion recibida como parametro.\n");

    //play
    my_printf("   'play'       - Se inicia el juego Tron Light Cycles\n");
    my_printf("                  Jugador 1 se mueve con AWSD\n");
    my_printf("                  Jugador 2 se mueve con IJKL\n");


    //Division by 0
    my_printf("   'dividezero' - Genera una excepcion causada por dividir por 0.\n");

    //Invalid operation
    my_printf("   'invalidop'  - Genera una excepcion causada por una operacion invalida.\n");
    my_printf("   'setsmallfont' - Setea la fuente pequena\n");
    my_printf("   'setnormalfont' - Setea la fuente normal\n");
    my_printf("   'setbigfont' - Setea la fuente grande\n");
}

void time(void){
  char time[11];
  get_time(time);
  my_printf("\n Hora: %s \n", time);

  char date[11];
  get_date(date);
  my_printf("\n Fecha: %s \n", date);
}

void setSmallFont(){
  setFont(1);
}

void setNormalFont(){
  setFont(2);
}

void setBigFont(){
  setFont(3);
}