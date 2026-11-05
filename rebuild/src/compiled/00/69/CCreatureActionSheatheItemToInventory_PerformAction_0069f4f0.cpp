// CCreatureAction_SheatheItemToInventory::PerformAction() -- take the item out of the creature's primary carry slot (putting out its fire when it is a live flammable), refresh the sheathed weapons, cancel any melee flourish, restart the current creature mode and, for the main player's creature, drop the player's aggressive mode
// Leaf: ?PerformAction@CCreatureAction_SheatheItemToInventory@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F4F0, length 379 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8), CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x46
// CTCCarrying, bit 6 of +0x28; 0x6D CTCFlammable on the item, bit 13 of +0x2c; 0x13 CTCInventoryWeapons, bit 19 of +0x20; 0x30
// CTCCombat, bit 16 of +0x24; 0x31 CTCCreatureModeManager, bit 17 of +0x24), CTCCarrying::RemoveThingFromPrimarySlot(bool)
// @ 0x006A1E00, CTCFlammable::ExtinguishFire @ 0x00780C80, CTCInventoryWeapons::RefreshSheathedWeapons @ 0x005C9962,
// CTCCombat::SetMeleeFlourishValid(bool) @ 0x00733E10, CTCCreatureModeManager::RestartCurrentMode @ 0x0073A840 (manifest
// "CNavQuadTreeNode::RemoveNeighbours" is propagated), CThing::GetPlayer @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520,
// CPlayer::SetAggressiveMode(bool) @ 0x00487EB0.
// Shape: the carrying and mode-manager components are zero-preloaded `if (HasTC) GetTC` on a creature fetched once per lookup
// (`xor esi,esi` before the GetPItem call); the other three lookups are tested blocks; GetTC as the end/hit ternary (component in ecx) except the CTCCombat one, whose argument push wants the out-parameter GetInterface form (component in eax).
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CPlayer { public:
    void SetAggressiveMode(bool aggressive);    // 0x00487EB0
public:
    bool IsMainPlayer() const;   // 0x00487520
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
class CTCCarrying { public:
    CThing* RemoveThingFromPrimarySlot(bool drop);   // 0x006A1E00
};
class CTCFlammable { public:
    void ExtinguishFire();                      // 0x00780C80
};
class CTCInventoryWeapons { public:
    void RefreshSheathedWeapons();              // 0x005C9962
};
class CTCCombat { public:
    void SetMeleeFlourishValid(bool valid);     // 0x00733E10
};
class CTCCreatureModeManager { public:
    void RestartCurrentMode();                  // 0x0073A840
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:
    void PerformAction();
};

void CCreatureAction_SheatheItemToInventory::PerformAction()
{
    CTCCarrying* carrying = 0;
    {
        CThing* creature = PCreature.GetPItem();
        if (creature->HasTC(TCI_CARRYING))
            carrying = ((CTCCarrying*)&creature->GetTC(TCI_CARRYING));
    }
    CThing* item = carrying->RemoveThingFromPrimarySlot(false);
    if (item && !item->IsDead() && item->HasTC(TCI_FLAMMABLE))
        ((CTCFlammable*)&item->GetTC(TCI_FLAMMABLE))->ExtinguishFire();
    {
        CThing* creature = PCreature.GetPItem();
        if (creature->HasTC(TCI_INVENTORY_WEAPONS))
            ((CTCInventoryWeapons*)&creature->GetTC(TCI_INVENTORY_WEAPONS))->RefreshSheathedWeapons();
    }
    CTCCombat* combat;
    if (PCreature.GetPItem()->GetTC(TCI_COMBAT, &combat))
        combat->SetMeleeFlourishValid(false);
    CTCCreatureModeManager* modeManager = 0;
    {
        CThing* creature = PCreature.GetPItem();
        if (creature->HasTC(TCI_ENTITY_MODE_MANAGER))
            modeManager = ((CTCCreatureModeManager*)&creature->GetTC(TCI_ENTITY_MODE_MANAGER));
    }
    modeManager->RestartCurrentMode();
    if (PCreature.GetPItem()->GetPlayer().IsMainPlayer())
        PCreature.GetPItem()->GetPlayer().SetAggressiveMode(false);
}