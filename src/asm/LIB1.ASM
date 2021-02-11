.model tiny
.486
.code

public setmode
public settextmode
public putstring
public pixelshow
public setpal
public cyclep

setmode:
    push bp 
     mov bp, sp
    push ax
     mov ax, word ptr [bp+4] 
     int 10h
     pop ax
     pop bp
     pop ax
     jmp ax

settextmode:
     mov ax, 3
     int 10h
     ret

putstring:
    push bp
     mov bp, sp
    push ax
    push dx
     mov dx, word ptr [bp+4]
     mov ah, 9
     int 21h
     pop dx
     pop ax
     pop bp
     ret

pixelshow:
    pusha
      mov ax, 0a000h    
      mov es, ax
      xor di, di
      mov y , 0
ly:
      mov x , 0
lx:
      mov ax, y
      mul sw
      mov di, ax
      add di, x
      mov ax, x
      mov es:[di], al
      inc x
      cmp x, 320
       jb lx
      inc y
      cmp y, 200
       jb ly
     popa
      ret
 
    sw dw  320 
     x dw  0
     y dw  0
     c db  0

cyclep:
    pusha
     push cs
      pop es
     push cs
      pop ds
      mov dx, 03c8h     
      mov al, 191
      out dx, al
      inc dx
      mov di, offset color1
      mov cx, 3
      cld
      rep insb
      mov bx, 1
cyc:
      mov dx, 03c8h
      mov al, bl
      out dx, al
      mov di, offset color2
      inc dx
      mov cx, 3
      cld
      rep  insb
      dec dx
      mov al, bl
      out dx, al
      inc dx
      mov si, offset color1
      mov cx, 3
      cld
      rep outsb
      mov si, offset color2
      mov di, offset color1
      mov cx, 3
      cld
      rep movsb
      inc bx
      cmp bx, 192
      jle  cyc
     popa
      ret

   color1 db 3 dup(0)
   color2 db 3 dup(0)


setpal:
    pusha
      xor cx, cx
set1:
      mov dx, 3c8h
      mov ax, cx
      out dx, al
      inc dx
      out dx, al
      xor ax, ax
      out dx, al
      out dx, al
      dec dx
      mov ax, cx
      add ax, 64
      out dx, al
      inc dx
      mov al, 255
      out dx, al
      mov ax, cx
      out dx, al
      xor ax, ax
      out dx, al
      dec dx
      mov ax, cx
      add ax, 128
      out dx, al
      inc dx
      mov al, 255
      out dx, al
      out dx, al
      mov ax, cx
      out dx, al
      inc cx
      cmp cx, 64
       jb set1
     popa
      ret

end
