#pragma optimize("s",on)
// CTCElectrocuteOnTouch::Alloc(CThing&) -- factory: new CTCElectrocuteOnTouch(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D6039, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCElectrocuteOnTouch::CTCElectrocuteOnTouch ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCElectrocuteOnTouch) = 0xc bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007E25E0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

class CTCElectrocuteOnTouch { public:
    unsigned char _body[0xc];       // sizeof(CTCElectrocuteOnTouch) = 0xc
    CTCElectrocuteOnTouch(void* owner);                   // 0x007E25E0
};

CTCElectrocuteOnTouch* __fastcall CTCElectrocuteOnTouch_Alloc(void* owner)
{
    return new CTCElectrocuteOnTouch(owner);
}