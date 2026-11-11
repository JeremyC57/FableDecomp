# agentFAM06 lessons (one line each)
- 671 of the 688 `CIVCountedPointer_*` rows have no call, jmp or absolute reference in retail: they are /OPT:NOREF-kept COMDATs, and with no caller neither T nor the member is provable.
- The 27/24-byte release body is shared by CIVCountedPointer<T> (30 T) and CDefPointer<T> (249 T) Release/destructor alike; COMDAT selection keeps one body per symbol, so ~570 bodies is about the 2 x 280 (member, T) names, mostly CDefPointer.
- A 47-byte "assign" body that returns no `this` (no `mov eax,esi`) is not operator=: compare the 49-byte CDefPointer operator= 0x00592844; it is a void member taking a const counted-pointer reference (CDefPointer::SetDynamicCast<U> shape).
- Two byte-identical COMDAT bodies cannot carry the same symbol (the linker keeps one per name): 0x009BDE90 / 0x009BDFA0 cannot both be ??1CSubTitle.
- A CTC component destructor with no vtable store (release, then `jmp ~CTCBase`) is the compiler-generated destructor: VC7.1 stores the vptr in a user-declared one; emit the implicit ??1 with a qualified explicit destructor call.
- Byte-identical element loops (fill, __uninitialized_copy) over 8-byte {counted pointer, word} elements say nothing about the element type: the caller's bound container (and its TU) decides (0x00A43890 is CLipSyncDataBankEntry, not CDataBankTextEntry).
- Raw E8/E9 + dword scans give false "absolute references" inside other instructions: re-disassemble each hit from its function start before trusting it.
- STLport helpers in a /Gr TU take `// Flags: /Gr` over the shared fable_containers.h template (YI symbol) instead of a hand-written `__fastcall` copy.
