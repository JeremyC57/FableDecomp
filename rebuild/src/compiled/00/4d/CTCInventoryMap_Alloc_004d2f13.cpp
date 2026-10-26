#pragma optimize("s",on)
// CTCInventoryMap::Alloc(CThing&) -- factory: new CTCInventoryMap(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2F13, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x258 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCInventoryMap::CTCInventoryMap ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCInventoryMap) = 0x258 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00606D20 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCInventoryMap { public:
    unsigned char _body[0x258];       // sizeof(CTCInventoryMap) = 0x258
    CTCInventoryMap(CThing* owner);                   // 0x00606D20
};

CTCInventoryMap* __fastcall CTCInventoryMap_Alloc(CThing* owner)
{
    return new CTCInventoryMap(owner);
}