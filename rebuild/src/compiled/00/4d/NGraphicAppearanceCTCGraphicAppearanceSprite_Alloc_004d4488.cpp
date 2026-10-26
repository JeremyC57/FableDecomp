#pragma optimize("s",on)
// NGraphicAppearance::CTCGraphicAppearanceSprite::Alloc(CThing&) -- factory: new CTCGraphicAppearanceSprite(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4488, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x30 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGraphicAppearanceSprite::CTCGraphicAppearanceSprite ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCGraphicAppearanceSprite) = 0x30 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0077C3E0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

struct CTCGraphicAppearanceSprite {
    unsigned char _body[0x30];       // sizeof(CTCGraphicAppearanceSprite) = 0x30
    CTCGraphicAppearanceSprite(CThing* owner);                   // 0x0077C3E0
};

CTCGraphicAppearanceSprite* __fastcall CTCGraphicAppearanceSprite_Alloc(CThing* owner)
{
    return new CTCGraphicAppearanceSprite(owner);
}