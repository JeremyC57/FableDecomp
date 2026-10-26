#pragma optimize("s",on)
// CTCHighlightItem::Alloc(CThing&) -- factory: new CTCHighlightItem(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4548, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHighlightItem::CTCHighlightItem ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCHighlightItem) = 0x28 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0077C920 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

struct CTCHighlightItem {
    unsigned char _body[0x28];       // sizeof(CTCHighlightItem) = 0x28
    CTCHighlightItem(CThing* owner);                   // 0x0077C920
};

CTCHighlightItem* __fastcall CTCHighlightItem_Alloc(CThing* owner)
{
    return new CTCHighlightItem(owner);
}