// Create_CModeCrouch(CThing*) -- factory: new CModeCrouch(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073EC10, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x84 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeCrouch::CModeCrouch ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CModeCrouch) = 0x84 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0073E410 on it with the
// argument, returning the new object (or null). One of 18 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CModeCrouch {
    unsigned char _body[0x84];       // sizeof(CModeCrouch) = 0x84
    CModeCrouch(CThing* owner);                   // 0x0073E410
};

CModeCrouch* __fastcall Create_CModeCrouch(CThing* owner)
{
    return new CModeCrouch(owner);
}