// _global::GFCheckBlackHeight(CMemoryAllocatorVariableSize::CMemoryArea*, long) -- red-black tree debug walk: count the black nodes down the left-child chain
// Leaf: GFCheckBlackHeight
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A644A0, 22 bytes, __fastcall (node in ecx, running count in edx), the
//   self-recursion compiled to a tail `jmp` back to the entry. Catalogued FUN_00a644a0; named from Ego_r.pdb
//   `?GFCheckBlackHeight@@YIJPAVCMemoryArea@CMemoryAllocatorVariableSize@@J@Z` (__fastcall, long result) -- its
//   neighbour 0x00A644C0 is GFCheckTreeNodes. CMemoryArea: Colour uchar +0x1C (1 = black), PLeftChild +0x10.
// Shape: a `return GFCheckBlackHeight(left, count)` written directly in the body is turned into a loop by VC7.1
//   (`mov eax,edx ; mov dl,1 ; cmp [ecx+1Ch],dl ; ... jne` back-edge); retail's tail `jmp` to its own entry needs
//   the recursive call inside an inline helper (the null test of the left child and the call), which VC7.1 expands
//   after its tail-recursion pass and then emits as a plain tail call.
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
};

long __fastcall GFCheckBlackHeight(CMemoryAllocatorVariableSize::CMemoryArea* node, long count);

inline bool IsBlack(const CMemoryAllocatorVariableSize::CMemoryArea* node) { return node->Colour == 1; }

inline long CheckBlackHeightBelow(CMemoryAllocatorVariableSize::CMemoryArea* left, long count)
{
    if (left != 0)
        return GFCheckBlackHeight(left, count);
    return count;
}

long __fastcall GFCheckBlackHeight(CMemoryAllocatorVariableSize::CMemoryArea* node, long count)
{
    if (node->Colour == 1)
        ++count;
    return CheckBlackHeightBelow(node->PLeftChild, count);
}