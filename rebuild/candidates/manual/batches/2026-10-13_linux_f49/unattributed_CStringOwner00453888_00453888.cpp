// CStringOwner_00453888::CStringOwner_00453888() -- constructs one CMember_004533c5 member at +0x0; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00453888, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CMember_004533c5::CMember_004533c5 ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CMember_004533c5
// at +0x0 (default constructor @ 0x004533C5: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 20 bodies of this shape.
struct CMember_004533c5Member {
    CMember_004533c5Member();                        // 0x004533C5
};

struct CStringOwner_00453888 {
    CMember_004533c5Member m_string;                 // +0x0
    CStringOwner_00453888();
};

CStringOwner_00453888::CStringOwner_00453888()
    : m_string()
{
}
