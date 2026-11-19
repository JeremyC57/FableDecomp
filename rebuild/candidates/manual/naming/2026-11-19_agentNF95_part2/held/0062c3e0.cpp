// CWorld::EACreatureSprint(const CGameEvent*) -- game-event action: read the sprint flag from the event and, if the event player's hero is alive and has the special-movement component, start or stop sprinting when the state changes
// Leaf: EACreatureSprint
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Flags: /GS
// Retail: Fable.exe @ 0x0062C3E0, 208 bytes (/GS cookie 0x0139C8A8). PDB ?EACreatureSprint@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData 0x009F17A0 (label "CEventPacket::CopyToBuffer"), CMemoryBuffer::Read(unsigned long,
// void*) 0x009F18F0 (label "CircularBuffer_Read"), CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"),
// CPlayer::GetPControlledCreature 0x00487DC0, CInterfaceMap::LowerBound 0x0040F020, CTCHeroSpecialMovement::SetSprinting(bool)
// 0x007E0670, __security_check_cookie 0x00BFE9F9.
// Layout (retail CThing): interface bitset +0x20 (TC 0xDF = CTCHeroSpecialMovement, word +0x38 bit 31 -> sign test),
// interface map +0x44, Dead +0x91 bit 0; CTCHeroSpecialMovement::Sprinting +0x0C (PDB).
// Shape: the payload is a 4-byte flag read into the dead `event` slot and normalised to a bool (bl) inside its own block
// (so the GetTC key can reuse the slot) before the player lookup; the two SetSprinting calls (true/false) share one call after the pushes.
// NF95 2026-11-19 (q11320): the unpacking buffer is Ego_r.pdb CGameEventDataPackage (engine/CGameEventDataPackage.h layout, 0x2C bytes)
//   read through ?GetData@CGameEventDataPackage@@QAEXJPAX@Z 0x009F18F0 (Ego_r.pdb 0x00B454F0 / FableWin.pdb 0x030A2200) and filled by
//   ?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z 0x009F17A0 (Ego_r.pdb 0x00B453F0); were the stand-ins
//   CMemoryBuffer::Read(unsigned long, void*) and CGameEvent::GetData(CMemoryBuffer&).
class CGameEventDataPackage { public:
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CGameEventDataPackage() : Length(0), ReadPos(0), WritePos(0) { }
    void GetData(long size, void* data);           // 0x009F18F0
};
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

class CTCHeroSpecialMovement { public:
    unsigned char _pad00[0x0c];
    bool          Sprinting;                       // +0x0C
    bool          Running;                         // +0x0D
    void SetSprinting(bool sprinting);             // 0x007E0670
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };          // 0x00487DC0

class CWorld { public:
protected:
    CPlayer* GetPlayer(unsigned long number);               // 0x0049D850 ?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z (Ego_r/FableWin, protected)
public:
    void EACreatureSprint(const CGameEvent* event);
};

void CWorld::EACreatureSprint(const CGameEvent* event)
{
    bool sprint;
    {
        CGameEventDataPackage buffer;
        event->GetDataPackage(&buffer);
        long value;
        buffer.GetData(4, &value);
        sprint = value != 0;
    }
    CThing* hero = GetPlayer(event->Player)->GetPControlledCreature();
    if (hero && !hero->IsDead() && hero->HasTC(TCI_HERO_SPECIAL_MOVEMENT))
    {
        CTCHeroSpecialMovement* movement = ((CTCHeroSpecialMovement*)&hero->GetTC(TCI_HERO_SPECIAL_MOVEMENT));
        if (sprint)
        {
            if (!movement->Sprinting)
                movement->SetSprinting(true);
        }
        else if (movement->Sprinting)
            movement->SetSprinting(false);
    }
}