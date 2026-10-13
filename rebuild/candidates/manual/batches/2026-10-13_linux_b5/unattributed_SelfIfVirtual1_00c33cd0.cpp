// <unattributed>::SelfIfVirtual1() -- `this` when virtual slot 1 answers true, else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C33CD0, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; call [eax+4] ; neg al ; sbb eax,eax ; and eax,esi ; pop esi ; ret
// A pointer-returning query: virtual slot 1 (a bool predicate) selects between `this` and null;
// `neg al ; sbb eax,eax ; and eax,esi` is the branch-free `cond ? this : 0`.
struct CSelfQuery_Virtuals {
    virtual void v00();
    virtual bool IsEnabled();               // slot 1 (+0x04)
    CSelfQuery_Virtuals* SelfIfVirtual1();
};

CSelfQuery_Virtuals* CSelfQuery_Virtuals::SelfIfVirtual1()
{
    return IsEnabled() ? this : 0;
}
