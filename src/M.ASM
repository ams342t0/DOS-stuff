.model tiny
.486

extrn   setmode:NEAR
extrn putstring:NEAR
extrn pixelshow:NEAR
extrn    cyclep:NEAR
extrn    setpal:NEAR

.code
org 100h

begin:
    push 13h
    call setmode
    call pixelshow
    push offset msg
    call putstring
    push offset prompt
    call putstring
    call justwait
    push 3
    call setmode
    mov ax, 4c00h
    int 21h

justwait:
    push ax
    xor  ax, ax
w:
    call cyclep
    mov ah, 1
    int 16h
    or  al, 0
    jz w
    pop ax
    ret


msg     db "Welcome back to DOS",13,10,'$'     
prompt  db "Press any key to continue...$"      
end begin
