// Create_CTCSpotLight(CThing*) -- factory: new CTCSpotLight(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D33DC, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSpotLight::CTCSpotLight ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCSpotLight) = 0x2c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00731D40 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCSpotLight {
    unsigned char _body[0x2c];       // sizeof(CTCSpotLight) = 0x2c
    CTCSpotLight(CThing* owner);                   // 0x00731D40
};

CTCSpotLight* __fastcall Create_CTCSpotLight(CThing* owner)
{
    return new CTCSpotLight(owner);
}
