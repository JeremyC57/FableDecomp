// CStringOwner_00510540::CStringOwner_00510540() -- constructs one CFileLargeThemePalette member at +0x16; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00510540, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x16] ; call CFileLargeThemePalette::CFileLargeThemePalette ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CFileLargeThemePalette
// at +0x16 (default constructor @ 0x0050EE40: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 18 bodies of this shape.
struct CFileLargeThemePalette {
    CFileLargeThemePalette();                        // 0x0050EE40
};

struct CStringOwner_00510540 {
    unsigned char _pad[0x16];
    CFileLargeThemePalette m_string;                 // +0x16
    CStringOwner_00510540();
};

CStringOwner_00510540::CStringOwner_00510540()
    : m_string()
{
}