// CStringOwner_00869fa0::CStringOwner_00869fa0() -- constructs one CCharString member at +0x0; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00869FA0, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CCharString::CCharString ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CCharString
// at +0x0 (default constructor @ 0x0099E4B0: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 20 bodies of this shape.
struct CCharStringMember {
    CCharStringMember();                        // 0x0099E4B0
};

struct CStringOwner_00869fa0 {
    CCharStringMember m_string;                 // +0x0
    CStringOwner_00869fa0();
};

CStringOwner_00869fa0::CStringOwner_00869fa0()
    : m_string()
{
}