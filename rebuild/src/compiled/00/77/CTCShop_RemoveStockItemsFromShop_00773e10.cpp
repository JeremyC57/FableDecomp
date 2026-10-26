// CTCShop::RemoveStockItemsFromShop() -- kill every alive owned object of the shop thing that is a stock item, then set the stock state to 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00773E10, 202 bytes (plain `ret`).
// Callees: the interface LowerBound @ 0x0040F020, CBaseIntelligentPointer::GetPItem const @ 0x00A01B50 (four times per
// object), CDefClassBase::GetSubDef_0059350c(CDefPointer&) @ 0x0059350C (the 'CStockItemDef' instantiation; landed spelling),
// CThing::Kill(bool) @ 0x004C9B80, CTCShop::SetStockState(long) @ 0x00773C00; the stock def's virtual slot 1 (destroy).
// Layout: CThing interface bitset +0x20 (0x21 = bit 1 of the second word, `test byte ptr [+24h],2`), map +0x44, dead flag
// bit 0 of +0x91, PDef +0x70; the interface 0x21 component is CTCThingOwner (Ego_r.pdb: ObjectList at +0x0c, a retail
// 4-byte STLport list -- the header node pointer -- of CIntelligentPointer<CThing>, node {next, prev, item at +8}).
// Shape: HasTC then the one-ternary GetTC into a named owner (kept in ebp); the stock def out pointer is zeroed through the
// zero register and released only on the alive path (its scope is the inner block); the thing's def is fetched into a local
// before the out pointer is pushed.
class CThing;
class CTCBase;
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
};
class CStockItemDef : public CDefBase { public: };
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_0059350c(CDefPointer<CStockItemDef>& out);   // 0x0059350C
};
void* FindDef_CDefClassBase_GetSubDef_0059350c_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_0059350c(CDefPointer<CStockItemDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0059350c_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void Kill(bool immediately);                        // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                      // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CObjectNode {                                    // STLport list node of CIntelligentPointer<CThing>
    CObjectNode* _M_next;
    CObjectNode* _M_prev;
    CBaseIntelligentPointer _M_data;                    // +0x08
};
class CTCThingOwner { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CObjectNode* ObjectList;                            // +0x0c (header node)
};
class CTCShop { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void SetStockState(long state);                     // 0x00773C00
    void RemoveStockItemsFromShop();
};

void CTCShop::RemoveStockItemsFromShop()
{
    CThing* shop = ParentThing;
    if (shop->HasTC(0x21)) {
        CTCThingOwner* owner = (CTCThingOwner*)shop->GetTC(0x21);
        for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
            CBaseIntelligentPointer& object = node->_M_data;
            CDefPointer<CStockItemDef> stockDef;
            if (object.GetPItem() && !(object.GetPItem()->m_flags91 & 1)) {
                CDefClassBase* def = object.GetPItem()->PDef;
                if (def->GetSubDef_0059350c(stockDef))
                    object.GetPItem()->Kill(true);
            }
        }
    }
    SetStockState(1);
}