// CTCInventoryMap::TeleportHeroToTeleporter(CThing* teleporter) -- while the hero creature is alive, sends it through the teleporter thing's teleporter component
// Leaf: ?TeleportHeroToTeleporter@CTCInventoryMap@@QAEXPAVCThing@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FBD20, length 98 bytes (+ int3), `ret 4`; `this` only feeds the player-manager getter.
// Callees: CTCBase::PeekPlayerManager() const @ 0x00686D50 (manifest "CViewBase::GetDisplayEngine", propagated),
// CPlayerManager::GetMainPlayer @ 0x00449960, CPlayer::PeekPControlledCreature @ 0x00487DD0 (manifest a propagated _Cons_val),
// CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface for interface 0xDD: bit 29 of the bitset dword at +0x38),
// CTCTeleporter::TeleportHero() @ 0x007DFCF0.
// Shape: the inline bitset test (`test dword ptr [eax+38h],20000000h`) and the GetInterface key homed in the dead `teleporter`
// argument slot ([esp+8] after `push esi`); every miss shares the single `ret 4`.
// Layout: CThing flag byte +0x91 bit 0 (dead), interface bitset +0x20, interface map +0x44 (local models).
struct CTCBase_Any;
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;      // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCTeleporter { public:                          // interface type 0xDD
    void TeleportHero();                        // 0x007DFCF0
};

class CTCBase
{
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryMap : public NInventory::CTCInventoryBase { public:
    void TeleportHeroToTeleporter(CThing* teleporter);
};

void CTCInventoryMap::TeleportHeroToTeleporter(CThing* teleporter)
{
    const CThing* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead()) {
        CTCTeleporter* teleporterTC;
        if (teleporter->GetTC(TCI_TELEPORTER, &teleporterTC))
            teleporterTC->TeleportHero();
    }
}