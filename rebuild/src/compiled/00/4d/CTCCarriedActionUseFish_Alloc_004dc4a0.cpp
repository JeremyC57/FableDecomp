#pragma optimize("s",on)
// CTCCarriedActionUseFish::Alloc(CThing&) -- factory: new CTCCarriedActionUseFish(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC4A0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseFish::CTCCarriedActionUseFish ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCCarriedActionUseFish) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DADCD on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCCarriedActionUseFish {
    unsigned char _body[0x14];       // sizeof(CTCCarriedActionUseFish) = 0x14
    CTCCarriedActionUseFish(void* owner);                   // 0x004DADCD
};

CTCCarriedActionUseFish* __fastcall CTCCarriedActionUseFish_Alloc(void* owner)
{
    return new CTCCarriedActionUseFish(owner);
}