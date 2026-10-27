// PKCTRRedblackTree_00c18b20::FindByKey(PKCTRComparator_00c19010* cmp, const void* key) -- binary search by key through a comparator (slots 2 / 3): on a hit remember the node at +0x24 and return its item, else 0
// Leaf: ?FindByKey@PKCTRRedblackTree_00c18b20@@UAEPAXPAVPKCTRComparator_00c19010@@PBX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C19010, 103 bytes, thiscall, `ret 8`: virtual slot 8 of 0x012B2F0C, comparator slots
//   2 / 3 (EqualToKey / LessThanKey), the sentinel address in ebp, this spilled to the `push ecx` slot; 0x00C19080 is the item twin (slots 0 / 1).
// Identity (descriptive): the libpk audio library's red-black tree (vtable 0x012B2F0C; the landed constructor 0x00C194F0
//   / InsertFixup 0x00C18B20 model: allocator +4, owned allocator +8, nil sentinel +0xc, root +0x20, +0x24 cleared by the
//   constructor; node { left, right, parent, colour, key +0x10 }). No PDB name set holds a PKCTR symbol, so the classes
//   are named by address, as the landed members are.
//   +0x24 is named LastFound here: this search stores the hit node there (the landed models call it Count).
struct PKCTRRedblackNode_00c19010 {
    PKCTRRedblackNode_00c19010* Left;      // +0x00
    PKCTRRedblackNode_00c19010* Right;     // +0x04
    PKCTRRedblackNode_00c19010* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    void*                       Key;       // +0x10
};

struct PKCTRAllocator_00c19010;

class PKCTRComparator_00c19010 {               // the item comparator (e.g. CVtable_012b28d8 0x00C155D0..0x00C15640)
public:
    virtual int Equal(const void* a, const void* b);          // slot 0
    virtual int Less(const void* a, const void* b);           // slot 1
    virtual int EqualToKey(const void* key, const void* b);   // slot 2
    virtual int LessThanKey(const void* key, const void* b);  // slot 3
};

class PKCTRRedblackTree_00c18b20 {
public:
    virtual ~PKCTRRedblackTree_00c18b20();                     // vtable 0x012B2F0C
    virtual void* FindByKey(PKCTRComparator_00c19010* cmp, const void* key);
    PKCTRAllocator_00c19010*    PAllocator;                    // +0x04
    PKCTRAllocator_00c19010*    POwnedAllocator;               // +0x08
    PKCTRRedblackNode_00c19010  Sentinel;                      // +0x0c (0x14 bytes)
    PKCTRRedblackNode_00c19010* Root;                          // +0x20
    PKCTRRedblackNode_00c19010* LastFound;                     // +0x24
};

void* PKCTRRedblackTree_00c18b20::FindByKey(PKCTRComparator_00c19010* cmp, const void* key)
{
    PKCTRRedblackNode_00c19010* node = Root;
    while (node != &Sentinel) {
        if (cmp->EqualToKey(key, node->Key)) {
            LastFound = node;
            return node->Key;
        }
        if (cmp->LessThanKey(key, node->Key))
            node = node->Left;
        else
            node = node->Right;
    }
    return 0;
}