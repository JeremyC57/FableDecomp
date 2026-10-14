#include <new>
// CRetailTree_0067b0b0::BuyNode(const SPair& value) -- allocate a 0x18-byte tree node (malloc @ 0x00BFEA0E) and copy the 8-byte value into it at +0x10 (the node pointer is null-tested through the value's address)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067B0B0, length 48 bytes.
// Leaf: BuyNode
// The chunk label (CMemoryAllocatorVariableSize::NodeAllocate) is propagated. Shape: a `__thiscall` member that never reads `this`, the value address computed with `lea ecx,[eax+10h]` and null-tested, the two dwords copied through esi/edx, `ret 4`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct SPair_0067b0b0 {
    long m_first;
    long m_second;
};
struct SNode_0067b0b0 {                                // 0x18 bytes: links, colour, the value at +0x10
    unsigned char m_links[0x10];
    SPair_0067b0b0   m_value;                         // +0x10
};

struct CRetailTree_0067b0b0 {
    SNode_0067b0b0* BuyNode(const SPair_0067b0b0& value);   // `this` (the tree) is not read
};

SNode_0067b0b0* CRetailTree_0067b0b0::BuyNode(const SPair_0067b0b0& value)
{
    SNode_0067b0b0* node = (SNode_0067b0b0*)malloc(sizeof(SNode_0067b0b0));
    new (&node->m_value) SPair_0067b0b0(value);
    return node;
}
