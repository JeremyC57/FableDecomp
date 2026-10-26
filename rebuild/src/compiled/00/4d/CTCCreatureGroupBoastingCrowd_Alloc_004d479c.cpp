#pragma optimize("s",on)
// CTCCreatureGroupBoastingCrowd::Alloc(CThing&) -- factory: new CTCCreatureGroupBoastingCrowd(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D479C, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x48 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureGroupBoastingCrowd::CTCCreatureGroupBoastingCrowd ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCCreatureGroupBoastingCrowd) = 0x48 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00784B30 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCCreatureGroupBoastingCrowd { public:
    unsigned char _body[0x48];       // sizeof(CTCCreatureGroupBoastingCrowd) = 0x48
    CTCCreatureGroupBoastingCrowd(CThing* owner);                   // 0x00784B30
};

CTCCreatureGroupBoastingCrowd* __fastcall CTCCreatureGroupBoastingCrowd_Alloc(CThing* owner)
{
    return new CTCCreatureGroupBoastingCrowd(owner);
}