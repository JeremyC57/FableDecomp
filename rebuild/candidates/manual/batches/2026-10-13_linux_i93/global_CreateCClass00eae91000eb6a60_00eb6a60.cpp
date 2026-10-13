// Create_CClass_00eae910(a, b) -- __fastcall factory: new CClass_00eae910(a, b) with the constructor @ 0x00EAE910 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB6A60, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x88 ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; push edi ; mov ecx,eax ; call CClass_00eae910::CClass_00eae910 ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// A __fastcall factory (first argument in ecx, second in edx): allocates sizeof(CClass_00eae910) = 0x88
// bytes (operator new @ 0x00BFEA1A), null-checks the block and runs the two-argument
// constructor @ 0x00EAE910 on it, returning the new object (or null). The argument types are
// not recoverable from the body; `void*` models them. One of 10 bodies of this shape.
struct CClass_00eae910 {
    unsigned char _body[0x88];
    CClass_00eae910(void* first, void* second);     // 0x00EAE910
};

CClass_00eae910* __fastcall Create_CClass_00eae910_00eb6a60(void* first, void* second)
{
    return new CClass_00eae910(first, second);
}
