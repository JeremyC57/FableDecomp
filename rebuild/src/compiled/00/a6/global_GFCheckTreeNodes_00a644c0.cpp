// GFCheckTreeNodes(CMemoryAllocatorVariableSize::CMemoryArea* node) -- debug walk of the variable-size allocator's free-area red-black tree: recurse into the left then the right child of every node and report the tree as valid
// Leaf: ?GFCheckTreeNodes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A644C0, 35 bytes, plain `ret` (PDB ?GFCheckTreeNodes@@YI_NPAVCMemoryArea@CMemoryAllocatorVariableSize@@@Z:
//   `bool __fastcall`, the node in ecx). Callees: itself (twice).
// Layout: Ego_r.pdb CMemoryAllocatorVariableSize::CMemoryArea (32 bytes): PNextArea +0, PRightChild +4, PPreviousArea +8,
//   PParentArea +0xc, PLeftChild +0x10, PParentChunk +0x14, DataSize +0x18, Colour +0x1c, ID +0x1d. No engine header.
// Shape: the release build keeps only the recursion (the per-node checks compile away): `if (node) { if (left) recurse;
//   if (right) recurse; } return true;` -- the null tests are explicit in the source (the calls skip null children).
class CMemoryAllocatorVariableSize {
public:
    class CMemoryAreaChunk;
    class CMemoryArea {
    public:
        CMemoryArea*      PNextArea;                    // +0x00
        CMemoryArea*      PRightChild;                  // +0x04
        CMemoryArea*      PPreviousArea;                // +0x08
        CMemoryArea*      PParentArea;                  // +0x0c
        CMemoryArea*      PLeftChild;                   // +0x10
        CMemoryAreaChunk* PParentChunk;                 // +0x14
        long              DataSize;                     // +0x18
        unsigned char     Colour;                       // +0x1c
        unsigned char     ID;                           // +0x1d
    };
};

bool __fastcall GFCheckTreeNodes(CMemoryAllocatorVariableSize::CMemoryArea* node)
{
    if (node) {
        if (node->PLeftChild)
            GFCheckTreeNodes(node->PLeftChild);
        if (node->PRightChild)
            GFCheckTreeNodes(node->PRightChild);
    }
    return true;
}