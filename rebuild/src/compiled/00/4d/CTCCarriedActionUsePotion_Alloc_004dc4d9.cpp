#pragma optimize("s",on)
// CTCCarriedActionUsePotion::Alloc(CThing&) -- factory: new CTCCarriedActionUsePotion(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC4D9, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUsePotion::CTCCarriedActionUsePotion ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCCarriedActionUsePotion) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DADF5 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCCarriedActionUsePotion {
    unsigned char _body[0x14];       // sizeof(CTCCarriedActionUsePotion) = 0x14
    CTCCarriedActionUsePotion(void* owner);                   // 0x004DADF5
};

CTCCarriedActionUsePotion* __fastcall CTCCarriedActionUsePotion_Alloc(void* owner)
{
    return new CTCCarriedActionUsePotion(owner);
}