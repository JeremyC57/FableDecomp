// PKCTRRedblackMap::Remove(const unsigned long* key) -- remove the key's node from libpk's keyed red-black map (the last-found cache first, else a binary search; 3 when absent): splice out the node or its in-order successor, copy the successor's key/value over, rebalance when a black node left (DeleteFixup), hand the spliced node back to the allocator and clear the cache, returning 0
// Leaf: ?Remove@PKCTRRedblackMap@@QAEJPBK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B7D0, 213 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: the libpk audio library's second red-black container (no vtable, node allocator +0, nil sentinel +4, root
//   +0x1c, last-found cache +0x20; node { left, right, parent, colour, key +0x10 (unsigned compare), value +0x14 }); the
//   file manager keeps its open files in one (0x00C1A070). libpk has no PDB, so every name here is DESCRIPTIVE and taken
//   from the landed siblings exactly: ?Remove@PKCTRRedblackMap@@QAEJPBK@Z (the bound spelling for this address),
//   DeleteFixup 0x00C1B560 (landed ?DeleteFixup@PKCTRRedblackMap@@QAEXPAUPKCTRRedblackMapNode@@@Z) and the node allocator's
//   Release 0x00C1B1E0 (landed ?Release@CMapNodeAllocator@@QAEXPAPAUPKCTRRedblackNode@@@Z, vtable 0x012B30EC).
// Callers: the file manager's close paths 0x00C1A4C9, 0x00C1A974.
// Shape: the spliced node y has its address taken (Release(&y)), so VC7.1 homes it in the dead key argument slot
//   (`mov [esp+0Ch],eax` at every successor step). The cache test is its own `LastFound && LastFound->Key == *key` arm
//   that ASSIGNS z (the `z = LastFound; if (!z || z->Key != *key)` form gives z ecx instead of retail's edx); the search
//   loop's equality exit and its normal end share the `z == &Sentinel` test. Release needs a same-TU noinline stand-in
//   body (declaration-only grows the body to 231 bytes: y is no longer known to stay in the frame word).
// Label correction: the catalog's `CGameMap_RemoveByKey` is propagated (no CGameMap here).
struct PKCTRRedblackMapNode {
    PKCTRRedblackMapNode* Left;      // +0x00
    PKCTRRedblackMapNode* Right;     // +0x04
    PKCTRRedblackMapNode* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

struct PKCTRRedblackNode;                       // the node type the landed Release spelling names

class CMapNodeAllocator {                       // vtable 0x012B30EC (landed 0x00C1B1E0 model)
public:
    virtual void Free(void* node);              // slot 0 (0x00C1B320)
    virtual void* Allocate();                   // slot 1 (0x00C1B330)
    __declspec(noinline) void Release(PKCTRRedblackNode** node);   // 0x00C1B1E0
};

#pragma auto_inline(off)
void CMapNodeAllocator::Release(PKCTRRedblackNode** node)   // same-TU stand-in for the landed body @ 0x00C1B1E0
{
    Free(*node);
    *node = 0;
}
#pragma auto_inline(on)

class PKCTRRedblackMap {
public:
    CMapNodeAllocator*    PAllocator;                 // +0x00
    PKCTRRedblackMapNode  Sentinel;                      // +0x04 (0x18 bytes)
    PKCTRRedblackMapNode* Root;                          // +0x1c
    PKCTRRedblackMapNode* LastFound;                     // +0x20 (the lookup cache)

    void DeleteFixup(PKCTRRedblackMapNode* x);                // 0x00C1B560
    long Remove(const unsigned long* key);
};

long PKCTRRedblackMap::Remove(const unsigned long* key)
{
    PKCTRRedblackMapNode* z;
    if (LastFound != 0 && LastFound->Key == *key) {
        z = LastFound;
    } else {
        z = Root;
        while (z != &Sentinel) {
            if (*key == z->Key)
                break;
            if (*key < z->Key)
                z = z->Left;
            else
                z = z->Right;
        }
        if (z == &Sentinel)
            return 3;
    }
    PKCTRRedblackMapNode* y;
    if (z->Left == &Sentinel || z->Right == &Sentinel) {
        y = z;
    } else {
        y = z->Right;
        while (y->Left != &Sentinel)
            y = y->Left;
    }
    PKCTRRedblackMapNode* x;
    if (y->Left != &Sentinel)
        x = y->Left;
    else
        x = y->Right;
    x->Parent = y->Parent;
    if (y->Parent) {
        if (y == y->Parent->Left)
            y->Parent->Left = x;
        else
            y->Parent->Right = x;
    } else {
        Root = x;
    }
    if (y != z) {
        z->Key = y->Key;
        z->Value = y->Value;
    }
    if (y->Colour == PKCTR_BLACK)
        DeleteFixup(x);
    PAllocator->Release((PKCTRRedblackNode**)&y);
    LastFound = 0;
    return 0;
}