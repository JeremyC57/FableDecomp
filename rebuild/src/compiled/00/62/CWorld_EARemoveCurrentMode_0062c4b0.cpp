// CWorld::EARemoveCurrentMode(const CGameEvent*) -- game-event action: remove the player mode named in the event from the event player and reset the hero's projectile-weapon mode when the hero is alive and has ranged attacks
// Leaf: EARemoveCurrentMode
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Flags: /GS
// Retail: Fable.exe @ 0x0062C4B0, 189 bytes (/GS cookie 0x0139C8A8). PDB ?EARemoveCurrentMode@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"), CPlayer::GetPControlledCreature 0x00487DC0,
// CGameEvent::GetData 0x009F17A0 (label "CEventPacket::CopyToBuffer"), CMemoryBuffer::Read(unsigned long, void*)
// 0x009F18F0 (label "CircularBuffer_Read"), CPlayer::RemoveMode(EPlayerMode) 0x00634190, CInterfaceMap::LowerBound
// 0x0040F020, CTCHeroRangedAttacks::ResetProjectileWeaponMode 0x00789B30, __security_check_cookie 0x00BFE9F9.
// Layout (retail CThing): interface bitset +0x20 (TC 0x81 = CTCHeroRangedAttacks, word +0x30 bit 1), interface map
// +0x44, Dead +0x91 bit 0.
// Shape: player and hero are named locals fetched before the payload; the mode is read into the dead `event` slot inside its own
// block, so the later GetTC key reuses that slot.
// NF95 2026-11-19 (q11320): the unpacking buffer is Ego_r.pdb CGameEventDataPackage (engine/CGameEventDataPackage.h layout, 0x2C bytes)
//   read through ?GetData@CGameEventDataPackage@@QAEXJPAX@Z 0x009F18F0 (Ego_r.pdb 0x00B454F0 / FableWin.pdb 0x030A2200) and filled by
//   ?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z 0x009F17A0 (Ego_r.pdb 0x00B453F0); were the stand-ins
//   CMemoryBuffer::Read(unsigned long, void*) and CGameEvent::GetData(CMemoryBuffer&).
// NF97 2026-11-19 (port): the read buffer is the shared engine/CGameEventDataPackage.h class (FABLE_RETAIL_CGAMEEVENTDATAPACKAGE:
//   inline ctor zeroing Length / StartPos / EndPos, GetData 0x009F18F0), no longer a local model.
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#include "engine/CGameEventDataPackage.h"
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    void GetDataPackage(CGameEventDataPackage* package) const;     // 0x009F17A0
};

class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };

enum EPlayerMode { PLAYER_MODE_NONE = 0 };
class CTCHeroRangedAttacks { public:
    void ResetProjectileWeaponMode();              // 0x00789B30
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
    void RemoveMode(EPlayerMode mode);             // 0x00634190
};

class CWorld { public:
protected:
    CPlayer* GetPlayer(unsigned long number);               // 0x0049D850 ?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z (Ego_r/FableWin, protected)
public:
    void EARemoveCurrentMode(const CGameEvent* event);
};

void CWorld::EARemoveCurrentMode(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    CThing* hero = player->GetPControlledCreature();
    {
        CGameEventDataPackage buffer;
        event->GetDataPackage(&buffer);
        EPlayerMode mode;
        buffer.GetData(4, &mode);
        player->RemoveMode(mode);
    }
    if (hero && !hero->IsDead() && hero->HasTC(TCI_HERO_RANGED_ATTACKS))
        ((CTCHeroRangedAttacks*)&hero->GetTC(TCI_HERO_RANGED_ATTACKS))->ResetProjectileWeaponMode();
}