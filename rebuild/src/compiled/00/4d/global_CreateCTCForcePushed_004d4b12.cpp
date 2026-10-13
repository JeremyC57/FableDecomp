#pragma optimize("s",on)
// Create_CTCForcePushed(CThing*) -- factory: new CTCForcePushed(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4B12, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x50 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCForcePushed::CTCForcePushed ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCForcePushed) = 0x50 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00798570 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCForcePushed {
    unsigned char _body[0x50];       // sizeof(CTCForcePushed) = 0x50
    CTCForcePushed(CThing* owner);                   // 0x00798570
};

CTCForcePushed* __fastcall Create_CTCForcePushed(CThing* owner)
{
    return new CTCForcePushed(owner);
}