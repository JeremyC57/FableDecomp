// EH unwind funclet @ 0x012133C0 of CPhysicalPlayer::Play (parent @ 0x00C271F0): VC7.1 emits the
// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;
// the linker placed Fable's blocks at 0x0121xxxx. This row is the parent's whole 18-byte block at +0x0:
// the state-0 unwind funclet (8 bytes: `lea ecx,[ebp-60h] ; jmp` the dtor 0x00C0FFE0 of the frame
// object at ebp-60h) followed by the parent's `__ehhandler$` thunk at 0x012133C8 (10 bytes: `mov eax,
// offset FuncInfo 0x0136CCB4 ; jmp ___CxxFrameHandler`). Proof: the parent's prologue pushes 0x012133C8
// (0x00C271F2); FuncInfo 0x0136CCB4 (magic 0x19930520) has its unwind-map action 0x012133C0 at 0x0136CCB0.
// It is produced by the parent's translation unit, included below (no code of its own).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012133C0, oracle length 18 bytes.
// Leaf: Unwind@012133c0
// Flags: /EHsc /MD
#include "../src/compiled/00/c2/CPhysicalPlayer_Play_00c271f0.cpp"