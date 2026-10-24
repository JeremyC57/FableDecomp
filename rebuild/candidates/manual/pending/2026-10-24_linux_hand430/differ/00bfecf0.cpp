// _global::__aullshr -- MSVC CRT 64-bit helper: unsigned 64-bit right shift of edx:eax by cl
// Grade: DIFFER(0v31) -- not authored: hand-written CRT assembly from Microsoft's own sources (Vc7/crt/src/intel/ullshr.asm, `_aullshr PROC NEAR`), statically linked into Fable.exe; no retail C++ produced it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFECF0, 31 bytes. Compiler-invoked helper (VC emits `call __aullshr`-style references for
// unsigned __int64 >> n). The body uses idioms VC7.1 never emits from C/C++ (shrd eax,edx,cl and register-only edx:eax/cl calling convention), matching the CRT .asm line for line.
// Parked as "CRT asm-only, not ours to reconstruct"; there is no C++ reference body because any C++ spelling of the
// operation compiles back into a call to this helper.
