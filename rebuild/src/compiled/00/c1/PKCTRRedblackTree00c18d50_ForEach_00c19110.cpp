// PKCTRRedblackTree_00c18d50::ForEach(PKCTRVisitor_00c18d50* visitor) -- visit every item in order (VisitInOrder from the root)
// Leaf: ?ForEach@PKCTRRedblackTree_00c18d50@@UAEXPAVPKCTRVisitor_00c18d50@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C19110, 17 bytes, thiscall, `ret 4`: virtual slot 13 of 0x012B2F0C,
//   `push visitor ; push [ecx+20h] ; call 0x00C18D50` with this passed through.
// Identity (descriptive): the libpk audio library's red-black tree (vtable 0x012B2F0C; the landed constructor 0x00C194F0
//   / InsertFixup 0x00C18B20 model: allocator +4, owned allocator +8, nil sentinel +0xc, root +0x20, +0x24 cleared by the
//   constructor; node { left, right, parent, colour, key +0x10 }). No PDB name set holds a PKCTR symbol, so the classes
//   are named by address, as the landed members are.
//   The class is spelled as the landed callee's (VisitInOrder 0x00C18D50) so the call binds to the same symbol.
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
    virtual void ForEach(PKCTRVisitor_00c18d50* visitor);
    PKCTRAllocator_00c18d50*    PAllocator;                    // +0x04
    PKCTRAllocator_00c18d50*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c18d50  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c18d50* Root;                          // +0x20

    void VisitInOrder(PKCTRRedblackNode_00c18d50* node, PKCTRVisitor_00c18d50* visitor);   // 0x00C18D50
};

void PKCTRRedblackTree_00c18d50::ForEach(PKCTRVisitor_00c18d50* visitor)
{
    VisitInOrder(Root, visitor);
}