// CTCShop::SellItem(long itemDefIndex) -- one unit of a stocked item leaves the shop: clear it as the special item, drop its quantity by one (not below 0), raise its max stock by the def's per-sell change, and kill the first living display thing of that def in the building
// Grade: DIFFER(ours v retail) -- esi/edi register swap in the building-contents loop (node vs item pointer webs); tried ref/ptr locals, accessors, iterator vs while loop, inline members (20 probes).; chunk 759 also tried engine/CIntelligentPointer.h + IS_THING_ALIVE, PeekTC over the real CVectorMap (TCI_THING_OWNER) and a GetSubDef<CShopDef> stand-in
// Leaf: ?SellItem@CTCShop@@QAEXJ@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CDefPointeeBase { public:
    virtual void Destroy();                         // slot 0
    virtual void Delete() const;                    // slot 1 (+0x04)
    mutable long RefCount;                          // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CShopDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x80 - 0x08];
    float MaxStockChangePerSell;                    // +0x80 (PDB +0x90)
};
class CSubDefInfo { public: long DefIndex; long OriginalParentDefIndex; };
class CDefinitionManager { public:
    CDefPointer<const CShopDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase : public CDefPointeeBase { public:
    virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* PDefManager;                // +0x1c
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
// Same-TU body of GetSubDef<CShopDef> 0x005130A0 (template member, out of line in retail): the landed CTCWound::AddWound
// 0x00757B30 stand-in shape (`__declspec(noinline)`).
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CShopDef>(CDefPointer<const CShopDef>* out) const
{
    const CSubDefInfo* info;
    {
        CCharString name("CShopDef", -1);
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CShopDef> result = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        *out = result;
        return true;
    }
    return false;
}
class CTCBase { public: virtual ~CTCBase(); };
class CThing : public CBaseObject {
public:
    unsigned char  _pad08[0x20 - 0x08];
    std::bitset<274> UsingComponents;               // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char  _pad54[0x70 - 0x54];
    CDefClassBase* PDef;                            // +0x70
    unsigned char  _pad74[0x8c - 0x74];
    unsigned short DefIndex;                        // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  m_stateFlags;                    // +0x91 (bit 0: dead)
    void Kill(bool immediately);                    // 0x004C9B80
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id, T** ptc) const
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            const std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = static_cast<T*>(it->second);
            return true;
        }
        return false;
    }
};
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& p)   // FableWin ??$IS_THING_ALIVE@..@@YI_NABV..@@Z (inline)
{
    if (p.Get() != 0) {
        if (!(p.Get()->m_stateFlags & 1))
            return true;
    }
    return false;
}
struct SThingListNode {                             // STLport list<CIntelligentPointer<CThing> > node
    SThingListNode*             next;
    SThingListNode*             prev;
    CIntelligentPointer<CThing> value;              // +0x08
};
class CTCThingOwner : public CTCBase { public:      // TCI_THING_OWNER (engine/CTCThingOwner.h layout)
    unsigned char   _pad04[0x0c - 0x04];
    SThingListNode* ObjectList;                     // +0x0c (the STLport list's node pointer)
};
class CShopItem { public:
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
class CTCShop { public:
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
    CDefPointer<const CShopDef> shopDef;
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
    CTCThingOwner* owner;
    if (thing->PeekTC(TCI_THING_OWNER, &owner)) {
        for (SThingListNode* node = owner->ObjectList->next; node != owner->ObjectList; node = node->next) {
            CIntelligentPointer<CThing>& item = node->value;
            if (IS_THING_ALIVE(item) && item->DefIndex == itemDefIndex) {
                item->Kill(true);
                break;
            }
        }
    }
}
