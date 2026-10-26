#pragma optimize("s",on)
// CTCBoastingPodium::Alloc(CThing&) -- factory: new CTCBoastingPodium(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7E19, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBoastingPodium::CTCBoastingPodium ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCBoastingPodium) = 0x2c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004E7513 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCBoastingPodium { public:
    unsigned char _body[0x2c];       // sizeof(CTCBoastingPodium) = 0x2c
    CTCBoastingPodium(CThing* owner);                   // 0x004E7513
};

CTCBoastingPodium* __fastcall CTCBoastingPodium_Alloc(CThing* owner)
{
    return new CTCBoastingPodium(owner);
}