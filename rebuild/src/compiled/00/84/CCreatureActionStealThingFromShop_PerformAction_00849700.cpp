// CCreatureAction_StealThingFromShop::PerformAction() -- steal the live shop item: when the creature's inventory cannot take it, show "You already have this item!" (single-holding items) or the no-room message as a 3-second subtitle; otherwise create a copy of the item at the creature, add it to that inventory, post message 0x1c (delay 0x32) from the thief to the item's owner (the interface-0x33 CTCOwnedEntity::POwner) and make a SIMPLE_NOISE
// Leaf: ?PerformAction@CCreatureAction_StealThingFromShop@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00849700, 719 bytes, virtual PerformAction, plain `ret`. Ego_r.pdb
//   `?PerformAction@CCreatureAction_StealThingFromShop@@UAEXXZ`; the item is the PDB's PThingToSteal (retail +0xa8, PDB +0xb4).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 / const
//   copy 0x004365B0, CTCInventoryItem::GetInventoryType 0x005D9030 / GetMaxNumberOfItems 0x005D9050 (both PDBs, next after GetInventoryType: Ego_r 0x00559F80 / 0x00559FA0 = retail +0x20; the landed sibling
//   0x00849380 spells it GetMaxNumberItems) / AddToInventory 0x005D8D50, puts 0x00BFEAF2 + abort 0x00BFEAEC (STLport bitset<274>::test range check, "bitset"),
//   IsSpaceInInventoryForItem 0x005BF2ED (Ego_r + FableWin.pdb QBE_NJ@Z; Ego_r pair IsSpaceInInventoryForItem / GetAmountOfItemsContainable 0x26 apart = retail 0x005BF2ED / 0x005BF30D, both tail-calling the CCategory pair 0x005BF19C / 0x005BF1D7 = Ego_r IsSpaceInCategoryForItem / GetAmountOfItemsContainable), CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / operator=(const char*)
//   0x0099EFE0 / ~ 0x0099EAE0, CThing::PeekComponent 0x004C7A70, CSubtitleRenderer::SetText 0x00492ED1 on g_pPlayerGui
//   0x013B8790 (+0x18 SubtitleRenderer), CThing::GetPos 0x004C73D0, CThingObject::Create 0x00703210 (__fastcall),
//   CCreatureActionBase::GetWorld 0x00692940 (CWorld +0x60 PMessageEventManager), CMessageEventManager::AddMessage 0x00687540,
//   CThingCreatureBase::MakeNoise 0x00662110.
// Layout: retail CCreatureActionBase is 0xa8 bytes (engine/CCreatureActionBase.h is the PDB 0xb4 view with 16-byte containers;
//   the landed siblings 0x00849380 / 0x00848500 use the same local model). CThing (retail): UsingComponents +0x20, Components
//   +0x44, DefGlobalIndex +0x8c, PlayerNumber +0x90, Dead bit 0 of +0x91 (PDB +0x9c/+0xa0/+0xa1: 16 bytes of containers less).
//   CMainGameComponent::PCutSceneMainFont +0x1614c (PDB offset). CTCOwnedEntity is packed in the PDB (POwner at +0x0d).
// Shape (chunk 618, from the landed sibling CCreatureAction_BuyThingFromShop::PerformAction 0x00849380): the creation goes
//   through an inline helper taking the item and the creature twice (the three GetPItem calls first, the def word read into a
//   local in the helper, then the stack arguments, GetPos into edx); the font bank comes from a named CMainGameComponent local
//   (edx); the message manager is a named local after a named thief local; the non-const interface lookup selects its entry
//   with a ternary (the copy's and the owner's lookups then load the component straight into ecx) while the const lookup keeps
//   the if-form; the creation's thing arguments are direct GetPItem expressions (an inline GetCreature() swaps edi/ebx).
// Names: GetCreature() is a descriptive inline (the PDB's CCreatureActionBase::GetPCreature returns `const CThingCreatureBase*`;
//   a CIntelligentPointer<T>::GetPItem template layer changes the register allocation here, so the casts are spelled out as
//   in the landed sibling); GetCheckedTC / GetTC / GetInterface / CreateThingAt are the fleet's descriptive inline helpers.
extern "C" int __cdecl puts(const char* text);          // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
#include "engine/C3DVector.h"
class CRightHandedSet;
class CCharString { public:
    void* m_pData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
class CTCBase;
class CThing;
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CFontBank;
#include "engine/CCountedPointer.h"
class CMainGameComponent { public:
    unsigned char              _pad[0x1614c];
    CCountedPointer<CFontBank> PCutSceneMainFont;       // +0x1614c (Ego_r.pdb offset)
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad54[0x38];
    unsigned short DefGlobalIndex;                      // +0x8c
    unsigned char _pad8e[2];
    signed char   PlayerNumber;                         // +0x90
    unsigned char  Dead : 1;                            // +0x91
    long GetPlayer() const { return PlayerNumber; }
    const C3DVector& GetPos() const;                    // 0x004C73D0
    CMainGameComponent* PeekComponent();                // 0x004C7A70
    bool TestComponent(unsigned long position) const    // STLport bitset<274>::test (exception-free range check)
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (UsingComponents[position >> 5] & (1UL << (position & 31))) != 0;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
    __forceinline CTCBase* GetCheckedTC(long interfaceType)
    {
        CTCBase* tc;
        if (TestComponent(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player, const CRightHandedSet* orientation,
                                           unsigned long flags, const CCharString& scriptName);   // 0x00703210
};
class CThingCreatureBase : public CThing { public:
    void MakeNoise(const CCharString& noise);           // 0x00662110
};
// A copy of `item` at `at`, owned by `owner`'s player (the landed sibling 0x00849380's helper).
__forceinline CThing* CreateThingAt(const CThing* item, const CThing* at, const CThing* owner, const CCharString& name)
{
    long defIndex = item->DefGlobalIndex;
    return CThingObject::Create(defIndex, at->GetPos(), owner->GetPlayer(), 0, 0, name);
}
namespace NInventory {
class CTCInventoryBase { public:
    bool IsSpaceInInventoryForItem(long defIndex) const;   // 0x005BF2ED (PDB name, see Callees)
};
}
class CTCInventoryItem { public:
    ETCInterfaceType GetInventoryType() const;          // 0x005D9030
    long GetMaxNumberOfItems() const;                   // 0x005D9050 (PDB name)
    bool AddToInventory(NInventory::CTCInventoryBase& inventory, bool a, bool b, long price, bool c, bool d);   // 0x005D8D50
};
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
class CMessageEventManager { public:
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                     float value1, float value2, long data);   // 0x00687540
};
class CWorld { public:
    unsigned char         _pad[0x60];
    CMessageEventManager* PMessageEventManager;         // +0x60 (CCountedPointer data word)
};
#pragma pack(push, 1)
class CBaseObject;
class CBaseIntelligentPointer { public:                 // vtable 0x0129C95C (vptr + PData, 8 bytes)
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    void* PData;                                        // +0x04
};
class CTCOwnedEntity { public:                          // interface 0x33 (Ego_r.pdb 32 bytes, packed)
    unsigned char           _pad00[0x0c];
    bool                    SwitchableNavigationTCAdded;   // +0x0c
    CBaseIntelligentPointer POwner;                     // +0x0d
};
#pragma pack(pop)
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CThing* GetCreature() const { return (CThing*)PCreature.GetPItem(); }
protected:
    CWorld& GetWorld();                                 // 0x00692940
};
class CCreatureAction_StealThingFromShop : public CCreatureActionBase { public:
    CBaseIntelligentPointer PThingToSteal;              // +0xa8
    virtual void PerformAction();
};

void CCreatureAction_StealThingFromShop::PerformAction()
{
    if (((CThing*)PThingToSteal.GetPItem()) && !((CThing*)PThingToSteal.GetPItem())->Dead) {
        CTCInventoryItem* inventoryItem;
        static_cast<const CThing*>(((CThing*)PThingToSteal.GetPItem()))->GetInterface(&inventoryItem, 0x1a);
        NInventory::CTCInventoryBase* inventory = (NInventory::CTCInventoryBase*)GetCreature()->GetCheckedTC(inventoryItem->GetInventoryType());
        if (!inventory->IsSpaceInInventoryForItem(((CThing*)PThingToSteal.GetPItem())->DefGlobalIndex)) {
            CCharString message;
            if (inventoryItem->GetMaxNumberOfItems() == 1)
                message = "You already have this item!";
            else
                message = "You don't have room in your inventory for this item!";
            CMainGameComponent* component = GetCreature()->PeekComponent();
            g_pPlayerGui->SubtitleRenderer.SetText(*component->PCutSceneMainFont, message, 0, 3.0f, false, true);
            return;
        }
        CThing* copy = CreateThingAt(((CThing*)PThingToSteal.GetPItem()), (CThing*)PCreature.GetPItem(), (CThing*)PCreature.GetPItem(), "");
        CTCInventoryItem* copyItem;
        copy->GetInterface(&copyItem, 0x1a);
        copyItem->AddToInventory(*inventory, false, true, 0, false, false);
        CTCOwnedEntity* owned;
        ((CThing*)PThingToSteal.GetPItem())->GetInterface(&owned, 0x33);
        CThing* owner = (CThing*)owned->POwner.GetPItem();
        CThing* thief = GetCreature();
        CMessageEventManager* manager = GetWorld().PMessageEventManager;
        manager->AddMessage(0x1c, 0x32, thief->GetPos(), GetCreature(), owner, 0.0f, 0.0f, 0);
        ((CThingCreatureBase*)GetCreature())->MakeNoise("SIMPLE_NOISE");
    }
}