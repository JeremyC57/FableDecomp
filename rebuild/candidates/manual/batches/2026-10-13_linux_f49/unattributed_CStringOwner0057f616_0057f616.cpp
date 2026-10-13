// CStringOwner_0057f616::CStringOwner_0057f616() -- constructs one CMember_0057a5dd member at +0x0; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057F616, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CMember_0057a5dd::CMember_0057a5dd ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CMember_0057a5dd
// at +0x0 (default constructor @ 0x0057A5DD: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 20 bodies of this shape.
struct CMember_0057a5ddMember {
    CMember_0057a5ddMember();                        // 0x0057A5DD
};

struct CStringOwner_0057f616 {
    CMember_0057a5ddMember m_string;                 // +0x0
    CStringOwner_0057f616();
};

CStringOwner_0057f616::CStringOwner_0057f616()
    : m_string()
{
}
