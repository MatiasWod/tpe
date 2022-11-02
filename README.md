# CFL Kernel

| Integrante | Legajo |
| --- | --- |
| Bautista Canevaro | 62179 |
| Juan Segundo Arnaude | 62184 |
| Matias Wodtke | 62098 |
| Mercedes Baron | 61187 |

<hr>

* [Descripción general](#descripción-general)
* [Compilación y ejecución del kernel](#compilación-y-ejecución-del-kernel)
  * [Prerequisitos](#prerequisitos)
  * [Opción 1](#opción-1)
  * [Opción 2](#opción-2)
* [Comandos disponibles](#comandos-disponibles)
  * [help](#help)
  * [dividezero](#dividezero)
  * [invalidop](#invalidop)
  * [inforeg](#inforeg)
  * [printmem](#printmem)
  * [time](#time)
  * [setsmallfont](#setsmallfont)
  * [setnormalfont](#setnormalfont)
  * [setbigfont](#setbigfont)
* [Teclas para ejecutar ciertas acciones](#teclas-para-ejecutar-ciertas-acciones)
* [System Calls implementadas](#system-calls-implementadas)
  * [Read](#read-0x01)
  * [Write](#write)
  * [Clear Window](#clear-window-0x04)
  * [Restart Cursor](#seconds-elapsed)
  * [Initialize Window](#system-datetime)
  * [System Date](#print-byte-from-memory)
  * [Register Info](#start-split-screen)
  * [Paint System](#start-unique-screen)
  * [Seconds Elapsed](#load-process)
  * [Miliseconds Elapsed](#hibernate-process)
  * [Set Character Font](#get-inforeg-registers)

<hr>

# Descripción general

CFL Kernel es un pequeño sistema operativo desarrollado a partir de Pure64, en el marco de la materia Arquitectura de Computadoras.

Es posible interactuar con el sistema a través de una terminal, que permite ejecutar diversos comandos para verificar su funcionamiento.

Para su utilización es necesario tener acceso a un teclado; el mouse no es utilizado.

> ⚠️ CFL Kernel supone que se posee de un teclado con [distrubución ANSI "United States"](https://upload.wikimedia.org/wikipedia/commons/5/51/KB_United_States-NoAltGr.svg).

# Compilación y ejecución del kernel

### Prerequisitos

Se necesitan tener instalados los siguientes paquetes para compilar y correr este proyecto:

- nasm
- qemu
- gcc
- make
- docker

### Opción 1

Por única vez, descargar la imagen de Docker:

```bash
$ docker pull agodio/itba-so:1.0
```

Luego, cada vez que se quiera compilar, ejecutar el siguiente comando en el directorio del proyecto:

```bash
$ docker run -v ${PWD}:/root --security-opt seccomp:unconfined -ti agodio/itba-so:1.0
```

Se iniciará el contenedor de Docker. Ahora, ejecutar los siguientes comandos:

```bash
$ cd root
$ cd Toolchain
$ make all
$ cd ..
$ make all
$ exit
```

Ahora habremos cerrado el contenedor de Docker. Solo falta ejecutar el kernel:

```bash
$ ./run.sh
```

### Opción 2

Primero, por única vez descargar la imagen de Docker:

```bash
$ docker pull agodio/itba-so:1.0
```

Luego, ejecutar el siguiente comandos en el directorio del proyecto:

```bash
$ docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -ti --name tpe agodio/itba-so:1.0
```

Luego, cada vez que se quiera compilar el proyecto, ejecutar los siguientes comandos en el directorio del proyecto:

```bash
$ ./a-compiletpe
$ ./run.sh
```

# Comandos disponibles

### `help`

Programa que despliega en pantalla una lista de comandos válidos para introducir, junto a una pequeña descripción del mismo.

### `dividezero`

Programa que realiza una división por cero. El procesador lanzará una excepción de tipo 0. El objetivo del programa es verificar el funcionamiento de dicha excepción.

### `invalidop`

Programa que intenta ejecutar una instrucción inexistente/invalida. El procesador lanzará una excepción. El objetivo del programa es verificar el funcionamiento de dicha excepción.

### `inforeg`

Programa que imprime a pantalla una lista de los registros de uso general junto al valor que almacenaban en el momento que se tomó el snapshot (para tomar snapshot utilizar '-')

> 🗣 Para guardar los registros en un cierto momento y luego poder imprimirlos con el comando, apretar la tecla $F10$.

### `printmem`

Luego de ingresar printmem se le pedira al usuario una posicion de memoria que debe cumplir ciertos requisitos para ser una dirección válida:

- Debe ser un valor hexadecimal válido
- Debe ser menor a $0xFFFFFFFF8$. Esto se debe a que ese es el límite del mapa de memoria de QEMU.

### `time`

Programa que imprime en pantalla la fecha y hora actual. Dicha fecha y hora es desplegada en GMT-3.

### `setsmallfont`

Cambia la fuente a una fuente con caracteres pequeños.

### `setnormalfont`

Cambia la fuente a una fuente con caracteres de tamaño mediano.

### `setbigfont`

Cambia la fuente a una fuente con caracteres granes.



# Teclas para ejecutar ciertas acciones

Para ejecutar ciertas acciones que disrrumpen el funcionamiento normal de la terminal, se han asignado teclas especiales del teclado.

Recordar que se supone que se posee de un teclado con [distrubución ANSI "United States"](https://upload.wikimedia.org/wikipedia/commons/5/51/KB_United_States-NoAltGr.svg).

| Acción | Tecla asociada |
| --- | --- |
| Cancelar la ejecución de un programa que se está ejecutando en la pantalla completa | F1 |
| Pausar y reanudar la ejecución de un programa que se está ejecutando en la pantalla completa | F2 |
| Pausar y reanudar la ejecución del programa que se está ejecutando en la parte izquierda en la pantalla dividida | F3 |
| Pausar y reanudar la ejecución del programa que se está ejecutando en la parte derecha en la pantalla dividida | F4 |
| Salir del modo de pantalla dividida y cancelar la ejecución de ambos programas que se están ejecutando | ESC |
| Guardar el contenido de los registros de uso general para luego imprimirlos usando el comando inforeg | F10 |

# System Calls implementadas

Se debe generar una interrupción del tipo 80 para ejecutar la system call deseada.

Los registros que se detallan a continuación deben poseer los parámetros para la ejecución de la system call.

En *RAX* se indica qué system call se desea ejecutar.

El valor de retorno de la system call se obtendrá en *RAX*.

| System Call | RAX | RDI | RSI | RDX |
| --- | --- | --- | --- | --- |
| Read | 0x00 | int fd | char * buf | int count |
| Write | 0x01 | int fd | char * buf | int count |
| Get Time | 0x02 | char * buf |  |  |
| Has Ticked | 0x03 |  |  |  |
| Clear Window | 0x04 |  |  |  |
| Restart Cursor | 0x05 |  |  |  |
| Initialize Window | 0x06 |  |  |  |
| System Date | 0x07 | char * buf |  |  |
| Register Info | 0x08 |  |  |  |
| Paint System | 0x09 | color_t color | uint32_t position |  |
| Seconds Elapsed | 0x0A |  |  |  |
| Miliseconds Elapsed | 0x0B | s |  |  |
| Set Character Font | 0x0B | uint64_t fontNumber |  |  |

### Read (0x00)

Recibe un file descriptor de donde leer, un vector de caracteres donde se guardarán los caracteres leídos y la cantidad de bytes a leer.

Retorna la cantidad de bytes leídos.

> 🗣 El primer argumento (File Descriptor) se incluye para seguir el estándar de Linux. Sin embargo, en este kernel sólo se puede leer de la entrada estándar y eso es lo que ocurrirá sin importar el valor del parámetro

### Write (0x01)

Recibe un file descriptor a donde escribir, un vector de caracteres con los caracteres a escribir y la cantidad de bytes a escribir.

Retorna la cantidad de bytes que se escribieron.

Los file descriptor disponibles son:

|   |   |
|---|---|
| 1 | Standart Output |
| 2 | Standard Error |
| 3 | Left Output |
| 4 | Left Error |
| 5 | Right Output |
| 6 | Right Error |

### Get Time (0x02)

Retorna la hora actual (hora, minutos, segundos) en el buffer enviado como parametro.


### Clear Window (0x04)

No recibe parametros.

Limpia la pantalla imprimiendo pixeles negros en toda la extension de la misma.


### Restart Cursor (0x05)

No recibe parametros.

Resetea la posicion del 'cursor' al principio de la pantalla. El cursor es la proxima posicion donde se imprimen caracteres, este se mueve a medida que se imprimen caracteres.

### Initialize Window (0x06)

No recibe argumentos.

Inicializa la pantalla.

### System Date (0x07)

Retorna la fecha actual (dia, mes, año) en el buffer enviado como parametro.

### Register Info (0x08)

Retorna la informacion de los registros tomada en la ultima 'snapshot'. 

### Paint System (0x09)

Recibe un color y una posicion de la pantalla.

Pinta la posicion de pantalla recibida del color que recibe.

### Seconds Elapsed (0x0A)

No recibe argumentos.

Retorna la cantidad de segundos desde que se inicio el kernel.

### Miliseconds Elapsed (0x0B)

No recibe argumentos.

Retorna la cantidad de milisegundos desde que se inicio el kernel.

### Set Character Font (0x0C)

Recibe un uint32_t de argumento que es el id de la fuente.

Modifica la fuente actual a la fontNumber recibida.