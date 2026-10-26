#pragma optimize("s",on)
// CTCInventoryWeapons::Alloc(CThing&) -- factory: new CTCInventoryWeapons(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2EBA, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x1a0 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCInventoryWeapons::CTCInventoryWeapons ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCInventoryWeapons) = 0x1a0 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x005C3947 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCInventoryWeapons { public:
    unsigned char _body[0x1a0];       // sizeof(CTCInventoryWeapons) = 0x1a0
    CTCInventoryWeapons(CThing* owner);                   // 0x005C3947
};

CTCInventoryWeapons* __fastcall CTCInventoryWeapons_Alloc(CThing* owner)
{
    return new CTCInventoryWeapons(owner);
}