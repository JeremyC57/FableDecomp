// PKCTRRedblackTree_00c18b20::InsertFixup(PKCTRRedblackNode_00c18b20* x) -- restore the red-black properties after inserting node x (recolour on a red uncle, otherwise rotate), then blacken the root
// Leaf: InsertFixup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C18B20, length 223 bytes (+ int3 padding), `ret 4`.
// Identity: the libpk audio library's red-black tree (".\PKCTRRedblackTree.cpp" in the insert @ 0x00C18E20, the only
//   caller). The object layout follows the landed constructor 0x00C194F0 (0x28 bytes: vtable 0x012B2F0C, the node allocator
//   +4, the owned allocator +8, the embedded nil sentinel node +0xc, the root +0x20 initialised to &sentinel, the count +0x24).
//   The node is { left +0, right +4, parent +8, colour +0xc (0 black, 1 red), key +0x10 }. The body is the textbook
//   (Niemann) insertFixup. The catalog label `_Tree<_Tmap_traits<long,CBlendedParticleEffectSet,...>>::_Insert` is
//   propagated; no PDB name set holds a PKCTR symbol, so the classes are named by address.
// Callees: RotateLeft @ 0x00C18A80 and RotateRight @ 0x00C18AD0 (thiscall, `ret 4`; the manifest's List_RemoveNode /
//   List_RemoveIntNode labels are wrong).
// Shape: the rotations are compiled earlier in the same TU (`#pragma auto_inline(off)` bodies above), so VC7.1 knows they
//   leave ecx alone and never reloads `this` after the calls; RED/BLACK ride in ebx/ebp; the two uncle-is-red arms are
//   tail-merged (the mirror arm jumps into the first).
// Layout: local model (no engine header; the tree is not in the PDB type set).
struct PKCTRRedblackNode_00c18b20 {
    PKCTRRedblackNode_00c18b20* Left;      // +0x00
    PKCTRRedblackNode_00c18b20* Right;     // +0x04
    PKCTRRedblackNode_00c18b20* Parent;    // +0x08
    long                        Colour;    // +0x0c
    void*                       Key;       // +0x10
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

struct PKCTRAllocator_00c18b20;

class PKCTRRedblackTree_00c18b20 {
public:
    virtual ~PKCTRRedblackTree_00c18b20();                     // vtable 0x012B2F0C
    PKCTRAllocator_00c18b20*    PAllocator;                    // +0x04
    PKCTRAllocator_00c18b20*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c18b20  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c18b20* Root;                          // +0x20
    long                        Count;                         // +0x24

    void RotateLeft(PKCTRRedblackNode_00c18b20* x);            // 0x00C18A80
    void RotateRight(PKCTRRedblackNode_00c18b20* x);           // 0x00C18AD0
    void InsertFixup(PKCTRRedblackNode_00c18b20* x);
};

#pragma auto_inline(off)
void PKCTRRedblackTree_00c18b20::RotateLeft(PKCTRRedblackNode_00c18b20* x)
{
    PKCTRRedblackNode_00c18b20* y = x->Right;
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

void PKCTRRedblackTree_00c18b20::RotateRight(PKCTRRedblackNode_00c18b20* x)
{
    PKCTRRedblackNode_00c18b20* y = x->Left;
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
#pragma auto_inline(on)

void PKCTRRedblackTree_00c18b20::InsertFixup(PKCTRRedblackNode_00c18b20* x)
{
    while (x != Root && x->Parent->Colour == PKCTR_RED) {
        if (x->Parent == x->Parent->Parent->Left) {
            PKCTRRedblackNode_00c18b20* y = x->Parent->Parent->Right;
            if (y->Colour == PKCTR_RED) {
                x->Parent->Colour = PKCTR_BLACK;
                y->Colour = PKCTR_BLACK;
                x->Parent->Parent->Colour = PKCTR_RED;
                x = x->Parent->Parent;
            } else {
                if (x == x->Parent->Right) {
                    x = x->Parent;
                    RotateLeft(x);
                }
                x->Parent->Colour = PKCTR_BLACK;
                x->Parent->Parent->Colour = PKCTR_RED;
                RotateRight(x->Parent->Parent);
            }
        } else {
            PKCTRRedblackNode_00c18b20* y = x->Parent->Parent->Left;
            if (y->Colour == PKCTR_RED) {
                x->Parent->Colour = PKCTR_BLACK;
                y->Colour = PKCTR_BLACK;
                x->Parent->Parent->Colour = PKCTR_RED;
                x = x->Parent->Parent;
            } else {
                if (x == x->Parent->Left) {
                    x = x->Parent;
                    RotateRight(x);
                }
                x->Parent->Colour = PKCTR_BLACK;
                x->Parent->Parent->Colour = PKCTR_RED;
                RotateLeft(x->Parent->Parent);
            }
        }
    }
    Root->Colour = PKCTR_BLACK;
}