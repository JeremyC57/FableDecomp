// NCreatureMode::CModeDrunk::Alloc(CThing&) -- factory: new CModeDrunk(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742BA0, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x74 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeDrunk::CModeDrunk ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CModeDrunk) = 0x74 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00741110 on it with the
// argument, returning the new object (or null). One of 18 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

struct CModeDrunk {
    unsigned char _body[0x74];       // sizeof(CModeDrunk) = 0x74
    CModeDrunk(CThing* owner);                   // 0x00741110
};

CModeDrunk* __fastcall CModeDrunk_Alloc(CThing* owner)
{
    return new CModeDrunk(owner);
}