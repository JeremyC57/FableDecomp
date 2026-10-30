// __security_check_cookie(unsigned int cookie) -- VC7.1 CRT secchk.c: compare the frame cookie with the global, report on mismatch
// Grade: DIFFER(14v14) -- CRT secchk.c (VC7.1 ships it as hand-written asm): retail `cmp ecx,[cookie] ; jne fail ; ret ; fail: jmp report`; every C++ form makes the tail call the fall-through (`je ; jmp ; ret`). Should come from the CRT library, not reconstruction. 2 grades.
// Leaf: @__security_check_cookie@4
extern "C" unsigned int __security_cookie;        // 0x0139C8A8
extern "C" void __cdecl report_failure(void);     // 0x00BFE9C8

extern "C" void __fastcall __security_check_cookie(unsigned int cookie)
{
    if (cookie != __security_cookie)
        report_failure();
}
