// PKCTRRedblackTree_00c18df0::GetCount() -- number of items in the tree, counted by an in-order walk from the root
// Leaf: ?GetCount@PKCTRRedblackTree_00c18df0@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C19130, 27 bytes, thiscall: virtual slot 10 of 0x012B2F0C; the counter lives in
//   the `push ecx` slot and is cleared after the argument pushes.
// Identity (descriptive): the libpk audio library's red-black tree (vtable 0x012B2F0C; the landed constructor 0x00C194F0
//   / InsertFixup 0x00C18B20 model: allocator +4, owned allocator +8, nil sentinel +0xc, root +0x20, +0x24 cleared by the
//   constructor; node { left, right, parent, colour, key +0x10 }). No PDB name set holds a PKCTR symbol, so the classes
//   are named by address, as the landed members are.
//   The class is spelled as the landed callee's (CountNodes 0x00C18DF0) so the call binds to the same symbol.
struct PKCTRRedblackNode_00c18df0 {
    PKCTRRedblackNode_00c18df0* Left;      // +0x00
    PKCTRRedblackNode_00c18df0* Right;     // +0x04
    PKCTRRedblackNode_00c18df0* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    void*                       Key;       // +0x10
};

struct PKCTRAllocator_00c18df0;

class PKCTRRedblackTree_00c18df0 {
public:
    virtual ~PKCTRRedblackTree_00c18df0();                     // vtable 0x012B2F0C
    virtual long GetCount();
    PKCTRAllocator_00c18df0*    PAllocator;                    // +0x04
    PKCTRAllocator_00c18df0*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c18df0  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c18df0* Root;                          // +0x20

    void CountNodes(PKCTRRedblackNode_00c18df0* node, long* counter);   // 0x00C18DF0
};

long PKCTRRedblackTree_00c18df0::GetCount()
{
    long count = 0;
    CountNodes(Root, &count);
    return count;
}