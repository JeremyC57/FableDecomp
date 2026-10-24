// PKCTRRedblackTree_00c18d90::FindNth(PKCTRRedblackNode_00c18d90* node, long index, long* counter) -- in-order index lookup: walks the subtree at node counting keys in *counter and returns the key of the one whose position equals index (0 when the subtree runs out)
// Leaf: FindNth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C18D90, 86 bytes (+ int3 padding), __fastcall `ret 0Ch`.
// Identity: the libpk audio library's red-black tree (the landed constructor 0x00C194F0 / InsertFixup 0x00C18B20 model:
//   vtable 0x012B2F0C, node allocator +4, owned allocator +8, nil sentinel +0xc, root +0x20, count +0x24; node { left,
//   right, parent, colour, key +0x10 }). No PDB name set holds a PKCTR symbol, so the classes are named by address.
// Callers: itself (0x00C18DB7) and the public GetAt(index) @ 0x00C190F0 (`long counter = 0; return FindNth(Root, index,
//   &counter);`, landed as CVtable012b2f0c::GetItemByIndex).
// Shape: written fully recursive -- VC7.1 turns the right-hand tail call into the loop back to the entry test (the
//   sentinel address re-formed there, `lea eax,[ebp+0Ch]`), so the sentinel `return 0` keeps its own epilogue (`xor
//   eax,eax` scheduled among the pops) while the two key returns share the other; the `while` form merges the zero into
//   the found-key exit.
// Label correction: the catalog's `Tree_PreorderFind` is descriptive (in-order index lookup).
struct PKCTRRedblackNode_00c18d90 {
    PKCTRRedblackNode_00c18d90* Left;      // +0x00
    PKCTRRedblackNode_00c18d90* Right;     // +0x04
    PKCTRRedblackNode_00c18d90* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    void*                       Key;       // +0x10
};

struct PKCTRAllocator_00c18d90;

class PKCTRRedblackTree_00c18d90 {
public:
    virtual ~PKCTRRedblackTree_00c18d90();                     // vtable 0x012B2F0C
    PKCTRAllocator_00c18d90*    PAllocator;                    // +0x04
    PKCTRAllocator_00c18d90*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c18d90  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c18d90* Root;                          // +0x20
    long                        Count;                         // +0x24

    void* FindNth(PKCTRRedblackNode_00c18d90* node, long index, long* counter);
};

void* PKCTRRedblackTree_00c18d90::FindNth(PKCTRRedblackNode_00c18d90* node, long index, long* counter)
{
    if (node == &Sentinel)
        return 0;
    void* key = FindNth(node->Left, index, counter);
    if (key)
        return key;
    if (*counter == index)
        return node->Key;
    ++*counter;
    return FindNth(node->Right, index, counter);
}