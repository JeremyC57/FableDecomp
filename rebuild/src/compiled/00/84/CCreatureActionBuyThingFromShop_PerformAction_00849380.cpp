// CCreatureAction_BuyThingFromShop::PerformAction() -- buy the shop's stock item for the creature: when the creature's inventory can take it, pay the price, create a copy of the item at the creature, add it to that inventory, give the shopkeeper a sold-item event and post message 0x13; otherwise show "already have" / "no room" as a 3-second subtitle
// Leaf: PerformAction
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
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
//   CThing::GetPos @ 0x004C73D0, CThingObject::Create @ 0x00703210 (__fastcall), CTCInventoryItem::AddToInventory @
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
//   first, the def word is read into a local in the helper, then the stack arguments, then GetPos into edx. The font bank
//   comes from a named CMainGameComponent local (a named font-bank local or a plain argument picks eax over edx); the sold
//   event's buyer goes through the intelligent pointer's inline conversion (fetched before the push of `false`); the shopkeeper
//   is an inline CTCShop accessor (`lea ecx,[eax+10h]`); the message manager is a named local after a named buyer local.
extern "C" int __cdecl puts(const char* text);          // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
class CTCBase;
#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CFontBank;
class CMainGameComponent { public:
    unsigned char _pad[0x1614c];
    CFontBank*    FontBank;                             // +0x1614c
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 bitset<274> of the interface types present
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad54[0x38];
    unsigned short DefIndex;                            // +0x8c
    unsigned char  _pad8e[2];
    signed char    PlayerNumber;                        // +0x90
    unsigned char  m_flags91;                           // +0x91 bit 0: dead
    long GetPlayer() const { return PlayerNumber; }
    const C3DVector& GetPos() const;               // 0x004C73D0
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
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetCheckedInterface(T** ppOut, long interfaceType)
    {
        if (TestComponent(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CRightHandedSet;
class CThingObject;
class CThingObject { public:
    static CThingObject* __fastcall Create(long a0, C3DVector const&a1, long a2, CRightHandedSet const*a3, unsigned long a4, CCharString const&a5);   // 0x00703210 (Ego_r.pdb ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)
};
// A copy of `item` at `at`, owned by `owner`'s player.
__forceinline CThing* CreateThingAt(const CThing* item, const CThing* at, const CThing* owner, const CCharString& name)
{
    long defIndex = item->DefIndex;
    return ((CThing*)CThingObject::Create(defIndex, *(&at->GetPos()), owner->GetPlayer(), 0, 0, name));
}

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                    // +0x04
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};

class CTCHeroStats { public:
    void AddMoney(long amount, bool showMessage);       // 0x0057B327
};
struct CTCInventoryBase {
    bool CanAddItem(long defIndex) const;               // 0x005BF2ED
};
class CTCInventoryItem { public:
    long GetInventoryType() const;                      // 0x005D9030
    long GetMaxNumberItems() const;                     // 0x005D9050
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long price, bool c, bool d);   // 0x005D8D50
};
class CTCShop { public:
    unsigned char           _pad00[0x10];
    CBaseIntelligentPointer PShopKeeper;                // +0x10
    CThing* GetShopKeeper() const { return ((CThing*)PShopKeeper.GetPItem()); }
};
class CTCStockItem { public:
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
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);                   // 0x008113C0
};
class CMessageEventManager { public:
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                     float value1, float value2, long data);   // 0x00687540
};
class CWorld { public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};

class CCreatureActionBase { public:                            // retail sizeof 0xa8
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
        CSubtitleRenderer* subtitles = &CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer;
        CTCStockItem* stockItem;
        static_cast<const CThing*>(((CThing*)PTarget.GetPItem()))->GetInterface(&stockItem, 0x54);
        CTCHeroStats* stats;
        if (((CThing*)PCreature.GetPItem())->GetInterface(&stats, 4)) {
            CTCInventoryItem* inventoryItem;
            static_cast<const CThing*>(((CThing*)PTarget.GetPItem()))->GetInterface(&inventoryItem, 0x1a);
            CThing* creature = ((CThing*)PCreature.GetPItem());
            CTCInventoryBase* inventory;
            if (creature->GetCheckedInterface(&inventory, inventoryItem->GetInventoryType())) {
                if (!inventory->CanAddItem(((CThing*)PTarget.GetPItem())->DefIndex)) {
                    CCharString message;
                    if (inventoryItem->GetMaxNumberItems() == 1)
                        message = "You already have this item!";
                    else
                        message = "You don't have room in your inventory for this item!";
                    CMainGameComponent* component = ((CThing*)PCreature.GetPItem())->GetMainGameComponent();
                    subtitles->SetText(component->FontBank, message, 0, 3.0f, false, true);
                    return;
                }
                stats->AddMoney(-stockItem->Price, true);
                CThing* item = CreateThingAt(((CThing*)PTarget.GetPItem()), ((CThing*)PCreature.GetPItem()), ((CThing*)PCreature.GetPItem()), "");
                CTCInventoryItem* newItem;
                item->GetInterface(&newItem, 0x1a);
                newItem->AddToInventory(inventory, false, true, stockItem->Price, false, false);
                CThing* shopKeeper = stockItem->GetShopTC()->GetShopKeeper();
                CTCEntityEvents* events;
                if (shopKeeper && shopKeeper->GetInterface(&events, 0x42))
                    events->AddEvent(new NEntityEvents::CEventSoldItem(PCreature, ((CThing*)PTarget.GetPItem()), false));
                CThing* buyer = ((CThing*)PCreature.GetPItem());
                CMessageEventManager* manager = GetWorld()->MessageEventManager;
                manager->AddMessage(0x13, 1000, buyer->GetPos(), ((CThing*)PCreature.GetPItem()), 0, 0.0f, 0.0f, 0);
            }
        }
    }
}