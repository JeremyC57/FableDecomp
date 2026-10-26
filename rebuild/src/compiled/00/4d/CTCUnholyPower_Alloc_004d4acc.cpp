#pragma optimize("s",on)
// CTCUnholyPower::Alloc(CThing&) -- factory: new CTCUnholyPower(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4ACC, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xd0 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCUnholyPower::CTCUnholyPower ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCUnholyPower) = 0xd0 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007941D0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCUnholyPower { public:
    unsigned char _body[0xd0];       // sizeof(CTCUnholyPower) = 0xd0
    CTCUnholyPower(CThing* owner);                   // 0x007941D0
};

CTCUnholyPower* __fastcall CTCUnholyPower_Alloc(CThing* owner)
{
    return new CTCUnholyPower(owner);
}