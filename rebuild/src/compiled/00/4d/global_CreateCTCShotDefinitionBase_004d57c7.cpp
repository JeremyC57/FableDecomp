#pragma optimize("s",on)
// Create_CTCShotDefinitionBase(CThing*) -- factory: new CTCShotDefinitionBase(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D57C7, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xac ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionBase::CTCShotDefinitionBase ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCShotDefinitionBase) = 0xac bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007CA830 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCShotDefinitionBase {
    unsigned char _body[0xac];       // sizeof(CTCShotDefinitionBase) = 0xac
    CTCShotDefinitionBase(CThing* owner);                   // 0x007CA830
};

CTCShotDefinitionBase* __fastcall Create_CTCShotDefinitionBase(CThing* owner)
{
    return new CTCShotDefinitionBase(owner);
}