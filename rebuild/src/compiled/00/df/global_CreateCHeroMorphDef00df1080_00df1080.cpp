// Create_CHeroMorphDef(a, b) -- __fastcall factory: new CHeroMorphDef(a, b) with the constructor @ 0x00DEC7F0 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF1080, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x12c ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; push edi ; mov ecx,eax ; call CHeroMorphDef::CHeroMorphDef ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// A __fastcall factory (first argument in ecx, second in edx): allocates sizeof(CHeroMorphDef) = 0x12c
// bytes (operator new @ 0x00BFEA1A), null-checks the block and runs the two-argument
// constructor @ 0x00DEC7F0 on it, returning the new object (or null). The argument types are
// not recoverable from the body; `void*` models them. One of 10 bodies of this shape.
struct CHeroMorphDef {
    unsigned char _body[0x12c];
    CHeroMorphDef(void* first, void* second);     // 0x00DEC7F0
};

CHeroMorphDef* __fastcall Create_CHeroMorphDef_00df1080(void* first, void* second)
{
    return new CHeroMorphDef(first, second);
}