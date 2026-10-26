#pragma optimize("s",on)
// CTCActionUseReadable::Alloc(CThing&) -- factory: new CTCActionUseReadable(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC004, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x1c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseReadable::CTCActionUseReadable ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCActionUseReadable) = 0x1c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DA9D7 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCActionUseReadable { public:
    unsigned char _body[0x1c];       // sizeof(CTCActionUseReadable) = 0x1c
    CTCActionUseReadable(CThing* owner);                   // 0x004DA9D7
};

CTCActionUseReadable* __fastcall CTCActionUseReadable_Alloc(CThing* owner)
{
    return new CTCActionUseReadable(owner);
}