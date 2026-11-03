// CStringOwner_00510990::CStringOwner_00510990() -- constructs one CFileLargeThemePalette member at +0xe; the owning class is not identifiable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00510990, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xe] ; call CFileLargeThemePalette::CFileLargeThemePalette ; mov eax,esi ; pop esi ; ret
// Default constructor of a non-polymorphic class whose only constructed member is a CFileLargeThemePalette
// at +0xe (default constructor @ 0x0050EE40: stores a null data pointer and bumps the
// string-instance counter). No vtable store and no other initialisation, so the owning class
// cannot be named from the body; it is landed under a descriptive placeholder. One of
// 18 bodies of this shape.
struct CFileLargeThemePalette {
    CFileLargeThemePalette();                        // 0x0050EE40
};

struct CStringOwner_00510990 {
    unsigned char _pad[0xe];
    CFileLargeThemePalette m_string;                 // +0xe
    CStringOwner_00510990();
};

CStringOwner_00510990::CStringOwner_00510990()
    : m_string()
{
}