// Create_CQ_HangingTreeEvilScript(a, b) -- __fastcall factory: new CQ_HangingTreeEvilScript(a, b) with the constructor @ 0x00D68F00 out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6ED10, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xd0 ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; push edi ; mov ecx,eax ; call CQ_HangingTreeEvilScript::CQ_HangingTreeEvilScript ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// A __fastcall factory (first argument in ecx, second in edx): allocates sizeof(CQ_HangingTreeEvilScript) = 0xd0
// bytes (operator new @ 0x00BFEA1A), null-checks the block and runs the two-argument
// constructor @ 0x00D68F00 on it, returning the new object (or null). The argument types are
// not recoverable from the body; `void*` models them. One of 10 bodies of this shape.
struct CQ_HangingTreeEvilScript {
    unsigned char _body[0xd0];
    CQ_HangingTreeEvilScript(void* first, void* second);     // 0x00D68F00
};

CQ_HangingTreeEvilScript* __fastcall Create_CQ_HangingTreeEvilScript_00d6ed10(void* first, void* second)
{
    return new CQ_HangingTreeEvilScript(first, second);
}
