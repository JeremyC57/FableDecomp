// CStringOwner_0059adfc::CStringOwner_0059adfc() -- constructs one CMember_0059aaf9 member at +0x0; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059ADFC, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CMember_0059aaf9::CMember_0059aaf9 ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CMember_0059aaf9
// at +0x0 (default constructor @ 0x0059AAF9: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 20 bodies of this shape.
struct CMember_0059aaf9Member {
    CMember_0059aaf9Member();                        // 0x0059AAF9
};

struct CStringOwner_0059adfc {
    CMember_0059aaf9Member m_string;                 // +0x0
    CStringOwner_0059adfc();
};

CStringOwner_0059adfc::CStringOwner_0059adfc()
    : m_string()
{
}
