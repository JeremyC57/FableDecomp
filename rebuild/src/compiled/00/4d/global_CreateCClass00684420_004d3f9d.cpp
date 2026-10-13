#pragma optimize("s",on)
// Create_CClass_00684420(CThing*) -- factory: new CClass_00684420(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3F9D, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x350 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CClass_00684420::CClass_00684420 ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CClass_00684420) = 0x350 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00684420 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CClass_00684420 {
    unsigned char _body[0x350];       // sizeof(CClass_00684420) = 0x350
    CClass_00684420(CThing* owner);                   // 0x00684420
};

CClass_00684420* __fastcall Create_CClass_00684420(CThing* owner)
{
    return new CClass_00684420(owner);
}