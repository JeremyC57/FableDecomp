#pragma optimize("s",on)
// CTCCarriedActionUseGiveBeer::Alloc(CThing&) -- factory: new CTCCarriedActionUseGiveBeer(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC3BC, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseGiveBeer::CTCCarriedActionUseGiveBeer ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCCarriedActionUseGiveBeer) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DAD2A on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCCarriedActionUseGiveBeer {
    unsigned char _body[0x14];       // sizeof(CTCCarriedActionUseGiveBeer) = 0x14
    CTCCarriedActionUseGiveBeer(void* owner);                   // 0x004DAD2A
};

CTCCarriedActionUseGiveBeer* __fastcall CTCCarriedActionUseGiveBeer_Alloc(void* owner)
{
    return new CTCCarriedActionUseGiveBeer(owner);
}