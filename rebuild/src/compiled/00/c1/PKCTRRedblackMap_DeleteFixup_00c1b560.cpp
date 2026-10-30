// PKCTRRedblackMap::DeleteFixup(PKCTRRedblackMapNode* x) -- restore the red-black properties after unlinking a black node from libpk's keyed map: walk x up while it is a black non-root, recolouring and rotating around its sibling w (the four textbook cases, mirrored for a right child), then blacken x
// Leaf: ?DeleteFixup@PKCTRRedblackMap@@QAEXPAUPKCTRRedblackMapNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1B560, 334 bytes, __fastcall, `ret 4`.
// Identity: the second libpk red-black container, the one chunk 593 / naming wave 11 spell PKCTRRedblackMap (ctor 0x00C1AE20,
//   Find 0x00C1AE50, InsertFixup 0x00C1B470, remove 0x00C1B7D0): no vtable, node allocator +0, nil sentinel node +4 (0x18
//   bytes: left, right, parent, colour, key +0x10, value +0x14), root +0x1C. The body is the same textbook (Niemann)
//   deleteFixup as PKCTRRedblackTree::DeleteFixup 0x00C18C00; the method name is descriptive (libpk has no PDB). The
//   manifest label "RedBlackTree_RemoveNodeVariant2" is a guess.
// Callees: RotateLeft 0x00C1B200, RotateRight 0x00C1B250 (thiscall `ret 4`; landed as PKCTRRedblackMap_00c1b470
//   placeholders, NAME_FIXES). Unlike the PKCTRRedblackTree twin they are NOT known to this compile (declared only), so
//   `this` lives in edi and is copied to ecx before every call, as in the landed InsertFixup 0x00C1B470.
// Layout: local model (no engine header; libpk is not in the PDB type set).
struct PKCTRRedblackMapNode {
    PKCTRRedblackMapNode* Left;            // +0x00
    PKCTRRedblackMapNode* Right;           // +0x04
    PKCTRRedblackMapNode* Parent;          // +0x08
    long                  Colour;          // +0x0c (0 black, 1 red)
    unsigned long         Key;             // +0x10
    void*                 Value;           // +0x14
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

struct PKCTRAllocator;

class PKCTRRedblackMap {
public:
    PKCTRAllocator*       PAllocator;                          // +0x00
    PKCTRRedblackMapNode  Sentinel;                            // +0x04 (0x18 bytes)
    PKCTRRedblackMapNode* Root;                                // +0x1c

    void RotateLeft(PKCTRRedblackMapNode* x);                  // 0x00C1B200
    void RotateRight(PKCTRRedblackMapNode* x);                 // 0x00C1B250
    void DeleteFixup(PKCTRRedblackMapNode* x);                 // 0x00C1B560
};

void PKCTRRedblackMap::DeleteFixup(PKCTRRedblackMapNode* x)
{
    while (x != Root && x->Colour == PKCTR_BLACK) {
        if (x == x->Parent->Left) {
            PKCTRRedblackMapNode* w = x->Parent->Right;
            if (w->Colour == PKCTR_RED) {
                w->Colour = PKCTR_BLACK;
                x->Parent->Colour = PKCTR_RED;
                RotateLeft(x->Parent);
                w = x->Parent->Right;
            }
            if (w->Left->Colour == PKCTR_BLACK && w->Right->Colour == PKCTR_BLACK) {
                w->Colour = PKCTR_RED;
                x = x->Parent;
            } else {
                if (w->Right->Colour == PKCTR_BLACK) {
                    w->Left->Colour = PKCTR_BLACK;
                    w->Colour = PKCTR_RED;
                    RotateRight(w);
                    w = x->Parent->Right;
                }
                w->Colour = x->Parent->Colour;
                x->Parent->Colour = PKCTR_BLACK;
                w->Right->Colour = PKCTR_BLACK;
                RotateLeft(x->Parent);
                x = Root;
            }
        } else {
            PKCTRRedblackMapNode* w = x->Parent->Left;
            if (w->Colour == PKCTR_RED) {
                w->Colour = PKCTR_BLACK;
                x->Parent->Colour = PKCTR_RED;
                RotateRight(x->Parent);
                w = x->Parent->Left;
            }
            if (w->Right->Colour == PKCTR_BLACK && w->Left->Colour == PKCTR_BLACK) {
                w->Colour = PKCTR_RED;
                x = x->Parent;
            } else {
                if (w->Left->Colour == PKCTR_BLACK) {
                    w->Right->Colour = PKCTR_BLACK;
                    w->Colour = PKCTR_RED;
                    RotateLeft(w);
                    w = x->Parent->Left;
                }
                w->Colour = x->Parent->Colour;
                x->Parent->Colour = PKCTR_BLACK;
                w->Left->Colour = PKCTR_BLACK;
                RotateRight(x->Parent);
                x = Root;
            }
        }
    }
    x->Colour = PKCTR_BLACK;
}