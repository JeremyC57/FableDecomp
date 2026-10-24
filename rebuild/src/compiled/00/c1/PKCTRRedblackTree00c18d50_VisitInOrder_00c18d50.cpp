// PKCTRRedblackTree_00c18d50::VisitInOrder(PKCTRRedblackNode_00c18d50* node, PKCTRVisitor_00c18d50* visitor) -- in-order walk of the subtree at node: the left subtree recursively, then the visitor's first virtual on the node's key, then on along the right spine
// Leaf: VisitInOrder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C18D50, 56 bytes (+ int3 padding), __fastcall `ret 8`.
// Identity: the libpk audio library's red-black tree (the landed constructor 0x00C194F0 / InsertFixup 0x00C18B20 model:
//   vtable 0x012B2F0C, node allocator +4, owned allocator +8, nil sentinel +0xc, root +0x20, count +0x24; node { left,
//   right, parent, colour, key +0x10 }). No PDB name set holds a PKCTR symbol, so the classes are named by address.
// Callers: itself (0x00C18D6B) and the public ForEach @ 0x00C19110 (`push visitor ; push [ecx+20h] ; call`).
// Shape: the right-hand recursion is the loop (`while (node != &Sentinel)`): one entry test, the sentinel address in ebp,
//   the visitor in edi loaded inside the guarded block; the visitor slot 0 takes the key's address (`lea eax,[esi+10h]`).
// Label correction: the catalog's `Tree_PreorderTraverse` is descriptive and wrong about the order (in-order).
struct PKCTRRedblackNode_00c18d50 {
    PKCTRRedblackNode_00c18d50* Left;      // +0x00
    PKCTRRedblackNode_00c18d50* Right;     // +0x04
    PKCTRRedblackNode_00c18d50* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    void*                       Key;       // +0x10
};

struct PKCTRAllocator_00c18d50;

class PKCTRVisitor_00c18d50 {
public:
    virtual void Visit(void** key) = 0;                        // slot 0
};

class PKCTRRedblackTree_00c18d50 {
public:
    virtual ~PKCTRRedblackTree_00c18d50();                     // vtable 0x012B2F0C
    PKCTRAllocator_00c18d50*    PAllocator;                    // +0x04
    PKCTRAllocator_00c18d50*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c18d50  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c18d50* Root;                          // +0x20
    long                        Count;                         // +0x24

    void VisitInOrder(PKCTRRedblackNode_00c18d50* node, PKCTRVisitor_00c18d50* visitor);
};

void PKCTRRedblackTree_00c18d50::VisitInOrder(PKCTRRedblackNode_00c18d50* node, PKCTRVisitor_00c18d50* visitor)
{
    while (node != &Sentinel) {
        VisitInOrder(node->Left, visitor);
        visitor->Visit(&node->Key);
        node = node->Right;
    }
}