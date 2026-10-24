// PKCTRRedblackMap_00c1b720::Insert(const unsigned long* key, void* const* value) -- inserts a key/value pair: returns 2 when the key is already present, 1 when the node allocator is exhausted, else links a red node under the search parent, rebalances (InsertFixup) and clears the lookup cache, returning 0
// Leaf: Insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B720, 167 bytes (+ int3 padding), __fastcall `ret 8`.
// Identity: the libpk audio library's second red-black container (the landed InsertFixup 0x00C1B470 model: no vtable,
//   the node allocator +0, the nil sentinel +4, the root +0x1c, a last-found cache +0x20; node { left, right, parent,
//   colour, key +0x10 (unsigned compare), value +0x14 }); the file manager keeps its open files in one at +0x20
//   (0x00C1A070). No PDB name set holds a PKCTR symbol, so the classes are named by address.
// Caller: the file manager's open @ 0x00C1A7B4. Callees: the allocator's virtual Alloc (slot 1, no arguments) and
//   InsertFixup @ 0x00C1B470 (landed).
// Shape: Niemann's insertNode: `parent = node` sits between the equality `je` and the ordering `jae`; the new node's
//   fields are stored parent, left, right, colour, key, value (the value pointer loaded first); the two error returns
//   keep their own epilogues ahead of the link-in path.
// Label correction: the catalog's `BTree_InsertNode` is descriptive.
struct PKCTRRedblackNode_00c1b720 {
    PKCTRRedblackNode_00c1b720* Left;      // +0x00
    PKCTRRedblackNode_00c1b720* Right;     // +0x04
    PKCTRRedblackNode_00c1b720* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

class PKCTRAllocator_00c1b720 {                         // the node allocator (vtable 0x012B30EC in the file manager: Free 0x00C1B320, Alloc 0x00C1B330)
public:
    virtual void Free(void* node);                             // slot 0
    virtual void* Alloc();                                     // slot 1: a 0x18-byte node
};

class PKCTRRedblackMap_00c1b720 {
public:
    PKCTRAllocator_00c1b720*    PAllocator;                    // +0x00
    PKCTRRedblackNode_00c1b720  Sentinel;                      // +0x04 (0x18 bytes)
    PKCTRRedblackNode_00c1b720* Root;                          // +0x1c
    PKCTRRedblackNode_00c1b720* LastFound;                     // +0x20 (the lookup cache)

    void InsertFixup(PKCTRRedblackNode_00c1b720* x);                // 0x00C1B470
    long Insert(const unsigned long* key, void* const* value);
};

long PKCTRRedblackMap_00c1b720::Insert(const unsigned long* key, void* const* value)
{
    PKCTRRedblackNode_00c1b720* parent = 0;
    PKCTRRedblackNode_00c1b720* current = Root;
    while (current != &Sentinel) {
        if (*key == current->Key)
            return 2;
        parent = current;
        if (*key < current->Key)
            current = current->Left;
        else
            current = current->Right;
    }
    PKCTRRedblackNode_00c1b720* x = (PKCTRRedblackNode_00c1b720*)PAllocator->Alloc();
    if (x == 0)
        return 1;
    x->Parent = parent;
    x->Left = &Sentinel;
    x->Right = &Sentinel;
    x->Colour = PKCTR_RED;
    x->Key = *key;
    x->Value = *value;
    if (parent) {
        if (*key < parent->Key)
            parent->Left = x;
        else
            parent->Right = x;
    } else {
        Root = x;
    }
    InsertFixup(x);
    LastFound = 0;
    return 0;
}