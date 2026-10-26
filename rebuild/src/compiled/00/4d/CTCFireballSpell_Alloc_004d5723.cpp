#pragma optimize("s",on)
// CTCFireballSpell::Alloc(CThing&) -- factory: new CTCFireballSpell(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5723, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x80 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFireballSpell::CTCFireballSpell ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCFireballSpell) = 0x80 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007C87F0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCFireballSpell { public:
    unsigned char _body[0x80];       // sizeof(CTCFireballSpell) = 0x80
    CTCFireballSpell(CThing* owner);                   // 0x007C87F0
};

CTCFireballSpell* __fastcall CTCFireballSpell_Alloc(CThing* owner)
{
    return new CTCFireballSpell(owner);
}