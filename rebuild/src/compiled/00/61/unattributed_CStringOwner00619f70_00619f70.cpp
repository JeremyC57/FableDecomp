// CStringOwner_00619f70::CStringOwner_00619f70() -- constructs one CCharString member at +0x4; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00619F70, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x4] ; call CCharString::CCharString ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CCharString
// at +0x4 (default constructor @ 0x0099AED0: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 18 bodies of this shape.
namespace N_0099aed0 { struct CCharStringMember {
    CCharStringMember();                        // 0x0099AED0
}; } using N_0099aed0::CCharStringMember;

struct CStringOwner_00619f70 {
    unsigned char _pad[0x4];
    CCharStringMember m_string;                 // +0x4
    CStringOwner_00619f70();
};

CStringOwner_00619f70::CStringOwner_00619f70()
    : m_string()
{
}