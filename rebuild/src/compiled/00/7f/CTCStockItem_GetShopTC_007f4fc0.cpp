// CTCStockItem::GetShopTC() const -- the shop component (interface 0x53) of the thing this stock item is linked to through its interface-0x33 link component, or null
// Leaf: GetShopTC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F4FC0, length 138 bytes (padded row 144).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (the owner's lookup of interface 0x33 = bit 19 of
// CThing+0x24) and the non-const copy @ 0x0040F020 (the linked thing's lookup of 0x53 = bit 19 of +0x28), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 twice.
// Layout: the 0x33 link component holds its CIntelligentPointer<CThing> at the odd offset +0x0D (bools before it; `#pragma pack(1)` as other
// CTC classes with intelligent pointers at odd PDB offsets). CThing bitset +0x20, interface map +0x44.
// Shape: the owner is a const thing (const LowerBound copy) read through the out-parameter GetInterface (`mov eax,[eax+4] ; lea esi`);
// the second lookup is a `__forceinline` GetTC (plain inline is emitted out of line); `CTCShop* shop = 0; if (HasTC) shop = GetTC;` gives the preloaded
// `xor edi,edi`; the intelligent pointer is a reference local (`lea esi,[eax+0Dh]`).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(push, 1)
struct CTCLinkComponent {                       // interface 0x33
    unsigned char _pad00[0x0d];
    CIntelligentPointer<CThing> PLinkedThing;       // +0x0d
};
#pragma pack(pop)
class CTCShop;
class CTCStockItem { public:
    virtual void v00();
    const CThing* ParentThing;                  // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool ForSale;                               // +0x0c
    bool Stealable;                             // +0x0d
    bool Initialised;                           // +0x0e
    long Price;                                 // +0x10
    CTCShop* GetShopTC() const;
};

CTCShop* CTCStockItem::GetShopTC() const
{
    CTCLinkComponent* linkTC;
    if (ParentThing->GetInterface(&linkTC, 0x33)) {
        CIntelligentPointer<CThing>& link = linkTC->PLinkedThing;
        if (link.GetPItem()) {
            CTCShop* shop = 0;
            CThing* thing = link.GetPItem();
            if (thing->HasTC(0x53))
                shop = (CTCShop*)thing->GetTC(0x53);
            return shop;
        }
    }
    return 0;
}