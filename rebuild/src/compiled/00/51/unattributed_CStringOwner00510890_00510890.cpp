// CStringOwner_00510890::CStringOwner_00510890() -- constructs one CMember_0050ee40 member at +0x16; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00510890, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x16] ; call CMember_0050ee40::CMember_0050ee40 ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CMember_0050ee40
// at +0x16 (default constructor @ 0x0050EE40: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 18 bodies of this shape.
struct CMember_0050ee40Member {
    CMember_0050ee40Member();                        // 0x0050EE40
};

struct CStringOwner_00510890 {
    unsigned char _pad[0x16];
    CMember_0050ee40Member m_string;                 // +0x16
    CStringOwner_00510890();
};

CStringOwner_00510890::CStringOwner_00510890()
    : m_string()
{
}