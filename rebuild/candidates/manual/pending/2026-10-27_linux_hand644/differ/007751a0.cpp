// CTCShop::SetShopKeeper(CThingCreatureBase* keeper) -- replace the shop keeper: unlock the old keeper's workplace and clean the stock up, take the new keeper, then refresh the display stock and lock the new keeper's workplace
// Grade: DIFFER(419v419) -- ParentThing->HasTC(0xab) after the GetPItem compare: retail mov eax,[ebp+4] ; mov ecx,[eax+34h] ; test ch,8, ours mov ecx,[ebp+4] ; mov eax,[ecx+34h] ; test ah,8 (and the later lea ecx/edx for the 0xBD key). A std::bitset<274> UsingComponents read through the non-const reference operator[] (probe/007751a0_p4.cpp) gives retail's exact registers but leaves one dead add eax,34h (422v419).
// Earlier wave: DIFFER(419v419) -- HasTC(0xab) on the reloaded ParentThing loads into ecx/eax swapped (and the following lea edx/ecx); tried accessor, split-if, named locals (9 probes).
// Leaf: ?SetShopKeeper@CTCShop@@QAEXPAVCThingCreatureBase@@@Z
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CTCTrespassable {                            // interface 0xBD
    unsigned char _pad00[0x0c];
    bool Trespassable;                              // +0x0c
};
class CThing;
class CBaseObject;
class CThingCreatureBase;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +4
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
    bool operator==(const CThing* item) const { return (CThing*)GetPItem() == item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                // +0x20
    CInterfaceMap  m_interfaces;                    // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  m_flags91;                       // +0x91 bit 0: dead
    unsigned char  _pad92[0xd4 - 0x92];
    unsigned char  m_houseLockingOverridden : 1;    // +0xd4 bit 0 (building)
    unsigned char  _padd5[0x170 - 0xd5];
    CBaseIntelligentPointer PWorkplace;             // +0x170 (creature; descriptive)
    CThing* GetPWorkplace() const { return (CThing*)PWorkplace.GetPItem(); }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    enum ESTOCK_STATE { };
    ESTOCK_STATE StockState;                        // +0x0c
    CIntelligentPointer<CThingCreatureBase> PShopKeeper;   // +0x10
    bool IsPermanentlyStocked() const;              // 0x007721A0
    void StockDisplay();                            // 0x00774C60
    void SetStockState(ESTOCK_STATE state);         // 0x00773C00
    void RemoveStockItemsFromShop();                // 0x00773E10
    void SetWorkplaceLocked(bool locked);
    void SetShopKeeper(CThingCreatureBase* keeper);
};

void CTCShop::SetShopKeeper(CThingCreatureBase* keeper)
{
    if ((CThing*)keeper == (CThing*)PShopKeeper.GetPItem())
        return;
    if (IS_THING_ALIVE(PShopKeeper)) {
        CThing* workplace = ((CThing*)PShopKeeper.GetPItem())->GetPWorkplace();
        if (workplace && !(workplace->m_flags91 & 1) && !workplace->HasTC(0xab)) {
            workplace->m_houseLockingOverridden = 0;
            CTCTrespassable* trespassable;
            if (workplace->GetInterface(&trespassable, 0xbd))
                trespassable->Trespassable = false;
        }
        RemoveStockItemsFromShop();
    }
    {
        CIntelligentPointer<CThingCreatureBase> item;
        item.SetPItem((CBaseObject*)keeper);
        PShopKeeper.SetPItem(item.GetPItem());
    }
    if (IS_THING_ALIVE(PShopKeeper)) {
        if (IsPermanentlyStocked()) {
            StockDisplay();
            if (PShopKeeper == ParentThing || ParentThing->HasTC(0xab))
                SetStockState((ESTOCK_STATE)0);
        }
        CThing* workplace = ((CThing*)PShopKeeper.GetPItem())->GetPWorkplace();
        if (workplace && !(workplace->m_flags91 & 1) && !workplace->HasTC(0xab)) {
            workplace->m_houseLockingOverridden = 1;
            CTCTrespassable* trespassable;
            if (workplace->GetInterface(&trespassable, 0xbd))
                trespassable->Trespassable = true;
        }
    }
}
