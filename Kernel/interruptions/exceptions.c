#include <graphicMode.h>
#include <keyboard.h>
#include <exceptions.h>

typedef void (*Exception)(void);
#define ALL_REGISTER_COUNT 20

static void zero_division(uint64_t * registers);
static void invalid_opcode(uint64_t * registers);
static void excepHandler(char * msg, uint64_t * registers);
extern void _hlt();

static void * const sampleCodeModuleAddress = (void*)0x400000;
typedef int (*EntryPoint)();

//								Exception 0						Exception 6
static Exception exceptions[]={&zero_division, 0, 0, 0, 0, 0, &invalid_opcode};
static char * message[] = {"Zero Division Exception", "Invalid Opcode Exception"};
static char * register_names[] = {"   RAX=", "   RBX=", "   RCX=", "   RDX=", "   RBP=", "   RDI=", "   RSI=", "    R8=",
                                  "    R9=", "   R10=", "   R11=", "   R12=", "   R13=", "   R14=", "   R15=", "   RIP=",
                                  "    CS=", "RFLAGS=", "   RSP=", "    SS="};


void exceptionDispatcher(int exception) {
  Exception ex = exceptions[exception];
	if(ex != 0){
		ex();
	}
}

static void excepHandler(char * msg, uint64_t * registers){
	print(msg);
	newLine();

    newLine();
    char buffer[17];
    for (int i = 0; i < ALL_REGISTER_COUNT; i++) {
        print(register_names[i]);
        printRegisterFormat(registers[i]);
        print(" ");
        if ((i+1) % 3 == 0) {
            newLine();
        }
    }

	newLine();
	newLine();

	print("Presione enter para continuar");
	int c;
	do{
		_hlt(); // halts the central processing unit until the next external interrupt is fired.
	} while((c=getChar()) != '\n');
	clearAll();
}

static void zero_division(uint64_t * registers) {
	excepHandler(message[0], registers);
}

static void invalid_opcode(uint64_t * registers){
	excepHandler(message[1], registers);
}