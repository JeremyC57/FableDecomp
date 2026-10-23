// Grade: DIFFER(734v728) -- first drafts: retail keeps the item def in ebp and reuses its frame slot for the value def, thing-def release form differs (retail load/dec/store and zeroes the pointer), frame layout / register allocation of the outer loop
// CTCWife::FindAppropriateGift() -- pick a random gift budget around the wife's gift price value, then look through the shop stock of every container thing in her home for the non-wearable, non-special gift whose value is closest to that budget (and not more than a quarter over it); remember it and deduct its value
// Leaf: ?FindAppropriateGift@CTCWife
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF530, 728 bytes (+ int3 padding), plain `ret`.
// Callees: LowerBound 0x0040F020 (interfaces 0x23 home, 0x53 stock); the home's contents getter 0x007EE540; CTCBase::GetWorld
//   0x00686A70 / GetDefinitionManager 0x006869D0; CWorld::GetPRandomSeed 0x0049D810; GFFloatRandom 0x004978A0 / GFROR13
//   0x00497890 (`__fastcall`); __ftol2 0x00BFEA70; CBaseIntelligentPointer::GetPItem 0x00A01B50; the thing's def getter
//   0x0082D2D0 (label "CThingBuilding::PeekPMaterial"); the stock map getter 0x00774610; map iterator increment 0x005B16FF /
//   ~map 0x004DF885; CDefinitionManager::GetDef 0x0042AF3C; GetSubDef 0x006D3EF0 / 0x0059350C (propagated labels).
// Layout (retail CTCWife = Ego_r.pdb up to +0x78): ParentThing +0x04, GiftGivingPriceValue +0x70, GiftToGiveDef +0x74.
#include <stdlib.h>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CDefNative {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_count;                                       // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->m_count == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};
struct CGiftDef : CDefNative {                          // descriptive
    unsigned char _pad08[0x28 - 0x08];
    long Kind;                                          // +0x28
    bool Special;                                       // +0x2c
};
struct CValueDef : CDefNative {                         // descriptive
    unsigned char _pad08[0x28 - 0x08];
    long Value;                                         // +0x28
};
struct CItemDef : CDefNative {
    bool GetGiftDef(CDefPointer<CGiftDef>* out);        // 0x006D3EF0
    bool GetValueDef(CDefPointer<CValueDef>* out);      // 0x0059350C
};
struct CThingDef : CDefNative {
    unsigned char _pad08[0x3c - 0x08];
    unsigned long Flags;                                // +0x3c bit 4: holds stock (descriptive)
};
struct CThingDefPointer {
    CThingDef* Object;
    CThingDefPointer() : Object(0) { }
    ~CThingDefPointer()
    {
        if (Object) {
            long count = --Object->m_count;
            if (count == 0)
                Object->Destroy();
            Object = 0;
        }
    }
    CThingDef* operator->() const { return Object; }
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CItemDef>* out);   // 0x0042AF3C
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    void GetDef(CThingDefPointer* out);           // 0x0082D2D0
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
};
struct CStockMap {                                      // map<long, long> (STLport)
    struct node { long color; node* parent; node* left; node* right; long key; long value; };
    struct iterator {
        node* p;
        explicit iterator(node* q) : p(q) { }
        void Increment();                               // 0x005B16FF
    };
    node* header;
    long count;
    long _cmp;
    ~CStockMap();                                       // 0x004DF885
};
struct CTCStock {                                       // interface 0x53 (descriptive)
    void GetStock(CStockMap* out);                      // 0x00774610
};
struct CHomeContents {
    unsigned char _pad00[0x28];
    CRetailList<CBaseIntelligentPointer> Things;        // +0x28
};
struct CTCHome {                                        // interface 0x23
    CHomeContents* GetContents();                       // 0x007EE540
};
struct CWorld {
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
unsigned long __fastcall GFROR13(unsigned long value);             // 0x00497890
__forceinline bool RandomBool(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed % 0xffffffffUL > 0x7fffffffUL;
}

struct CTCWife {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x70 - 0x08];
    long GiftGivingPriceValue;                          // +0x70
    long GiftToGiveDef;                                 // +0x74
    CWorld* GetWorld();                                 // 0x00686A70
    CDefinitionManager* GetDefinitionManager();         // 0x006869D0
    void FindAppropriateGift();
};

void CTCWife::FindAppropriateGift()
{
    if (!ParentThing->HasTC(0x23))
        return;
    CHomeContents* contents = ParentThing->GetTC<CTCHome>(0x23)->GetContents();
    if (!contents)
        return;
    float spread = GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed());
    spread = spread * spread + 1.0f;
    long price = GiftGivingPriceValue;
    long budget = (long)(RandomBool(GetWorld()->GetPRandomSeed()) ? price * spread : price / spread);
    long bestGift = 0;
    long bestValue = 0x7fffffff;
    CRetailList<CBaseIntelligentPointer>::node* head = contents->Things.head;
    for (CRetailList<CBaseIntelligentPointer>::node* n = head->next; n != head; n = n->next) {
        CThing* thing = n->value.GetPItem();
        if (!thing || thing->IsDead())
            continue;
        CThingDefPointer thingDef;
        thing->GetDef(&thingDef);
        if (!thingDef.Object)
            continue;
        if ((thingDef->Flags & 0x10) && thing->HasTC(0x53)) {
            CStockMap stock;
            thing->GetTC<CTCStock>(0x53)->GetStock(&stock);
            for (CStockMap::iterator it(stock.header->left); it.p != stock.header; it.Increment()) {
                CDefPointer<CItemDef> item;
                GetDefinitionManager()->GetDef(it.p->key, &item);
                CDefPointer<CGiftDef> gift;
                if (!item->GetGiftDef(&gift) || (gift->Kind != 1 && !gift->Special)) {
                    CDefPointer<CValueDef> value;
                    item->GetValueDef(&value);
                    long cost = value->Value;
                    if (abs(cost - budget) < abs(budget - bestValue) && cost - budget < budget / 4) {
                        bestValue = cost;
                        bestGift = it.p->key;
                    }
                }
            }
        }
    }
    if (bestGift > 0) {
        GiftToGiveDef = bestGift;
        GiftGivingPriceValue -= bestValue;
    }
}
