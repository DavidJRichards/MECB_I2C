;
; IRQ handling (CreatiVision version)
;

        .export         initirq, doneirq, __ZP_START__
;       .import         callirq, irq2
        .import         callirq

;        .include        "creativision.inc"
BIOS_IRQ2_ADDR          := $FF52

.zeropage
    __ZP_START__:

; ------------------------------------------------------------------------

.segment        "ONCE"

initirq:
        lda     #<IRQStub
        ldx     #>IRQStub
        jmp     setvec

; ------------------------------------------------------------------------

.code

irq2:   jmp     BIOS_IRQ2_ADDR

doneirq:
        lda     #<BIOS_IRQ2_ADDR
        ldx     #>BIOS_IRQ2_ADDR
setvec: sei
        sta     irq2+1
        stx     irq2+2
        cli
        rts

; ------------------------------------------------------------------------

.segment        "CODE"

IRQStub:
        cld                             ; Just to be sure
        jsr     callirq                 ; Call the functions
        jmp     BIOS_IRQ2_ADDR          ; Jump to the BIOS IRQ vector

