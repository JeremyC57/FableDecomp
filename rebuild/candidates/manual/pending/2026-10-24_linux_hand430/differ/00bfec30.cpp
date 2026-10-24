// _global::__allrem -- MSVC CRT 64-bit helper: signed 64-bit remainder (__stdcall, ret 10h; operands negated to unsigned, remainder sign follows the dividend)
// Grade: DIFFER(0v178) -- not authored: hand-written CRT assembly from Microsoft's own sources (Vc7/crt/src/intel/llrem.asm, `_allrem PROC NEAR`), statically linked into Fable.exe; no retail C++ produced it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFEC30, 178 bytes. Compiler-invoked helper (VC emits `call __allrem`-style references for
// __int64 % __int64). The body uses idioms VC7.1 never emits from C/C++ (shr/rcr normalisation loop, xchg ecx,eax, dec edi / jns sign flag), matching the CRT .asm line for line.
// Parked as "CRT asm-only, not ours to reconstruct"; there is no C++ reference body because any C++ spelling of the
// operation compiles back into a call to this helper.
