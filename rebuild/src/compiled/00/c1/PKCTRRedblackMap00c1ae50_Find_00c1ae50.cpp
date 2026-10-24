// PKCTRRedblackMap_00c1ae50::Find(const unsigned long* key, void** value) -- binary search for the key: on a hit stores the node's value, caches the node as the last found and returns 0; returns 3 when the key is absent
// Leaf: Find
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AE50, 67 bytes (+ int3 padding), __fastcall `ret 8`.
// Identity: the libpk audio library's second red-black container (the landed InsertFixup 0x00C1B470 model: no vtable,
//   the node allocator +0, the nil sentinel +4, the root +0x1c, a last-found cache +0x20; node { left, right, parent,
//   colour, key +0x10 (unsigned compare), value +0x14 }); the file manager keeps its open files in one at +0x20
//   (0x00C1A070). No PDB name set holds a PKCTR symbol, so the classes are named by address.
// Callers: the file manager's lookups 0x00C1A0A5 (GetFileSize), 0x00C1A149, 0x00C1A1E6, 0x00C1A3B0, 0x00C1A8B0.
// Shape: the key is read once inside the non-empty guard (no stores in the loop); `jae` = unsigned `key < node->Key`
//   goes left; the not-found `return 3` is shared by the empty tree and the end of the walk.
// Label correction: the catalog's `BTree_Find` is descriptive.
struct PKCTRRedblackNode_00c1ae50 {
    PKCTRRedblackNode_00c1ae50* Left;      // +0x00
    PKCTRRedblackNode_00c1ae50* Right;     // +0x04
    PKCTRRedblackNode_00c1ae50* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

struct PKCTRAllocator_00c1ae50;

class PKCTRRedblackMap_00c1ae50 {
public:
    PKCTRAllocator_00c1ae50*    PAllocator;                    // +0x00
    PKCTRRedblackNode_00c1ae50  Sentinel;                      // +0x04 (0x18 bytes)
    PKCTRRedblackNode_00c1ae50* Root;                          // +0x1c
    PKCTRRedblackNode_00c1ae50* LastFound;                     // +0x20 (the lookup cache)

    long Find(const unsigned long* key, void** value);
};

long PKCTRRedblackMap_00c1ae50::Find(const unsigned long* key, void** value)
{
    PKCTRRedblackNode_00c1ae50* node = Root;
    while (node != &Sentinel) {
        if (*key == node->Key) {
            *value = node->Value;
            LastFound = node;
            return 0;
        }
        if (*key < node->Key)
            node = node->Left;
        else
            node = node->Right;
    }
    return 3;
}