#pragma optimize("s",on)
// CTCActionUseStartExtrapolationMorph::Alloc(CThing&) -- factory: new CTCActionUseStartExtrapolationMorph(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC22D, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseStartExtrapolationMorph::CTCActionUseStartExtrapolationMorph ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCActionUseStartExtrapolationMorph) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DAB94 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCActionUseStartExtrapolationMorph {
    unsigned char _body[0x14];       // sizeof(CTCActionUseStartExtrapolationMorph) = 0x14
    CTCActionUseStartExtrapolationMorph(void* owner);                   // 0x004DAB94
};

CTCActionUseStartExtrapolationMorph* __fastcall CTCActionUseStartExtrapolationMorph_Alloc(void* owner)
{
    return new CTCActionUseStartExtrapolationMorph(owner);
}