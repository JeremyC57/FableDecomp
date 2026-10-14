// Distance(const SIterator&, const SIterator&) -- count the __cdecl successor steps @ 0x004292C0 from first to last (size TU)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004753B1, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[first] ; mov ecx,[last] ; mov eax,[eax] ; push esi ; mov esi,[ecx] ; push edi ; xor edi,edi ; jmp T ; L: push eax ; call Successor ; pop ecx ; inc edi ; T: cmp eax,esi ; jne L ; mov eax,edi ; pop edi ; pop esi ; ret
// Bidirectional-iterator distance for a tree: both iterators' nodes are loaded first, the end
// node cached, and the loop advances through the __cdecl successor helper @ 0x004292C0, counting
// after each step; the size TU cleans the argument with `pop ecx`. The manifest label is
// propagated; names are descriptive. One of 8 bodies of this shape.
#pragma optimize("s", on)

struct SNode;
SNode* __cdecl Successor(SNode* node);      // 0x004292C0

struct SIterator {
    SNode* m_node;
};

long __cdecl Distance_004753b1(const SIterator& first, const SIterator& last)
{
    SNode* node = first.m_node;
    SNode* end = last.m_node;
    long n = 0;
    while (node != end) {
        node = Successor(node);
        ++n;
    }
    return n;
}
