// CTCShop::SellItem(long itemDefIndex) -- one unit of a stocked item leaves the shop: clear it as the special item, drop its quantity by one (not below 0), raise its max stock by the def's per-sell change, and kill the first living display thing of that def in the building
// Grade: DIFFER(419v419) -- esi/edi register swap in the building-contents loop (node vs item pointer webs); tried ref/ptr locals, accessors, iterator vs while loop, inline members (20 probes).
struct CDefPointeeBase {
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
};
struct CShopDef : CDefPointeeBase {
    unsigned char _pad08[0x80 - 0x08];
    float MaxStockChangePerSell;                    // +0x80 (PDB +0x90)
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CShopDef>* out);     // 0x005130A0
};
void* FindDef_CDefClassBase_GetSubDef_1();
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

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
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
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                // +0x20
    CInterfaceMap  m_interfaces;                    // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                            // +0x70
    unsigned char  _pad74[0x8c - 0x74];
    unsigned short DefIndex;                        // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  m_stateFlags;                    // +0x91 (bit 0: dead)
    void Kill(bool immediately);                    // 0x004C9B80
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {                    // CIntelligentPointer<CThing>: vptr + item
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct SThingListNode {
    SThingListNode*         next;
    SThingListNode*         prev;
    CBaseIntelligentPointer value;                  // +0x08
};
struct CTCBuildingContents {                        // interface 0x21
    unsigned char   _pad00[0x0c];
    SThingListNode* ThingsHead;                     // +0x0c
};
struct CShopItem {
    float Quantity;                                 // +0x00
    float PriceMultiplier;                          // +0x04
    float MaxStock;                                 // +0x08
};
struct SStockNode { long color; SStockNode* parent; SStockNode* left; SStockNode* right; long first; CShopItem second; };
struct CStockIterator {
    SStockNode* node;
    bool operator==(const CStockIterator& o) const { return node == o.node; }
};
struct CStockMap {                                  // map<long, CShopItem> (retail 12 bytes)
    SStockNode* header;
    unsigned long _pad[2];
    CStockIterator find(const long& key);           // 0x00777120
    CStockIterator end() { CStockIterator it; it.node = header; return it; }
    CShopItem& operator[](const long& key);         // 0x00778280
};
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x34 - 0x08];
    CStockMap Stock;                                // +0x34
    long SpecialItem;                               // +0x40
    long DaysSinceSpecialItemPlaced;                // +0x44
    long DayLastChecked;                            // +0x48
    long GoldDonated;                               // +0x4c
    const CThing* GetThing() const { return ParentThing; }
    void SellItem(long itemDefIndex);
};

void CTCShop::SellItem(long itemDefIndex)
{
    CDefPointer<CShopDef> shopDef;
    ParentThing->PDef->GetSubDef(&shopDef);
    if (itemDefIndex == SpecialItem) {
        SpecialItem = -1;
        GoldDonated = 0;
    }
    {
        CStockIterator it = Stock.find(itemDefIndex);
        if (it.node == Stock.header)
            return;
    }
    float quantity = Stock[itemDefIndex].Quantity;
    float left;
    if (quantity < 1.0f)
        left = 0.0f;
    else
        left = quantity - 1.0f;
    Stock[itemDefIndex].Quantity = left;
    Stock[itemDefIndex].MaxStock += shopDef->MaxStockChangePerSell;
    if (Stock[itemDefIndex].MaxStock < 0.0f)
        Stock[itemDefIndex].MaxStock = 0.0f;
    const CThing* thing = GetThing();
    if (thing->HasTC(0x21)) {
        CTCBuildingContents* contents = thing->GetTC<CTCBuildingContents>(0x21);
        for (SThingListNode* node = contents->ThingsHead->next; node != contents->ThingsHead; node = node->next) {
            CBaseIntelligentPointer& item = node->value;
            if (item.GetPItem() && !(item.GetPItem()->m_stateFlags & 1) && item.GetPItem()->DefIndex == itemDefIndex) {
                item.GetPItem()->Kill(true);
                break;
            }
        }
    }
}
