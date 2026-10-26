#pragma optimize("s",on)
// CTCHasName::Alloc(CThing&) -- factory: new CTCHasName(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCF15, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHasName::CTCHasName ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCHasName) = 0x10 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DB543 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCHasName { public:
    unsigned char _body[0x10];       // sizeof(CTCHasName) = 0x10
    CTCHasName(CThing* owner);                   // 0x004DB543
};

CTCHasName* __fastcall CTCHasName_Alloc(CThing* owner)
{
    return new CTCHasName(owner);
}