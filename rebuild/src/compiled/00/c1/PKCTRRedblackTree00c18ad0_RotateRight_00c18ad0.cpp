// PKCTRRedblackTree_00c18ad0::RotateRight(PKCTRRedblackNode_00c18ad0* x) -- rotate node x right: its left child y takes its place (y->Right becomes x->Left), x becomes y's right child; the root is re-pointed when x had no parent
// Leaf: RotateRight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C18AD0, 75 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: the libpk audio library's red-black tree (the landed constructor 0x00C194F0 / InsertFixup 0x00C18B20 model (vtable 0x012B2F0C, node allocator +4, owned allocator +8, nil sentinel +0xc, root +0x20, count +0x24)). Callers: its InsertFixup @ 0x00C18B20 and delete fixup;
//   the mirror rotation is @ 0x00C18A80. No PDB name set holds a PKCTR symbol, so the classes are named by address.
// Shape: the textbook (Niemann) rotation: the sentinel address `lea edi,[ecx+0Ch]` is formed once after the first
//   store; a null parent (not the sentinel) means x was the root (`mov [ecx+20h],eax`).
// Label correction: the catalog's `List_RemoveIntNode` is wrong (this is a tree rotation).
struct PKCTRRedblackNode_00c18ad0 {
    PKCTRRedblackNode_00c18ad0* Left;      // +0x00
    PKCTRRedblackNode_00c18ad0* Right;     // +0x04
    PKCTRRedblackNode_00c18ad0* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    void*                       Key;       // +0x10
};

struct PKCTRAllocator_00c18ad0;

class PKCTRRedblackTree_00c18ad0 {
public:
    virtual ~PKCTRRedblackTree_00c18ad0();                     // vtable 0x012B2F0C
    PKCTRAllocator_00c18ad0*    PAllocator;                    // +0x04
    PKCTRAllocator_00c18ad0*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c18ad0  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c18ad0* Root;                          // +0x20
    long                        Count;                         // +0x24

    void RotateRight(PKCTRRedblackNode_00c18ad0* x);
};

void PKCTRRedblackTree_00c18ad0::RotateRight(PKCTRRedblackNode_00c18ad0* x)
{
    PKCTRRedblackNode_00c18ad0* y = x->Left;
    x->Left = y->Right;
    if (y->Right != &Sentinel) y->Right->Parent = x;
    if (y != &Sentinel) y->Parent = x->Parent;
    if (x->Parent) {
        if (x == x->Parent->Right)
            x->Parent->Right = y;
        else
            x->Parent->Left = y;
    } else {
        Root = y;
    }
    y->Right = x;
    if (x != &Sentinel) x->Parent = y;
}