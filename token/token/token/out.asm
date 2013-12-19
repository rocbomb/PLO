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
@str0   DB "please input num <10 ",0
@str1   DB "please input number> ",0

MAX@main@ DWORD 1024
MIN@main@ DWORD -1024
ZERO@main@ DWORD 0
CHAR@main@ DWORD 67


.CODE

roc_swap PROC
	push EBP
	mov EBP,ESP
	sub ESP,8
	mov EBX,dword ptr[EBP+16]  ;pointer
	mov EAX,dword ptr[EBX]
	mov dword ptr [EBP-8],EAX
	mov EBX,dword ptr[EBP+20]  ;pointer
	mov EAX,dword ptr[EBX]
	mov EBX,dword ptr[EBP+16]  ;get pointer
	mov dword ptr[EBX],EAX
	mov EAX,dword ptr [EBP-8]
	mov EBX,dword ptr[EBP+20]  ;get pointer
	mov dword ptr[EBX],EAX
	mov	eax, dword ptr[ebp - 4]
	leave 
	ret 0
roc_swap ENDP


roc_qsort PROC
	push EBP
	mov EBP,ESP
	sub ESP,72
	mov EAX,dword ptr[EBP+12]  ;para
	mov dword ptr [EBP-8],EAX
	mov EAX,dword ptr[EBP+16]  ;para
	mov dword ptr [EBP-12],EAX
	mov EAX,dword ptr [EBP-8]
	mov ECX,dword ptr [EBP-12]
	add EAX,ECX
	mov dword ptr [EBP-36],EAX
	mov EAX,dword ptr [EBP-36]
	mov ECX,2 
	cdq
	idiv ECX
	mov dword ptr [EBP-40],EAX
	mov EBX,dword ptr [EBP+8]
	lea EAX,dword ptr[EBX-44] ;array addr
	mov ECX,dword ptr [EBP-40]
	shl ECX,2
	add EAX,ECX ;array addr + offset
	mov ECX,dword ptr[EAX] ;get array addr + offset value
	mov dword ptr[EBP-32],ECX 
	mov EAX,dword ptr [EBP-32]
	mov dword ptr [EBP-20],EAX
_lab1:
	mov EAX,dword ptr [EBP-8]
	mov ECX,dword ptr [EBP-12]
	cmp EAX,ECX
	jge _lab2
_lab3:
	mov EBX,dword ptr [EBP+8]
	lea EAX,dword ptr[EBX-44] ;array addr
	mov ECX,dword ptr [EBP-8]
	shl ECX,2
	add EAX,ECX ;array addr + offset
	mov ECX,dword ptr[EAX] ;get array addr + offset value
	mov dword ptr[EBP-44],ECX 
	mov EAX,dword ptr [EBP-44]
	mov ECX,dword ptr [EBP-20]
	cmp EAX,ECX
	jge _lab4
	mov EAX,dword ptr [EBP-8]
	mov ECX,1 
	add EAX,ECX
	mov dword ptr [EBP-48],EAX
	mov EAX,dword ptr [EBP-48]
	mov dword ptr [EBP-8],EAX
	jmp _lab3
_lab4:
_lab5:
	mov EBX,dword ptr [EBP+8]
	lea EAX,dword ptr[EBX-44] ;array addr
	mov ECX,dword ptr [EBP-12]
	shl ECX,2
	add EAX,ECX ;array addr + offset
	mov ECX,dword ptr[EAX] ;get array addr + offset value
	mov dword ptr[EBP-52],ECX 
	mov EAX,dword ptr [EBP-20]
	mov ECX,dword ptr [EBP-52]
	cmp EAX,ECX
	jge _lab6
	mov EAX,dword ptr [EBP-12]
	mov ECX,1 
	sub EAX,ECX
	mov dword ptr [EBP-56],EAX
	mov EAX,dword ptr [EBP-56]
	mov dword ptr [EBP-12],EAX
	jmp _lab5
_lab6:
	mov EAX,dword ptr [EBP-8]
	mov ECX,dword ptr [EBP-12]
	cmp EAX,ECX
	jg _lab7
	mov EBX,dword ptr [EBP+8]
	lea EAX,dword ptr[EBX-44] ;array addr
	mov ECX,dword ptr [EBP-8]
	shl ECX,2
	add EAX,ECX ;array addr + offset
	mov dword ptr[EBP-60],EAX 
	mov EBX,dword ptr [EBP+8]
	lea EAX,dword ptr[EBX-44] ;array addr
	mov ECX,dword ptr [EBP-12]
	shl ECX,2
	add EAX,ECX ;array addr + offset
	mov dword ptr[EBP-64],EAX 
	mov EAX,dword ptr [EBP-64]
	push EAX
	mov EAX,dword ptr [EBP-60]
	push EAX
  ;apd area
	mov  EAX, dword ptr [EBP + 8] 
	push EAX
	push EBP
	call roc_swap
	mov EAX,dword ptr [EBP-8]
	mov ECX,1 
	add EAX,ECX
	mov dword ptr [EBP-68],EAX
	mov EAX,dword ptr [EBP-68]
	mov dword ptr [EBP-8],EAX
	mov EAX,dword ptr [EBP-12]
	mov ECX,1 
	sub EAX,ECX
	mov dword ptr [EBP-72],EAX
	mov EAX,dword ptr [EBP-72]
	mov dword ptr [EBP-12],EAX
