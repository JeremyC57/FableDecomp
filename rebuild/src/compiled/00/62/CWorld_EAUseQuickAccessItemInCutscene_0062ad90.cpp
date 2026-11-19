// CWorld::EAUseQuickAccessItemInCutscene(const CGameEvent*) -- game-event action: if the event player's hero is alive and has an inventory component, read the quick-access slot from the event and hand it to the inventory
// Leaf: EAUseQuickAccessItemInCutscene
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Flags: /GS
// Retail: Fable.exe @ 0x0062AD90, 184 bytes (/GS cookie 0x0139C8A8). PDB ?EAUseQuickAccessItemInCutscene@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"), CPlayer::GetPControlledCreature 0x00487DC0,
// CGameEvent::GetData 0x009F17A0 (label "CEventPacket::CopyToBuffer"), CInterfaceMap::LowerBound 0x0040F020,
// CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0 (label "CircularBuffer_Read"), the inventory's quick-access
// handler 0x005819A7 (a `ret 4` stub in retail; manifest "$E2", name descriptive), __security_check_cookie 0x00BFE9F9.
// Layout (retail CThing): interface bitset +0x20 (TC 0x11 = CTCInventory, bit 0x20000 of word 0), interface map
// +0x44, Dead +0x91 bit 0.
// Shape: the payload buffer is filled before the HasTC test; the lookup key and the slot read both use the dead
// `event` argument slot; GetTC is the inline lower-bound lookup with the component kept in esi.
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

class CTCInventory { public:
    void UseQuickAccessItemInCutscene(long slot);  // 0x005819A7
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };          // 0x00487DC0

class CWorld { public:
protected:
    CPlayer* GetPlayer(unsigned long number);               // 0x0049D850 ?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z (Ego_r/FableWin, protected)
public:
    void EAUseQuickAccessItemInCutscene(const CGameEvent* event);
};

void CWorld::EAUseQuickAccessItemInCutscene(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CGameEventDataPackage buffer;
        event->GetDataPackage(&buffer);
        if (hero->HasTC(TCI_INVENTORY))
        {
            CTCInventory* inventory = ((CTCInventory*)&hero->GetTC(TCI_INVENTORY));
            if (inventory)
            {
                long slot;
                buffer.GetData(4, &slot);
                inventory->UseQuickAccessItemInCutscene(slot);
            }
        }
    }
}