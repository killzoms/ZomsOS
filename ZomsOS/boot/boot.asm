[org 0x7c00]
[bits 16]

.text:
_start:
    mov [BOOT_DRIVE], dl

    mov bp, 0x9000
    mov sp, bp
    
    mov bx, KERNEL_OFFSET
    mov dh, 15
    mov dl, [BOOT_DRIVE]
    call disk_load

    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm


    disk_load:
        push dx

        mov ah, 0x02
        mov al, dh
        mov ch, 0x00
        mov dh, 0x00
        mov cl, 0x02

        int 0x13

        pop dx
        ret

[bits 32]
    init_pm:
        mov ax, DATA_SEG
        mov ds, ax
        mov ss, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        mov ebp, 0x90000
        mov esp, ebp
        
        jmp KERNEL_OFFSET

.data:
    KERNEL_OFFSET equ 0x1000
    TEXT_LOC equ 0xb8000
    BOOT_DRIVE: db 0

    gdt_start:

    gdt_null:
        dd 0x0
        dd 0x0
    gdt_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 10011010b
        db 11001111b
        db 0x0
    gdt_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 10010010b
        db 11001111b
        db 0x0
    gdt_end:

    gdt_descriptor:
        dw gdt_end - gdt_start - 1
        dd gdt_start

    CODE_SEG equ gdt_code - gdt_start
    DATA_SEG equ gdt_data - gdt_start


times 510-($-$$) db 0
db 0x55
db 0xAA