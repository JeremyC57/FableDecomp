#pragma optimize("s",on)
// CTCBetting::Alloc(CThing&) -- factory: new CTCBetting(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D64FC, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x3c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBetting::CTCBetting ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCBetting) = 0x3c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007E57C0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCBetting { public:
    unsigned char _body[0x3c];       // sizeof(CTCBetting) = 0x3c
    CTCBetting(CThing* owner);                   // 0x007E57C0
};

CTCBetting* __fastcall CTCBetting_Alloc(CThing* owner)
{
    return new CTCBetting(owner);
}