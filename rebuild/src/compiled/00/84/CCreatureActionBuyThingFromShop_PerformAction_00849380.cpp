// CCreatureAction_BuyThingFromShop::PerformAction() -- buy the shop's stock item for the creature: when the creature's inventory can take it, pay the price, create a copy of the item at the creature, add it to that inventory, give the shopkeeper a sold-item event and post message 0x13; otherwise show "already have" / "no room" as a 3-second subtitle
// Leaf: PerformAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00849380, length 886 bytes (+ int3 padding), virtual PerformAction, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +0x08, PTarget +0xa8), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound const @ 0x004365B0 (the target's lookups: 0x54 CTCStockItem = bit 20 of +0x28, 0x1a CTCInventoryItem =
//   bit 26 of +0x20) and the non-const copy @ 0x0040F020 (the creature's 0x04 CTCHeroStats, the item's inventory type, the new
//   thing's 0x1a, the shopkeeper's 0x42 CTCEntityEvents), CTCInventoryItem::GetInventoryType @ 0x005D9030 / GetMaxNumberItems
//   @ 0x005D9050 (def +0x50 / +0x40; the manifest "CCategory::GetCategoryIdentifier" labels are propagated), puts @ 0x00BFEAF2 +
//   abort @ 0x00BFEAEC (STLport's exception-free bitset<274>::test range check, "bitset" @ 0x0122E068), CTCInventoryBase::CanAddItem
//   @ 0x005BF2ED, CCharString() @ 0x0099E4B0 / (const char*, long) @ 0x0099EBF0 / operator=(const char*) @ 0x0099EFE0 / ~ @
//   0x0099EAE0, CThing::GetMainGameComponent @ 0x004C7A70 (a static accessor called on the creature; font bank +0x1614c),
//   CSubtitleRenderer::SetText @ 0x00492ED1 on the player GUI @ 0x013B8790 (+0x18), CTCHeroStats::AddMoney @ 0x0057B327,
//   CThing::GetPosition @ 0x004C73D0, CThingObject::Create @ 0x00703210 (__fastcall), CTCInventoryItem::AddToInventory @
//   0x005D8D50, CTCStockItem::GetShopTC @ 0x007F4FC0, operator new @ 0x00BFEA1A (0x1c bytes), NEntityEvents::CEventSoldItem ctor
//   @ 0x0084AF30, CTCEntityEvents::AddEvent @ 0x008113C0, CCreatureActionBase::GetWorld @ 0x00692940, CMessageEventManager::
//   AddMessage @ 0x00687540. Strings: "You already have this item!" @ 0x012758B8, "You don't have room in your inventory for this
//   item!" @ 0x0127582C, "" @ 0x0122D70E.
// Layout: no engine header fits (CCreatureActionBase.h is the PDB 0xb4 view with 16-byte containers; retail is 0xa8, as in the
//   landed siblings 0x00848500 / 0x008491E0 of this TU): PCreature +0x08, PTarget +0xa8. CThing: bitset +0x20, interface map +0x44,
//   DefIndex word +0x8c, PlayerNumber byte +0x90, dead bit +0x91. CTCStockItem::Price +0x10, CTCShop::PShopKeeper +0x10.
// Shape: both CVectorMap::LowerBound copies are same-TU noinline stand-ins (the const one for the target's lookups through a
//   `const CThing*`), so every lookup key shares its frame word with its out pointer. The subtitle renderer is a named local
//   read before the first lookup; the inventory lookup is the range-checked bitset::test form. The creation goes through an
//   inline helper taking the three things (target, creature, creature) plus the implicit "" name: the three GetPItem calls come
//   first, the def word is read into a local in the helper, then the stack arguments, then GetPosition into edx. The font bank
//   comes from a named CMainGameComponent local (a named font-bank local or a plain argument picks eax over edx); the sold
//   event's buyer goes through the intelligent pointer's inline conversion (fetched before the push of `false`); the shopkeeper
//   is an inline CTCShop accessor (`lea ecx,[eax+10h]`); the message manager is a named local after a named buyer local.
extern "C" int __cdecl puts(const char* text);          // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-ins for the two byte-identical LowerBound copies (STLport lower_bound over the 8-byte {type, component}
// entries): with the bodies visible VC7.1 knows the key's address does not escape and packs the keys like retail.
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
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CFontBank;
struct CMainGameComponent {
    unsigned char _pad[0x1614c];
    CFontBank*    FontBank;                             // +0x1614c
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 bitset<274> of the interface types present
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                            // +0x8c
    unsigned char  _pad8e[2];
    signed char    PlayerNumber;                        // +0x90
    unsigned char  m_flags91;                           // +0x91 bit 0: dead
    long GetPlayer() const { return PlayerNumber; }
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CMainGameComponent* GetMainGameComponent();         // 0x004C7A70
    bool TestComponent(unsigned long position) const    // STLport bitset<274>::test (exception-free range check)
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (m_presentBits[position >> 5] & (1UL << (position & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetCheckedInterface(T** ppOut, long interfaceType)
    {
        if (TestComponent(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* orientation,
                                     unsigned long flags, const CCharString& name);   // 0x00703210
// A copy of `item` at `at`, owned by `owner`'s player.
__forceinline CThing* CreateThingAt(const CThing* item, const CThing* at, const CThing* owner, const CCharString& name)
{
    long defIndex = item->DefIndex;
    return CreateThingObject(defIndex, &at->GetPosition(), owner->GetPlayer(), 0, 0, name);
}

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
struct CPlayerGui {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

struct CTCHeroStats {
    void AddMoney(long amount, bool showMessage);       // 0x0057B327
};
struct CTCInventoryBase {
    bool CanAddItem(long defIndex) const;               // 0x005BF2ED
};
struct CTCInventoryItem {
    long GetInventoryType() const;                      // 0x005D9030
    long GetMaxNumberItems() const;                     // 0x005D9050
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long price, bool c, bool d);   // 0x005D8D50
};
struct CTCShop {
    unsigned char           _pad00[0x10];
    CBaseIntelligentPointer PShopKeeper;                // +0x10
    CThing* GetShopKeeper() const { return PShopKeeper.GetPItem(); }
};
struct CTCStockItem {
    unsigned char _pad00[0x10];
    long          Price;                                // +0x10
    CTCShop* GetShopTC() const;                         // 0x007F4FC0
};
struct CEventBase { };
namespace NEntityEvents {
struct CEventSoldItem : CEventBase {                    // 0x1c bytes
    unsigned char _pad[0x1c];
    CEventSoldItem(CThing* giver, CThing* item, bool ownsItem);   // 0x0084AF30
};
}
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);                   // 0x008113C0
};
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                     float value1, float value2, long data);   // 0x00687540
};
struct CWorld {
    unsigned char         _pad00[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};

struct CCreatureActionBase {                            // retail sizeof 0xa8
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    CWorld* GetWorld();                                 // 0x00692940
};
struct CCreatureAction_BuyThingFromShop : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;                    // +0xa8 the stock item on display
    virtual void PerformAction();
};

void CCreatureAction_BuyThingFromShop::PerformAction()
{
    if (IS_THING_ALIVE(PTarget)) {
        CSubtitleRenderer* subtitles = &g_pPlayerGui->SubtitleRenderer;
        CTCStockItem* stockItem;
        static_cast<const CThing*>(PTarget.GetPItem())->GetInterface(&stockItem, 0x54);
        CTCHeroStats* stats;
        if (PCreature.GetPItem()->GetInterface(&stats, 4)) {
            CTCInventoryItem* inventoryItem;
            static_cast<const CThing*>(PTarget.GetPItem())->GetInterface(&inventoryItem, 0x1a);
            CThing* creature = PCreature.GetPItem();
            CTCInventoryBase* inventory;
            if (creature->GetCheckedInterface(&inventory, inventoryItem->GetInventoryType())) {
                if (!inventory->CanAddItem(PTarget.GetPItem()->DefIndex)) {
                    CCharString message;
                    if (inventoryItem->GetMaxNumberItems() == 1)
                        message = "You already have this item!";
                    else
                        message = "You don't have room in your inventory for this item!";
                    CMainGameComponent* component = PCreature.GetPItem()->GetMainGameComponent();
                    subtitles->SetText(component->FontBank, message, 0, 3.0f, false, true);
                    return;
                }
                stats->AddMoney(-stockItem->Price, true);
                CThing* item = CreateThingAt(PTarget.GetPItem(), PCreature.GetPItem(), PCreature.GetPItem(), "");
                CTCInventoryItem* newItem;
                item->GetInterface(&newItem, 0x1a);
                newItem->AddToInventory(inventory, false, true, stockItem->Price, false, false);
                CThing* shopKeeper = stockItem->GetShopTC()->GetShopKeeper();
                CTCEntityEvents* events;
                if (shopKeeper && shopKeeper->GetInterface(&events, 0x42))
                    events->AddEvent(new NEntityEvents::CEventSoldItem(PCreature, PTarget.GetPItem(), false));
                CThing* buyer = PCreature.GetPItem();
                CMessageEventManager* manager = GetWorld()->MessageEventManager;
                manager->AddMessage(0x13, 1000, buyer->GetPosition(), PCreature.GetPItem(), 0, 0.0f, 0.0f, 0);
            }
        }
    }
}