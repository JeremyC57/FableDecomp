// CStringOwner_00675be0::CStringOwner_00675be0() -- constructs one CRandomAppearanceMorph member at +0x0; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675BE0, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CRandomAppearanceMorph::CRandomAppearanceMorph ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CRandomAppearanceMorph
// at +0x0 (default constructor @ 0x0086C4D0: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 20 bodies of this shape.
struct CRandomAppearanceMorphMember {
    CRandomAppearanceMorphMember();                        // 0x0086C4D0
};

struct CStringOwner_00675be0 {
    CRandomAppearanceMorphMember m_string;                 // +0x0
    CStringOwner_00675be0();
};

CStringOwner_00675be0::CStringOwner_00675be0()
    : m_string()
{
}