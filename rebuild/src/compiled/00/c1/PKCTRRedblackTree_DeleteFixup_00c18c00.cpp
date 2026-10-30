// PKCTRRedblackTree::DeleteFixup(PKCTRRedblackNode* x) -- restore the red-black properties after unlinking a black node: walk x up while it is a black non-root, recolouring and rotating around its sibling w (four textbook cases, mirrored for a right child), then blacken x
// Leaf: ?DeleteFixup@PKCTRRedblackTree@@QAEXPAUPKCTRRedblackNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C18C00, 335 bytes, __fastcall, `ret 4`.
// Identity: libpk's red-black tree class, the one compiled from ".\PKCTRRedblackTree.cpp" (the string 0x012B2E98 is
//   referenced only by this class's functions 0x00C18E73..0x00C1955F; libpk names its classes after their files:
//   PKCTRList.h, PKCTRArray.cpp, PKCTRMorphable.cpp). Layout of the landed ctor 0x00C194F0 / InsertFixup 0x00C18B20 (vtable
//   0x012B2F0C, nil sentinel +0x0C, root +0x20). The body is the textbook (Niemann) deleteFixup, the twin of the landed
//   InsertFixup; the method name is descriptive (libpk has no PDB). The manifest label "RedBlackTree_RemoveNode" is a guess.
// Callees: RotateLeft 0x00C18A80, RotateRight 0x00C18AD0 (thiscall `ret 4`; landed as PKCTRRedblackTree_00c18b20 placeholders,
//   NAME_FIXES), compiled above in the same TU like the landed InsertFixup, so VC7.1 knows they keep ecx and never reloads `this`.
// Layout: local model (no engine header; libpk is not in the PDB type set).
struct PKCTRRedblackNode {
    PKCTRRedblackNode* Left;               // +0x00
    PKCTRRedblackNode* Right;              // +0x04
    PKCTRRedblackNode* Parent;             // +0x08
    long               Colour;             // +0x0c (0 black, 1 red)
    void*              Key;                // +0x10
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

struct PKCTRAllocator;

class PKCTRRedblackTree {
public:
    virtual ~PKCTRRedblackTree();                              // vtable 0x012B2F0C
    PKCTRAllocator*    PAllocator;                             // +0x04
    PKCTRAllocator*    POwnedAllocator;                        // +0x08
    PKCTRRedblackNode  Sentinel;                               // +0x0c (0x14 bytes)
    PKCTRRedblackNode* Root;                                   // +0x20
    long               Count;                                  // +0x24

    void RotateLeft(PKCTRRedblackNode* x);                     // 0x00C18A80
    void RotateRight(PKCTRRedblackNode* x);                    // 0x00C18AD0
    void DeleteFixup(PKCTRRedblackNode* x);                    // 0x00C18C00
};

#pragma auto_inline(off)
void PKCTRRedblackTree::RotateLeft(PKCTRRedblackNode* x)
{
    PKCTRRedblackNode* y = x->Right;
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

void PKCTRRedblackTree::RotateRight(PKCTRRedblackNode* x)
{
    PKCTRRedblackNode* y = x->Left;
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

void PKCTRRedblackTree::DeleteFixup(PKCTRRedblackNode* x)
{
    while (x != Root && x->Colour == PKCTR_BLACK) {
        if (x == x->Parent->Left) {
            PKCTRRedblackNode* w = x->Parent->Right;
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
            PKCTRRedblackNode* w = x->Parent->Left;
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