// PKCTRRedblackTree_00c18b20::GetFirst() -- the smallest item (the leftmost node's key), 0 for an empty tree
// Leaf: ?GetFirst@PKCTRRedblackTree_00c18b20@@UAEPAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C19150, 39 bytes, thiscall: virtual slot 11 of 0x012B2F0C; the sentinel address
//   is formed in ecx (`add ecx,0Ch`), the walk stops on a sentinel child and the end test is repeated after the loop.
// Identity (descriptive): the libpk audio library's red-black tree (vtable 0x012B2F0C; the landed constructor 0x00C194F0
//   / InsertFixup 0x00C18B20 model: allocator +4, owned allocator +8, nil sentinel +0xc, root +0x20, +0x24 cleared by the
//   constructor; node { left, right, parent, colour, key +0x10 }). No PDB name set holds a PKCTR symbol, so the classes
//   are named by address, as the landed members are.
struct PKCTRRedblackNode_00c19150 {
    PKCTRRedblackNode_00c19150* Left;      // +0x00
    PKCTRRedblackNode_00c19150* Right;     // +0x04
    PKCTRRedblackNode_00c19150* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    void*                       Key;       // +0x10
};

struct PKCTRAllocator_00c19150;

class PKCTRRedblackTree_00c18b20 {
public:
    virtual ~PKCTRRedblackTree_00c18b20();                     // vtable 0x012B2F0C
    virtual void* GetFirst();
    PKCTRAllocator_00c19150*    PAllocator;                    // +0x04
    PKCTRAllocator_00c19150*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c19150  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c19150* Root;                          // +0x20
};

void* PKCTRRedblackTree_00c18b20::GetFirst()
{
    PKCTRRedblackNode_00c19150* node = Root;
    while (node != &Sentinel) {
        if (node->Left == &Sentinel)
            break;
        node = node->Left;
    }
    if (node != &Sentinel)
        return node->Key;
    return 0;
}