// CTCWound::CTCWound(CThing* parent) -- CTCBase(parent), own vtable, LastFrameChanged = 0 and the Wounds list (list<CTCWound::CWound>, 0x4c-byte nodes) default-constructed
// Leaf: ??0CTCWound
// Retail: Fable.exe @ 0x00757640, 51 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the STLport node allocator).
// Label: the manifest's "CTextContainer::CTextContainer" is propagated -- the vtable stored, 0x012698B4, is CTCWound in
// rtti-vtables.tsv; PDB CTCWound: LastFrameChanged +0xc, Wounds +0x10.
// Shape: LastFrameChanged and the list head share the zero register (`xor eax,eax`) around the `push 4Ch`; the
// sentinel is linked to itself before it is stored. CWound is 0x44 bytes (node 0x4c).
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

struct CWound { unsigned char _body[0x44]; };      // CTCWound::CWound (retail 0x44 bytes)

class CTCWound : public CTCBase_Calls { public:                  // vtable 0x012698B4
    unsigned long        LastFrameChanged;         // +0x0c
    CRetailList<CWound>  Wounds;                   // +0x10
    virtual void v00();
    CTCWound(CThing* parent);
};

CTCWound::CTCWound(CThing* parent)
    : CTCBase_Calls(parent)
    , LastFrameChanged(0)
{
}