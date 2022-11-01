
#ifndef SYSTEMCALLS_H_
#define SYSTEMCALLS_H_

#include <stdint.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2

int sys_write(uint64_t fd, char * buffer, uint64_t size);
int sys_read(uint64_t fd, char * buffer, uint64_t size);
void sys_date(char * buffer);
int sys_hasTicked();
void sys_time(char * buffer);
void sys_date(char * buffer);
void sys_clearWindow();
void sys_restartCursor();
void sys_divide();
void sys_uniqueWindow();
int sys_printmem(uint64_t * mem_address);
void sys_setScreen(uint8_t id);
void store_registers(uint64_t * start);
void sys_infoReg();
void sys_paint(unsigned int fd, int direction, uint32_t position);
int sys_seconds_elapsed();
int sys_miliseconds_elapsed();
void sys_set_font(int fontNumber);

#endif /* SYSTEMCALLS_H_ */
