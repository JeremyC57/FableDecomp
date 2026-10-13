// Create_CClass_00cf9350(a, b) -- __fastcall factory: new CClass_00cf9350(a, b) with the constructor @ 0x00CF9350 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFA2F0, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x1d0 ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; push edi ; mov ecx,eax ; call CClass_00cf9350::CClass_00cf9350 ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// A __fastcall factory (first argument in ecx, second in edx): allocates sizeof(CClass_00cf9350) = 0x1d0
// bytes (operator new @ 0x00BFEA1A), null-checks the block and runs the two-argument
// constructor @ 0x00CF9350 on it, returning the new object (or null). The argument types are
// not recoverable from the body; `void*` models them. One of 10 bodies of this shape.
struct CClass_00cf9350 {
    unsigned char _body[0x1d0];
    CClass_00cf9350(void* first, void* second);     // 0x00CF9350
};

CClass_00cf9350* __fastcall Create_CClass_00cf9350_00cfa2f0(void* first, void* second)
{
    return new CClass_00cf9350(first, second);
}