#pragma optimize("s",on)
// CTCBoastUI::Alloc(CThing&) -- factory: new CTCBoastUI(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5FAB, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBoastUI::CTCBoastUI ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCBoastUI) = 0x2c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x0056AC10 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCBoastUI {
    unsigned char _body[0x2c];       // sizeof(CTCBoastUI) = 0x2c
    CTCBoastUI(long owner);                   // 0x0056AC10
};

CTCBoastUI* __fastcall CTCBoastUI_Alloc(long owner)
{
    return new CTCBoastUI(owner);
}