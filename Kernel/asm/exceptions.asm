GLOBAL print_registers
GLOBAL give_control_to_user
EXTERN print
EXTERN printRegisterFormat
EXTERN newLine
EXTERN main
EXTERN getStackBase
GLOBAL load_registers
section .text

  print_registers:
      push rbp
      mov rbp, rsp

      xor r10, r10

  .loop_registers:
      
      mov rdi, [registers+r10]
      call print
      add r10, 8
      mov rdi, [rbp+r10+8]
      call printRegisterFormat
      call newLine
      cmp r10, length
      jne .loop_registers

      mov rsp, rbp
      pop rbp
      ret




; Retreived from https://os.phil-opp.com/handling-exceptions/ 
    ; STACK
    ;    RBP     -> stack frame
    ;   RIP     -> dirección de retorno
    ;   rax
    ;   rbx
    ;   rcx
    ;   rdx
    ;   rbp
    ;   rdi
    ;   rsi
    ;   r8
    ;   r9
    ;   r10
    ;   r11
    ;   r12
    ;   r13
    ;   r14
    ;   r15
    ;   RIP     -> instruction that caused the exception -> this is pushed by the microproccesor
    ;   CS
    ;   RFLAGS
    ;   RSP
    ;   SS

load_registers:
    enter 0, 0

    mov rbx, 0
.loop:
    mov rax, [rbp+rbx+16]
    mov [registers1+rbx], rax
    add rbx, 8
    cmp rbx, length1
    jne .loop
    
    mov rax, registers
    leave
    ret


section .data
      length1 equ 20*8
      segmentSS db " SS = ", 0
      rflags db " RFLAGS = ", 0
      segmentCS db " CS = ", 0
      registerRIP db " RIP = ", 0
      registerRAX db " RAX = ", 0
      registerRBX db " RBX = ", 0
      registerRCX db " RCX = ", 0
      registerRDX db " RDX = ", 0
      registerRBP db " RBP = ", 0
      registerRDI db " RDI = ", 0
      registerRSI db " RSI = ", 0
      registerRSP db " RSP = ", 0
      registerR8 db  " R8 = ", 0
      registerR9 db  " R9 = ", 0
      registerR10 db " R10 = ", 0
      registerR11 db " R11 = ", 0
      registerR12 db " R12 = ", 0
      registerR13 db " R13 = ", 0
      registerR14 db " R14 = ", 0
      registerR15 db " R15 = ", 0
      registers dq  registerRAX, registerR15, registerR14, registerR13, registerR12, registerR11, registerR10,registerR9, registerR8, registerRSI, registerRDI, registerRBP, registerRDX, registerRCX, registerRBX, registerRIP, segmentCS, rflags, registerRSP, segmentSS
      length equ $-registers
      
section .bss
    registers1 resq 20