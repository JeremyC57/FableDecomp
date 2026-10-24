// PKCTRRedblackMap_00c1b470::InsertFixup(PKCTRRedblackNode_00c1b470* x) -- restore the red-black properties after inserting node x (recolour on a red uncle, otherwise rotate), then blacken the root
// Leaf: InsertFixup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B470, length 230 bytes (+ int3 padding), `ret 4`.
// Identity: the second libpk red-black container (the insert @ 0x00C1B720, the only caller, returns 1 on "memory exhausted"
//   and 2 on a duplicate key). Its object has no vtable: the node allocator +0 (virtual Alloc at slot +4), the embedded nil
//   sentinel node +4 (0x18 bytes), the root +0x1c; the node is { left +0, right +4, parent +8, colour +0xc (0 black, 1 red),
//   key +0x10 (unsigned compare), value +0x14 }. Same textbook (Niemann) insertFixup as the twin 0x00C18B20. The catalog label
//   `_Tree<_Tmap_traits<long,CBlendedParticleEffectSet,...>>::_Insert` is propagated; the classes are named by address.
// Callees: RotateLeft @ 0x00C1B200 and RotateRight @ 0x00C1B250 (thiscall, `ret 4`; the manifest's List_RemoveNode_Variant2 /
//   List_RemoveIntNode_Variant2 labels are wrong).
// Shape: unlike the twin, the rotations are NOT known to this function's compile (declared only), so `this` lives in edi and is
//   copied to ecx before every call; ecx is then free for the uncle and edx holds the grandparent.
// Layout: local model (no engine header; the container is not in the PDB type set).
struct PKCTRRedblackNode_00c1b470 {
    PKCTRRedblackNode_00c1b470* Left;      // +0x00
    PKCTRRedblackNode_00c1b470* Right;     // +0x04
    PKCTRRedblackNode_00c1b470* Parent;    // +0x08
    long                        Colour;    // +0x0c
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

struct PKCTRAllocator_00c1b470;

class PKCTRRedblackMap_00c1b470 {
public:
    PKCTRAllocator_00c1b470*    PAllocator;                    // +0x00
    PKCTRRedblackNode_00c1b470  Sentinel;                      // +0x04 (0x18 bytes)
    PKCTRRedblackNode_00c1b470* Root;                          // +0x1c

    void RotateLeft(PKCTRRedblackNode_00c1b470* x);            // 0x00C1B200
    void RotateRight(PKCTRRedblackNode_00c1b470* x);           // 0x00C1B250
    void InsertFixup(PKCTRRedblackNode_00c1b470* x);
};

void PKCTRRedblackMap_00c1b470::InsertFixup(PKCTRRedblackNode_00c1b470* x)
{
    while (x != Root && x->Parent->Colour == PKCTR_RED) {
        if (x->Parent == x->Parent->Parent->Left) {
            PKCTRRedblackNode_00c1b470* y = x->Parent->Parent->Right;
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
            PKCTRRedblackNode_00c1b470* y = x->Parent->Parent->Left;
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