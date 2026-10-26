#pragma optimize("s",on)
// CTCSleep::Alloc(CThing&) -- factory: new CTCSleep(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D9367, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSleep::CTCSleep ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCSleep) = 0x20 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D60E5 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

class CTCSleep { public:
    unsigned char _body[0x20];       // sizeof(CTCSleep) = 0x20
    CTCSleep(long owner);                   // 0x004D60E5
};

CTCSleep* __fastcall CTCSleep_Alloc(long owner)
{
    return new CTCSleep(owner);
}