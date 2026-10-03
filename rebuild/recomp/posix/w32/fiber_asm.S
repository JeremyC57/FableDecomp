/* Fiber context switch: void w32_swapctx(void** saveSp, void* newSp).
   Saves the callee-saved registers and FP control state on the current stack, stores the
   stack pointer in *saveSp, switches to newSp and restores the frame found there (laid out
   by CreateFiberEx for a new fiber). */
#if defined(__x86_64__)
    .text
    .globl w32_swapctx
    .type w32_swapctx, @function
w32_swapctx:
    pushq %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    subq $8, %rsp
    stmxcsr (%rsp)
    fnstcw 4(%rsp)
    movq %rsp, (%rdi)
    movq %rsi, %rsp
    ldmxcsr (%rsp)
    fldcw 4(%rsp)
    addq $8, %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp
    ret
    .size w32_swapctx, .-w32_swapctx

    .globl w32_fiber_trampoline
    .type w32_fiber_trampoline, @function
w32_fiber_trampoline:
    andq $-16, %rsp
    call w32_fiber_entry
    ud2
    .size w32_fiber_trampoline, .-w32_fiber_trampoline
#elif defined(__aarch64__)
    .text
    .globl w32_swapctx
    .type w32_swapctx, %function
w32_swapctx:
    sub sp, sp, #176
    stp x19, x20, [sp, #0]
    stp x21, x22, [sp, #16]
    stp x23, x24, [sp, #32]
    stp x25, x26, [sp, #48]
    stp x27, x28, [sp, #64]
    stp x29, x30, [sp, #80]
    stp d8, d9, [sp, #96]
    stp d10, d11, [sp, #112]
    stp d12, d13, [sp, #128]
    stp d14, d15, [sp, #144]
    mrs x9, fpcr
    str x9, [sp, #160]
    mov x9, sp
    str x9, [x0]
    mov sp, x1
    ldp x19, x20, [sp, #0]
    ldp x21, x22, [sp, #16]
    ldp x23, x24, [sp, #32]
    ldp x25, x26, [sp, #48]
    ldp x27, x28, [sp, #64]
    ldp x29, x30, [sp, #80]
    ldp d8, d9, [sp, #96]
    ldp d10, d11, [sp, #112]
    ldp d12, d13, [sp, #128]
    ldp d14, d15, [sp, #144]
    ldr x9, [sp, #160]
    msr fpcr, x9
    add sp, sp, #176
    ret
    .size w32_swapctx, .-w32_swapctx

    .globl w32_fiber_trampoline
    .type w32_fiber_trampoline, %function
w32_fiber_trampoline:
    bl w32_fiber_entry
    brk #0
    .size w32_fiber_trampoline, .-w32_fiber_trampoline
#else
#error "unsupported architecture"
#endif
    .section .note.GNU-stack,"",%progbits
