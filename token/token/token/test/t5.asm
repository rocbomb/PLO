;-----------------
;	ml.exe /c /coff 1.asm
;	link.exe /SUBSYSTEM:CONSOLE /OPT:NOREF 1.obj
.386
.model flat, stdcall

option casemap:none

includelib E:\masm32\lib\kernel32.lib
includelib E:\masm32\lib\msvcrt.lib

include E:\masm32\include\msvcrt.inc
include E:\masm32\include\kernel32.inc

.DATA
print_int    DB "%d",0
print_char   DB "%c",0
print_string DB "%s",0
scanf_int    DB "%d",0
scanf_char   DB "%c",0
@str0   DB "please input num <10",0
@str1   DB "please input number",0

MAX@main@ DWORD 1024
MIN@main@ DWORD -1024
ZERO@main@ DWORD 0
CHAR@main@ DWORD 67


.CODE
_lab1:
_lab3:
_lab4:
_lab5:
_lab2:
_lab6:
_lab7:
start:
	push EBP
	mov EBP,ESP
	push offset @str0
	push offset print_string
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
	push offset @str1
	push offset print_string
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
	;ASSIGN im
	mov EAX, 1
	mov [EBP - 48], EAX
_lab10:
_lab11:
_lab8:
_lab9:
	;ASSIGN im
	mov EAX, 0
	mov [EBP - 48], EAX
_lab14:
_lab15:
_lab12:
	mov  EAX, [EBP - 88]
	push EAX
	push offset print_int
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
_lab13:
	push 0
	push offset print_int
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
	push -2
	push offset print_int
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
	;ASSIGN im
	mov EAX, 1
	mov [EBP - 48], EAX
	mov  EAX, [EBP - 48]
	push EAX
	push offset print_int
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
	push 0
	CALL ExitProcess
	LEAVE  
	RET 0
END start
