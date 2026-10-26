#pragma optimize("s",on)
// CTCCupboard::Alloc(CThing&) -- factory: new CTCCupboard(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7F42, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCupboard::CTCCupboard ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCCupboard) = 0x10 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D369B on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCCupboard { public:
    unsigned char _body[0x10];       // sizeof(CTCCupboard) = 0x10
    CTCCupboard(CThing* owner);                   // 0x004D369B
};

CTCCupboard* __fastcall CTCCupboard_Alloc(CThing* owner)
{
    return new CTCCupboard(owner);
}