// CStringOwner_0059af53::CStringOwner_0059af53() -- constructs one CMember_0059ac41 member at +0x0; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059AF53, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CMember_0059ac41::CMember_0059ac41 ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CMember_0059ac41
// at +0x0 (default constructor @ 0x0059AC41: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 20 bodies of this shape.
struct CMember_0059ac41Member {
    CMember_0059ac41Member();                        // 0x0059AC41
};

struct CStringOwner_0059af53 {
    CMember_0059ac41Member m_string;                 // +0x0
    CStringOwner_0059af53();
};

CStringOwner_0059af53::CStringOwner_0059af53()
    : m_string()
{
}
