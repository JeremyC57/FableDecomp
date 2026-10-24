// _global::__aulldiv -- MSVC CRT 64-bit helper: unsigned 64-bit division (__stdcall, ret 10h)
// Grade: DIFFER(0v104) -- not authored: hand-written CRT assembly from Microsoft's own sources (Vc7/crt/src/intel/ulldiv.asm, `_aulldiv PROC NEAR`), statically linked into Fable.exe; no retail C++ produced it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFEE00, 104 bytes. Compiler-invoked helper (VC emits `call __aulldiv`-style references for
// unsigned __int64 / unsigned __int64). The body uses idioms VC7.1 never emits from C/C++ (shr/rcr normalisation loop, quotient correction by dec esi), matching the CRT .asm line for line.
// Parked as "CRT asm-only, not ours to reconstruct"; there is no C++ reference body because any C++ spelling of the
// operation compiles back into a call to this helper.
