// _global::__ftol2 -- MSVC CRT float-to-__int64 truncation helper (st0 in, edx:eax out; fistp qword with the round-to-nearest correction toward zero)
// Grade: DIFFER(0v117) -- not authored: hand-written CRT assembly from Microsoft's own sources (Vc7/crt/src/intel/ftol2.asm `_ftol2`), statically linked into Fable.exe; no retail C++ produced it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFEA70, 117 bytes. Compiler-invoked helper (VC7.1 emits `call __ftol2` for every (long)/(__int64) float cast).
// Read once: FPU stack argument, 16-byte aligned frame (`and esp,-16`), `fistp qword` / `fild qword` / `fsubp`, sign-dependent
// `adc`/`sbb` correction, `leave` -- not producible from C++ (any C++ spelling of the conversion calls this very helper).
// Parked as "CRT asm-only, not ours to reconstruct"; no reference body.
