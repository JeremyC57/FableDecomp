// Create_CTCVillage(CThing*) -- factory: new CTCVillage(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3075, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x230 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCVillage::CTCVillage ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCVillage) = 0x230 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0059EB0C on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CTCVillage {
    unsigned char _body[0x230];       // sizeof(CTCVillage) = 0x230
    CTCVillage(CThing* owner);                   // 0x0059EB0C
};

CTCVillage* __fastcall Create_CTCVillage(CThing* owner)
{
    return new CTCVillage(owner);
}
