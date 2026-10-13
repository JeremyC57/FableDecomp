// CStringOwner_00b288c0::CStringOwner_00b288c0() -- constructs one CMember_0042fa30 member at +0x4; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B288C0, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x4] ; call CMember_0042fa30::CMember_0042fa30 ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CMember_0042fa30
// at +0x4 (default constructor @ 0x0042FA30: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 18 bodies of this shape.
struct CMember_0042fa30Member {
    CMember_0042fa30Member();                        // 0x0042FA30
};

struct CStringOwner_00b288c0 {
    unsigned char _pad[0x4];
    CMember_0042fa30Member m_string;                 // +0x4
    CStringOwner_00b288c0();
};

CStringOwner_00b288c0::CStringOwner_00b288c0()
    : m_string()
{
}