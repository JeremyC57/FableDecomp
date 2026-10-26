#pragma optimize("s",on)
// CTCInventoryMagic::Alloc(CThing&) -- factory: new CTCInventoryMagic(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2F46, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x140 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCInventoryMagic::CTCInventoryMagic ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCInventoryMagic) = 0x140 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0072DB90 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCInventoryMagic { public:
    unsigned char _body[0x140];       // sizeof(CTCInventoryMagic) = 0x140
    CTCInventoryMagic(CThing* owner);                   // 0x0072DB90
};

CTCInventoryMagic* __fastcall CTCInventoryMagic_Alloc(CThing* owner)
{
    return new CTCInventoryMagic(owner);
}