// CTCRegionFollower::CTCRegionFollower(CThing* parent) -- CTCBase(parent), own vtable, the Followers list (list<CIntelligentPointer<CThing>>) default-constructed: a self-linked 0x10-byte sentinel node
// Leaf: ??0CTCRegionFollower
// Retail: Fable.exe @ 0x006AED80, 50 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the STLport node allocator).
// Label: the manifest's "CTextContainer::CTextContainer" is propagated -- the vtable stored, 0x0125D234, is
// CTCRegionFollower in rtti-vtables.tsv; PDB CTCRegionFollower holds only Followers at +0xc.
// Shape: the inline list constructor runs after the derived vtable store (member init): the head is nulled, the
// sentinel allocated and linked to itself, then stored. The retail list is the 4-byte STLport one (PDB: 12 bytes).
class CThing;
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

// The retail (STLport) std::list: one sentinel-node pointer, nulled by the alloc-proxy initialiser, then a node is
// allocated (allocate(1) -> malloc) and linked to itself before it is stored (see the landed 0x00835370 shape).
template <class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;                                    // +0x00
    CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct CTCBase_Calls {                             // CTCBase, 0xc bytes
    virtual void v00();
    unsigned char _body[0x8];                      // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                 // 0x00686800
};

struct CIntelligentPointer_CThing {                 // CIntelligentPointer<CThing>, 8 bytes (vptr + PData)
    void* __vftable;
    void* PData;
};

class CTCRegionFollower : public CTCBase_Calls { public:         // vtable 0x0125D234
    CRetailList<CIntelligentPointer_CThing> Followers;   // +0x0c
    virtual void v00();
    CTCRegionFollower(CThing* parent);
};

CTCRegionFollower::CTCRegionFollower(CThing* parent)
    : CTCBase_Calls(parent)
{
}