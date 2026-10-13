// CStringOwner_00cd2e2d::CStringOwner_00cd2e2d() -- constructs one CMember_007e72a0 member at +0x0; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD2E2D, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CMember_007e72a0::CMember_007e72a0 ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CMember_007e72a0
// at +0x0 (default constructor @ 0x007E72A0: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 20 bodies of this shape.
struct CMember_007e72a0Member {
    CMember_007e72a0Member();                        // 0x007E72A0
};

struct CStringOwner_00cd2e2d {
    CMember_007e72a0Member m_string;                 // +0x0
    CStringOwner_00cd2e2d();
};

CStringOwner_00cd2e2d::CStringOwner_00cd2e2d()
    : m_string()
{
}