#pragma optimize("s",on)
// CTCTavernGameSpotTheAddition::Alloc(CThing&) -- factory: new CTCTavernGameSpotTheAddition(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5A77, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x188 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTavernGameSpotTheAddition::CTCTavernGameSpotTheAddition ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCTavernGameSpotTheAddition) = 0x188 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007D8DA0 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

struct CTCTavernGameSpotTheAddition {
    unsigned char _body[0x188];       // sizeof(CTCTavernGameSpotTheAddition) = 0x188
    CTCTavernGameSpotTheAddition(CThing* owner);                   // 0x007D8DA0
};

CTCTavernGameSpotTheAddition* __fastcall CTCTavernGameSpotTheAddition_Alloc(CThing* owner)
{
    return new CTCTavernGameSpotTheAddition(owner);
}