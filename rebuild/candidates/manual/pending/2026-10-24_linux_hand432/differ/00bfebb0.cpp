// _global::__aullrem -- MSVC CRT unsigned 64-bit remainder helper (__stdcall, dividend and divisor on the stack, remainder in edx:eax, `ret 10h`)
// Grade: DIFFER(0v117) -- not authored: hand-written CRT assembly from Microsoft's own sources (Vc7/crt/src/intel/ullrem.asm `_aullrem`), statically linked into Fable.exe; no retail C++ produced it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFEBB0, 117 bytes. Compiler-invoked helper (VC emits `call __aullrem` for unsigned __int64 %).
// Read once: the 32-bit-divisor fast path (two `div ecx`), the shr/rcr normalisation loop, `xchg ecx,eax`, the quotient
// back-multiply and the borrow-chain correction (`sub/sbb`, `neg edx ; neg eax ; sbb edx,0`) -- idioms VC7.1 never emits from C++.
// Parked as "CRT asm-only, not ours to reconstruct"; no reference body.
