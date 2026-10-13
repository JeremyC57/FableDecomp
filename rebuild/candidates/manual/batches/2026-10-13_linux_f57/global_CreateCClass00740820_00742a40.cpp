// Create_CClass_00740820(CThing*) -- factory: new CClass_00740820(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742A40, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CClass_00740820::CClass_00740820 ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CClass_00740820) = 0x28 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00740820 on it with the
// argument, returning the new object (or null). One of 18 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CClass_00740820 {
    unsigned char _body[0x28];       // sizeof(CClass_00740820) = 0x28
    CClass_00740820(CThing* owner);                   // 0x00740820
};

CClass_00740820* __fastcall Create_CClass_00740820(CThing* owner)
{
    return new CClass_00740820(owner);
}
