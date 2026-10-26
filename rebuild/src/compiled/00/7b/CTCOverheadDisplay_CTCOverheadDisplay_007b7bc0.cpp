// CTCOverheadDisplay::CTCOverheadDisplay(CThing* parent) -- overhead-display component: CTCBase(parent), three empty damage/repair/renown value lists, a null def pointer and no primitive handles
// Leaf: ??0CTCOverheadDisplay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B7BC0, length 98 bytes (+ int3 padding).
// Label correction: the catalog's "CTCDHeroSuit::CTCDHeroSuit" is propagated and wrong: the vtable stored, 0x0126DA94, is
// CTCOverheadDisplay's (rtti-vtables.tsv), and the stores fit Ego_r.pdb CTCOverheadDisplay with retail 4-byte lists.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (x3, one 0x10-byte sentinel node per list).
// Layout (retail): DamageValues +0xC, RepairValues +0x10, RenownValues +0x14 (single-pointer STLport list<pair<long,float> >),
// PDef +0x18 (CDefPointer<COverheadDisplayDef>), PrimitiveHandles +0x1C (12-byte vector).
// Shape: as the landed CTCTrophy ctor (0x006DDAE0): each inline list ctor nulls its head before malloc and links the node
// to itself before storing it; the three malloc pushes are popped together; one edi zero register; the vector is the STLport
// _Vector_base + alloc-proxy model (it places the `add esp,0Ch` before the last zero store).
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E

class CThing;
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* parent);                    // 0x00686800
};
struct CRetailList_node0x10 {                   // list<pair<long, float> >
    struct node { node* next; node* prev; long first; float second; };
    node* head;
    CRetailList_node0x10() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};
struct CDefPointer_COverheadDisplayDef {
    const void* m_pDef;
    CDefPointer_COverheadDisplayDef() : m_pDef(0) {}
};
template <class T> struct SRetailAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes): _Vector_base start / finish + the alloc proxy
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
class CEnginePrimitiveHandle;
class CTCOverheadDisplay : public CTCBase { public:
    CRetailList_node0x10 DamageValues;          // +0x0c
    CRetailList_node0x10 RepairValues;          // +0x10
    CRetailList_node0x10 RenownValues;          // +0x14
    CDefPointer_COverheadDisplayDef PDef;       // +0x18
    CRetailVector<CEnginePrimitiveHandle> PrimitiveHandles;  // +0x1c
    virtual void v01();
    CTCOverheadDisplay(CThing* parent);
};

CTCOverheadDisplay::CTCOverheadDisplay(CThing* parent)
    : CTCBase(parent)
{
}