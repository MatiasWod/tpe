#ifndef _USERSTDLIB_H_
#define _USERSTDLIB_H_

#include <stdint.h>
#include <_play.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2



// stdio.h
int sprint(uint8_t fd, char * str);
void my_printf(const char * frmt, ...);
char *convert(unsigned int, int);

// string.h
char * my_strcpy(char * destination, char * source);
char * my_strncpy(char * destination, char * source, int size);
int _strlen(const char * str);
int strcmp(char * s1, char * s2);
int put_char(uint8_t fd, char c);
int get_char();
int read_char();

// time.h
void get_time(char * buffer);
void get_date(char * buffer);
int seconds_elapsed();
int miliseconds_elapsed();
void sleepMiliseconds(int miliseconds);
void sleep(int seconds);

// stdlib.h
int printMem(uint64_t * value);
int atoi(char * str);

// screen.h
void clearScreen();
void restartCursor();
void uniqueWindow();
void infoReg();
void paint(color_t color, uint32_t position);
void setFont(int fontNumber);
int getFont();

#endif /* _USERSTDLIB_H_*/
