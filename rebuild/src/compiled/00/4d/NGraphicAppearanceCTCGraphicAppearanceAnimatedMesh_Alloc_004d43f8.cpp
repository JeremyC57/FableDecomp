#pragma optimize("s",on)
// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::Alloc(CThing&) -- factory: new CTCGraphicAppearanceAnimatedMesh(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D43F8, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGraphicAppearanceAnimatedMesh::CTCGraphicAppearanceAnimatedMesh ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCGraphicAppearanceAnimatedMesh) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0077B250 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCGraphicAppearanceAnimatedMesh {
    unsigned char _body[0x14];       // sizeof(CTCGraphicAppearanceAnimatedMesh) = 0x14
    CTCGraphicAppearanceAnimatedMesh(CThing* owner);                   // 0x0077B250
};

CTCGraphicAppearanceAnimatedMesh* __fastcall CTCGraphicAppearanceAnimatedMesh_Alloc(CThing* owner)
{
    return new CTCGraphicAppearanceAnimatedMesh(owner);
}