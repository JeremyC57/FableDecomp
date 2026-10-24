// _global::__allmul -- MSVC CRT 64-bit multiply helper (__stdcall, two __int64 operands on the stack, product in edx:eax, `ret 10h`)
// Grade: DIFFER(0v52) -- not authored: hand-written CRT assembly from Microsoft's own sources (Vc7/crt/src/intel/llmul.asm `_allmul`), statically linked into Fable.exe; no retail C++ produced it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFEB50, 52 bytes. Compiler-invoked helper (VC emits `call __allmul` for __int64 * __int64).
// Read once: the high-words-zero fast path (`or ecx,eax ; jne` then one `mul`), the three-`mul` cross product summed in ebx --
// the llmul.asm listing line for line. Parked as "CRT asm-only, not ours to reconstruct"; no reference body (any C++ 64-bit
// multiply compiles back into a call to this helper).
