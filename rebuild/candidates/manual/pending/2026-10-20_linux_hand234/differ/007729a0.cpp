// CTCShop::CalcIsStallLike() -- a shop is stall-like unless its shop def keeps a permanent stock display or a live thing listed in the parent's component 0x21 has interface 0x26
// Grade: DIFFER(319v319) -- register swap only in the 0x21 list loop: retail keeps the node in edi and &node->item in esi, ours the reverse (node pointer / item reference / STLport iterator forms all give the same allocation)
// Leaf: CalcIsStallLike
// Retail: Fable.exe (TLC, VC7.1) @ 0x007729A0, 319 bytes (plain `ret`).
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0, CDefinitionManager::GetDef(long, CDefPointer*) @ 0x0042AF3C,
//   CDefClassBase::GetSubDef(CDefPointer<const CShopDef>*) @ 0x005130A0, CVectorMap::LowerBound @ 0x0040F020 (interface 0x21),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times per node), the defs' virtual slot 1 (+0x04) on release.
// Layout: CThing def index word +0x8C, interface bitset +0x20 (0x21 = byte +0x24 bit 1; 0x26 = byte +0x24 bit 6), map +0x44,
//   dead flag byte +0x91 bit 0; CShopDef IsStockDisplayPermanent +0x98 (PDB 0xA8, def bodies 0x10 low); the 0x21 component's
//   STLport list<CIntelligentPointer<CThing>> head node pointer at +0x0C (node: next +0, item +8).
// Shape: the landed CTCShop::GetName (0x00772610) def pair with the same-TU stand-ins for GetDef / GetSubDef under
//   auto_inline(off) (VC7.1 then knows the out words do not escape: the sub-def is held in ebx and its frame word is reused
//   for the 0x21 lookup key); the defs are CDefPointer locals released on every exit; the list end is re-read per node.
class CTCBase;
struct CDefPointeeBase {
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
struct CShopDef : CDefPointeeBase {
    unsigned char _pad08[0x98 - 0x08];
    bool IsStockDisplayPermanent;                   // +0x98
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CShopDef>* out);     // 0x005130A0 (CDefPointer<const CShopDef>*)
};
void* FindDef_CDefClassBase_GetSubDef_1();          // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x00772610 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CShopDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C (CDefPointer<const CThingDef>*)
};
void* FindDef_CDefinitionManager_GetDef_0();        // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                // +0x20
    CInterfaceMap  m_interfaces;                    // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                        // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  m_dead : 1;                      // +0x91 bit 0
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pInfo;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CThingListNode {                             // STLport _List_node<CIntelligentPointer<CThing> >
    CThingListNode* _M_next;
    CThingListNode* _M_prev;
    CBaseIntelligentPointer _M_data;                // +0x08
};
struct CThingList {
    CThingListNode* _M_node;
    CThingListNode* begin() const { return _M_node->_M_next; }
    CThingListNode* end() const { return _M_node; }
};
struct CTCThingList_21 {                            // interface 0x21 (named descriptively)
    unsigned char _pad00[0x0c];
    CThingList Things;                              // +0x0c
};
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    CDefinitionManager* GetDefinitions() const;     // 0x006869D0 (CTCBase)
    bool CalcIsStallLike();
};

bool CTCShop::CalcIsStallLike()
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitions()->GetDef(ParentThing->DefIndex, &thingDef);
    CDefPointer<CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef) && shopDef->IsStockDisplayPermanent)
        return false;
    CThing* thing = ParentThing;
    if (thing->HasTC(0x21)) {
        CTCThingList_21* tc = thing->GetTC<CTCThingList_21>(0x21);
        for (CThingListNode* node = tc->Things.begin(); node != tc->Things.end(); node = node->_M_next) {
            if (node->_M_data.GetPItem() && !node->_M_data.GetPItem()->m_dead && node->_M_data.GetPItem()->HasTC(0x26))
                return false;
        }
    }
    return true;
}
