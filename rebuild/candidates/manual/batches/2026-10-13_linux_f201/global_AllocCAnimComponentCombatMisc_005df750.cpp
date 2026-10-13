// Alloc_CAnimComponentCombatMisc() -- new CAnimComponentCombatMisc with the constructor inlined: member +0x4 = 1, vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DF750, oracle length 31 bytes.
// Grade: candidate -- still DIFFER(31v31) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push 0xa ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov dword ptr [eax+0x4],1 ; mov dword ptr [eax],offset CAnimComponentCombatMisc::`vftable' ; ret ; N: xor eax,eax ; ret
// Factory for a root polymorphic class: allocates sizeof(CAnimComponentCombatMisc) = 0xa bytes (operator new
// @ 0x00BFEA1A, landed-trusted), null-checks and runs the inlined default constructor (the counter
// at +0x4 set to 1, then the vtable 0x01255738 from rtti-vtables.tsv). One of 7 bodies.
// The counter store comes BEFORE the vtable store: the counter is initialised by an inlined base
// constructor (whose own vtable store is dead and dropped); a member of CAnimComponentCombatMisc itself would be
// stored after the vtable. The base is not identifiable from the body.
struct CAnimComponentCombatMisc_Base {
    virtual void v00();
    long m_count;                          // +0x4
    CAnimComponentCombatMisc_Base() : m_count(1) { }
};

struct CAnimComponentCombatMisc : CAnimComponentCombatMisc_Base {
    virtual void v00();
    unsigned char _tail[0x2];
    CAnimComponentCombatMisc() : CAnimComponentCombatMisc_Base() { }
};

CAnimComponentCombatMisc* Alloc_CAnimComponentCombatMisc()
{
    return new CAnimComponentCombatMisc;
}
