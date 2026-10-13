// CStringOwner_00bc9d60::CStringOwner_00bc9d60() -- constructs one CMember_00bf2880 member at +0x0; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC9D60, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CMember_00bf2880::CMember_00bf2880 ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CMember_00bf2880
// at +0x0 (default constructor @ 0x00BF2880: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 20 bodies of this shape.
struct CMember_00bf2880Member {
    CMember_00bf2880Member();                        // 0x00BF2880
};

struct CStringOwner_00bc9d60 {
    CMember_00bf2880Member m_string;                 // +0x0
    CStringOwner_00bc9d60();
};

CStringOwner_00bc9d60::CStringOwner_00bc9d60()
    : m_string()
{
}
