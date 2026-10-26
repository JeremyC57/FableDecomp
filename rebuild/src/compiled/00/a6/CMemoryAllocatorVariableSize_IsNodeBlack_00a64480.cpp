// CMemoryAllocatorVariableSize::IsNodeBlack(const CMemoryArea*) -- red-black tree colour test: a null (leaf) node or a node whose Colour is 1 (black)
// Leaf: IsNodeBlack
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A64480, 27 bytes, `ret 4`; `this` (ecx) is never read. Catalogued FUN_00a64480;
//   named from Ego_r.pdb `?IsNodeBlack@CMemoryAllocatorVariableSize@@IBE_NPBVCMemoryArea@1@@Z` (protected const,
//   one const CMemoryArea* argument) and the body: CMemoryArea::Colour is the uchar at +0x1C of the 32-byte tree node
//   (PNextArea, PRightChild, PPreviousArea, PParentArea, PLeftChild, PParentChunk, DataSize, Colour, ID).
// Shape: `test eax,eax ; je black ; cmp byte [eax+1Ch],1 ; je black ; xor eax,eax ; ret 4 ; black: mov eax,1 ; ret 4`.
class CMemoryAllocatorVariableSize { public:
    class CMemoryAreaChunk;
    class CMemoryArea { public:
        CMemoryArea*      PNextArea;             // +0x00
        CMemoryArea*      PRightChild;           // +0x04
        CMemoryArea*      PPreviousArea;         // +0x08
        CMemoryArea*      PParentArea;           // +0x0C
        CMemoryArea*      PLeftChild;            // +0x10
        CMemoryAreaChunk* PParentChunk;          // +0x14
        long              DataSize;              // +0x18
        unsigned char     Colour;                // +0x1C (1 = black)
        unsigned char     ID;                    // +0x1D
    };
    int IsNodeBlack(const CMemoryArea* node) const;
};

int CMemoryAllocatorVariableSize::IsNodeBlack(const CMemoryArea* node) const
{
    if (node == 0 || node->Colour == 1)
        return 1;
    return 0;
}