_lab7:
	jmp _lab1
_lab2:
	mov EAX,dword ptr [EBP-12]
	mov ECX,dword ptr[EBP+12]  ;para
	cmp EAX,ECX
	jle _lab8
	mov EAX,dword ptr [EBP-12]
	push EAX
	mov EAX,dword ptr[EBP+12]  ;para
	push EAX
  ;apd area
	mov  EAX, dword ptr [EBP + 8] 
	push EAX
	call roc_qsort
_lab8:
	mov EAX,dword ptr [EBP-8]
	mov ECX,dword ptr[EBP+16]  ;para
	cmp EAX,ECX
	jge _lab9
	mov EAX,dword ptr[EBP+16]  ;para
	push EAX
	mov EAX,dword ptr [EBP-8]
	push EAX
  ;apd area
	mov  EAX, dword ptr [EBP + 8] 
	push EAX
	call roc_qsort
_lab9:
	mov	eax, dword ptr[ebp - 4]
	leave 
	ret 0
roc_qsort ENDP

start:
	push EBP
	mov EBP,ESP
	sub ESP,88
	push offset @str0
	push offset print_string
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
	lea EAX,dword ptr [EBP-52]
	push EAX
	push offset scanf_int
	call crt_scanf
	add  ESP ,  8
	;ASSIGN im
	mov EAX, 1
	mov dword ptr [EBP-48],EAX
_lab12:
	mov EAX,dword ptr [EBP-48]
	mov ECX,dword ptr [EBP-52]
	cmp EAX,ECX
	jg _lab11
	jmp _lab10
_lab13:
	mov EAX,dword ptr [EBP-48]
	mov ECX,1 
	add EAX,ECX
	mov dword ptr [EBP-48],EAX
	jmp _lab12
_lab10:
	push offset @str1
	push offset print_string
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
	lea EAX,dword ptr [EBP-56]
	push EAX
	push offset scanf_int
	call crt_scanf
	add  ESP ,  8
	mov EAX,dword ptr [EBP-48]
	mov ECX,1 
	sub EAX,ECX
	mov dword ptr [EBP-72],EAX
	lea EAX,dword ptr [EBP-44];array addr
	mov ECX,dword ptr [EBP-72]
	shl ECX,2
	add EAX,ECX ;array addr + offset
	mov ECX,dword ptr [EBP-56]
	mov dword ptr [EAX], ECX 
	jmp _lab13
_lab11:
	mov EAX,dword ptr [EBP-52]
	mov ECX,1 
	sub EAX,ECX
	mov dword ptr [EBP-76],EAX
	mov EAX,dword ptr [EBP-76]
	push EAX
	mov EAX,0 
	push EAX
  ;apd area
	push EBP
	call roc_qsort
	;ASSIGN im
	mov EAX, 0
	mov dword ptr [EBP-48],EAX
_lab16:
	mov EAX,dword ptr [EBP-52]
	mov ECX,1 
	sub EAX,ECX
	mov dword ptr [EBP-80],EAX
	mov EAX,dword ptr [EBP-48]
	mov ECX,dword ptr [EBP-80]
	cmp EAX,ECX
	jg _lab15
	jmp _lab14
_lab17:
	mov EAX,dword ptr [EBP-48]
	mov ECX,1 
	add EAX,ECX
	mov dword ptr [EBP-48],EAX
	jmp _lab16
_lab14:
	lea EAX,dword ptr [EBP-44];array addr
	mov ECX,dword ptr [EBP-48]
	shl ECX,2
	add EAX,ECX ;array addr + offset
	mov ECX,dword ptr[EAX] ;get array addr + offset value
	mov dword ptr[EBP-88],ECX 
	mov EAX,dword ptr [EBP-88]
	push EAX
	push offset print_int
	call crt_printf
	add  ESP ,  8
	push 10
	push offset print_char
	call crt_printf
	add  ESP ,  8
	jmp _lab17
_lab15:
	push 0
	CALL ExitProcess
	LEAVE  
	RET 0
END start
