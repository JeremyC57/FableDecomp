// PKCTRRedblackMap_00c1b200::RotateLeft(PKCTRRedblackNode_00c1b200* x) -- rotate node x left: its right child y takes its place (y->Left becomes x->Right), x becomes y's left child; the root is re-pointed when x had no parent
// Leaf: RotateLeft
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B200, 73 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: the libpk audio library's red-black map (the landed InsertFixup 0x00C1B470 model (no vtable: node allocator +0, nil sentinel +4, root +0x1c, lookup cache +0x20; key +0x10 unsigned, value +0x14)). Callers: its InsertFixup @ 0x00C1B470 and delete fixup;
//   the mirror rotation is @ 0x00C1B250. No PDB name set holds a PKCTR symbol, so the classes are named by address.
// Shape: the textbook (Niemann) rotation: the sentinel address `lea edi,[ecx+4]` is formed once after the first
//   store; a null parent (not the sentinel) means x was the root (`mov [ecx+1Ch],eax`).
// Label correction: the catalog's `List_RemoveNode_Variant2` is wrong (this is a tree rotation).
struct PKCTRRedblackNode_00c1b200 {
    PKCTRRedblackNode_00c1b200* Left;      // +0x00
    PKCTRRedblackNode_00c1b200* Right;     // +0x04
    PKCTRRedblackNode_00c1b200* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};

struct PKCTRAllocator_00c1b200;

class PKCTRRedblackMap_00c1b200 {
public:
    PKCTRAllocator_00c1b200*    PAllocator;                    // +0x00
    PKCTRRedblackNode_00c1b200  Sentinel;                      // +0x04 (0x18 bytes)
    PKCTRRedblackNode_00c1b200* Root;                          // +0x1c
    PKCTRRedblackNode_00c1b200* LastFound;                     // +0x20 (the lookup cache)

    void RotateLeft(PKCTRRedblackNode_00c1b200* x);
};

void PKCTRRedblackMap_00c1b200::RotateLeft(PKCTRRedblackNode_00c1b200* x)
{
    PKCTRRedblackNode_00c1b200* y = x->Right;
    x->Right = y->Left;
    if (y->Left != &Sentinel) y->Left->Parent = x;
    if (y != &Sentinel) y->Parent = x->Parent;
    if (x->Parent) {
        if (x == x->Parent->Left)
            x->Parent->Left = y;
        else
            x->Parent->Right = y;
    } else {
        Root = y;
    }
    y->Left = x;
    if (x != &Sentinel) x->Parent = y;
}