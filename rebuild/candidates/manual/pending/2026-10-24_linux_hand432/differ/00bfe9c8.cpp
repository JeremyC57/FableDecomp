// _global::report_failure -- MSVC CRT /GS failure reporter (static, `__try { __security_error_handler(_SECERR_BUFFER_OVERRUN, NULL); } __except(EXCEPTION_EXECUTE_HANDLER) { } _exit(3);`)
// Grade: DIFFER(0v48) -- not authored: statically linked Microsoft CRT object (Vc7/crt/src/seccook.c / secfail.c `report_failure`), compiled by Microsoft with the CRT's own SEH frame helper; not game code
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFE9C8, 48 bytes + the int3 (ends in the noreturn `call [__imp___exit]`, so the trailing int3 belongs to it).
// Shape read once: `push 8 ; push <scope table 0x012A8378> ; call __SEH_prolog (0x0040138C)` -- the CRT's shared SEH prolog
// (VC7.1 C++ in the game TUs never calls __SEH_prolog), the guarded call 0x00BFEFA4 (__security_error_handler) with (1, 0),
// the filter funclet `xor eax,eax ; inc eax ; ret` (EXCEPTION_EXECUTE_HANDLER), the handler restoring esp from [ebp-18h], then
// `push 3 ; call [0x0143FD7C]` (_exit import). Called from __security_check_cookie (landed 0x00BFE9F9).
// Parked as "CRT library code, not ours to reconstruct": no reference body (the SEH prolog form is not producible by the lane).
