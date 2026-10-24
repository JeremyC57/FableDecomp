// PKCTRRedblackTree_00c18df0::CountNodes(PKCTRRedblackNode_00c18df0* node, long* counter) -- adds the number of nodes of the subtree at node to *counter (left subtree recursively, then the node, then along the right spine)
// Leaf: CountNodes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C18DF0, 48 bytes, __fastcall `ret 8`.
// Identity: the libpk audio library's red-black tree (the landed constructor 0x00C194F0 / InsertFixup 0x00C18B20 model:
//   vtable 0x012B2F0C, node allocator +4, owned allocator +8, nil sentinel +0xc, root +0x20, count +0x24; node { left,
//   right, parent, colour, key +0x10 }). No PDB name set holds a PKCTR symbol, so the classes are named by address.
// Callers: itself (0x00C18E0B) and the public counter @ 0x00C19130 (`long n = 0; CountNodes(Root, &n);`).
// Shape: the right-hand recursion is the loop; `inc dword ptr [edi]` straight on the counter.
// Label correction: the catalog's `Tree_PreorderCount` is descriptive (in-order walk).
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
    PKCTRAllocator_00c18df0*    PAllocator;                    // +0x04
    PKCTRAllocator_00c18df0*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c18df0  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c18df0* Root;                          // +0x20
    long                        Count;                         // +0x24

    void CountNodes(PKCTRRedblackNode_00c18df0* node, long* counter);
};

void PKCTRRedblackTree_00c18df0::CountNodes(PKCTRRedblackNode_00c18df0* node, long* counter)
{
    while (node != &Sentinel) {
        CountNodes(node->Left, counter);
        ++*counter;
        node = node->Right;
    }
}