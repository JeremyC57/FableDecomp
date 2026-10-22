#pragma optimize("s",on)
// CTCCarriedActionUseLamp::Alloc(CThing&) -- factory: new CTCCarriedActionUseLamp(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC2D8, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseLamp::CTCCarriedActionUseLamp ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCCarriedActionUseLamp) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DAC82 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCCarriedActionUseLamp {
    unsigned char _body[0x14];       // sizeof(CTCCarriedActionUseLamp) = 0x14
    CTCCarriedActionUseLamp(void* owner);                   // 0x004DAC82
};

CTCCarriedActionUseLamp* __fastcall CTCCarriedActionUseLamp_Alloc(void* owner)
{
    return new CTCCarriedActionUseLamp(owner);
}