#pragma optimize("s",on)
// CTCHaste::Alloc(CThing&) -- factory: new CTCHaste(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2B4B, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHaste::CTCHaste ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCHaste) = 0x24 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004E0C61 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCHaste {
    unsigned char _body[0x24];       // sizeof(CTCHaste) = 0x24
    CTCHaste(long owner);                   // 0x004E0C61
};

CTCHaste* __fastcall CTCHaste_Alloc(long owner)
{
    return new CTCHaste(owner);
}