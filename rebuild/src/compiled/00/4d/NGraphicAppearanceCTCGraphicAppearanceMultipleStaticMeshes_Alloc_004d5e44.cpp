#pragma optimize("s",on)
// NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::Alloc(CThing&) -- factory: new CTCGraphicAppearanceMultipleStaticMeshes(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5E44, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGraphicAppearanceMultipleStaticMeshes::CTCGraphicAppearanceMultipleStaticMeshes ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCGraphicAppearanceMultipleStaticMeshes) = 0x20 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007E14C0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

struct CTCGraphicAppearanceMultipleStaticMeshes {
    unsigned char _body[0x20];       // sizeof(CTCGraphicAppearanceMultipleStaticMeshes) = 0x20
    CTCGraphicAppearanceMultipleStaticMeshes(CThing* owner);                   // 0x007E14C0
};

CTCGraphicAppearanceMultipleStaticMeshes* __fastcall CTCGraphicAppearanceMultipleStaticMeshes_Alloc(CThing* owner)
{
    return new CTCGraphicAppearanceMultipleStaticMeshes(owner);
}