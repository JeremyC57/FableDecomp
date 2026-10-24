// _global::__chkstk -- MSVC CRT stack probe (`_chkstk`/`_alloca_probe`: touch each 4 KB guard page below esp for an eax-byte frame, then move esp and return through the caller's address)
// Grade: DIFFER(0v61) -- not authored: hand-written CRT assembly from Microsoft's own sources (Vc7/crt/src/intel/chkstk.asm), statically linked into Fable.exe; no retail C++ produced it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFEA30, 61 bytes. Compiler-invoked helper (VC emits `mov eax,N ; call __chkstk` for frames >= 4 KB).
// Read once: eax-in convention, `xchg esp,eax`, `test [ecx],eax` page probes, `mov esp,ecx ; push <return> ; ret` -- idioms VC7.1 never
// emits from C/C++ (the esp switch alone rules it out). Parked as "CRT asm-only, not ours to reconstruct"; no reference body.
