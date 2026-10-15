// CGraphicBankManager::CGraphicBankManager() -- CBase(), own vtable, the GraphicDataBanks list (list<CGraphicDataBankInfo>, 0x40-byte nodes) default-constructed
// Leaf: ??0CGraphicBankManager
// Retail: Fable.exe @ 0x009F8690, 43 bytes (+ int3 padding).
// Callees: CBase::CBase() @ 0x0099A2F0, malloc @ 0x00BFEA0E (the STLport node allocator).
// Label: the manifest's "CTextContainer::CTextContainer" is propagated -- the vtable stored, 0x0129C7E4, is
// CGraphicBankManager in rtti-vtables.tsv; PDB CGraphicBankManager: GraphicDataBanks at +4 (retail 4-byte list).
// Shape: base call, derived vtable store, then the inline list constructor (head nulled, sentinel self-linked, stored).
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

struct CBase {                                     // the engine root class: a vptr only
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct CGraphicDataBankInfo { unsigned char _body[0x38]; };   // CGraphicBankManager::CGraphicDataBankInfo (retail 0x38 bytes)

struct CGraphicBankManager : CBase {               // vtable 0x0129C7E4
    CRetailList<CGraphicDataBankInfo> GraphicDataBanks;   // +0x04
    virtual void v00();
    CGraphicBankManager();
};

CGraphicBankManager::CGraphicBankManager()
{
}