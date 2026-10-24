// PKCTRRedblackMap_00c1b7d0::Remove(const unsigned long* key) -- removes the key's node (the last-found cache first, else a binary search; 3 when absent): Niemann's deleteNode -- splice out the node or its in-order successor, copy the successor's key/value over, rebalance when a black node left (DeleteFixup), hand the spliced node back to the allocator and clear the cache, returning 0
// Grade: DIFFER(213v213) -- ecx/edx register swap only: retail keeps the node z in edx (cache test, search loop, found path) and the key/scratch in ecx; ours swaps them
// Leaf: Remove
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B7D0, 213 bytes (+ int3 padding), __thiscall `ret 4`.
// Identity: the libpk audio library's second red-black container (the landed InsertFixup 0x00C1B470 model: no vtable,
//   the node allocator +0, the nil sentinel +4, the root +0x1c, a last-found cache +0x20; node { left, right, parent,
//   colour, key +0x10 (unsigned compare), value +0x14 }); the file manager keeps its open files in one at +0x20
//   (0x00C1A070). No PDB name set holds a PKCTR symbol, so the classes are named by address.
// Callers: the file manager's close paths 0x00C1A4C9, 0x00C1A974. Callees: DeleteFixup @ 0x00C1B560 and the allocator's
//   Release @ 0x00C1B1E0 (non-virtual: slot-0 Free of *node, then clears it).
// Shape: the spliced node y has its address taken (Release(&y)), so VC7.1 homes it in the dead key argument slot
//   (`mov [esp+0Ch],eax` at every successor step); the cache test reads the cached key first (`cmp ecx,[eax]`); the
//   search loop's equality exit and its normal end share the `z == &Sentinel` test; the found path re-forms the
//   sentinel address (the cache hit skips the search).
// Label correction: the catalog's `CGameMap_RemoveByKey` is propagated (no CGameMap here).
struct PKCTRRedblackNode_00c1b7d0 {
    PKCTRRedblackNode_00c1b7d0* Left;      // +0x00
    PKCTRRedblackNode_00c1b7d0* Right;     // +0x04
    PKCTRRedblackNode_00c1b7d0* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

class PKCTRAllocator_00c1b7d0 {                         // the node allocator
public:
    virtual void Free(void* node);                             // slot 0
    virtual void* Alloc();                                     // slot 1
    __declspec(noinline) void Release(PKCTRRedblackNode_00c1b7d0** node);   // 0x00C1B1E0: Free(*node), then *node = 0
};

#pragma auto_inline(off)
void PKCTRAllocator_00c1b7d0::Release(PKCTRRedblackNode_00c1b7d0** node)  // same-TU stand-in for the retail body @ 0x00C1B1E0
{
    Free(*node);
    *node = 0;
}
#pragma auto_inline(on)

class PKCTRRedblackMap_00c1b7d0 {
public:
    PKCTRAllocator_00c1b7d0*    PAllocator;                    // +0x00
    PKCTRRedblackNode_00c1b7d0  Sentinel;                      // +0x04 (0x18 bytes)
    PKCTRRedblackNode_00c1b7d0* Root;                          // +0x1c
    PKCTRRedblackNode_00c1b7d0* LastFound;                     // +0x20 (the lookup cache)

    void DeleteFixup(PKCTRRedblackNode_00c1b7d0* x);                // 0x00C1B560
    long Remove(const unsigned long* key);
};

long PKCTRRedblackMap_00c1b7d0::Remove(const unsigned long* key)
{
    PKCTRRedblackNode_00c1b7d0* z = LastFound;
    if (z == 0 || z->Key != *key) {
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
    PKCTRRedblackNode_00c1b7d0* y;
    if (z->Left == &Sentinel || z->Right == &Sentinel) {
        y = z;
    } else {
        y = z->Right;
        while (y->Left != &Sentinel)
            y = y->Left;
    }
    PKCTRRedblackNode_00c1b7d0* x;
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
    PAllocator->Release(&y);
    LastFound = 0;
    return 0;
}
