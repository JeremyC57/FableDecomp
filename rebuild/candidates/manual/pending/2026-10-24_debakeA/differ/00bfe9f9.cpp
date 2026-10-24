// Grade: DIFFER(14v14) -- IMPOSSIBLE (CRT hand-written asm): branch layout only: retail (CRT secchk.c, hand-written x86 asm) falls through to `ret` and jumps out to report_failure; VC7.1 always makes the sibling tail call the fall-through (`je ret ; jmp report_failure ; ret`) for ==/!=/goto/noreturn/optimize("s") forms (5 grades)
// __security_check_cookie(unsigned long cookie) -- /GS epilogue check of a frame's stack cookie
// Retail 0x00BFE9F9, 14 bytes (MSVCR71-era static CRT, secchk.c):
//   cmp ecx,[__security_cookie 0x0139C8A8] ; jne fail ; ret ; fail: jmp report_failure
// The CRT's own copy is written as inline assembly; the same bytes come from the plain C
// form below: a __fastcall check whose failure arm is a sibling tail call (`jmp`) into the
// file-static report_failure (0x00BFE9C8, an SEH-framed body that raises the GS failure and
// terminates). VC7.1 lays the equal case out first with its own `ret`.
// Leaf: __security_check_cookie

extern "C" unsigned long __security_cookie;            // 0x0139C8A8
extern "C" void __cdecl report_failure(void);           // 0x00BFE9C8

extern "C" void __fastcall __security_check_cookie(unsigned long cookie)
{
    if (cookie == __security_cookie)
        return;
    report_failure();
}
