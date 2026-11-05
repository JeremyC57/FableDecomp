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
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
};
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
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
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EACreatureSprint(const CGameEvent* event);
};

void CWorld::EACreatureSprint(const CGameEvent* event)
{
    bool sprint;
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        long value;
        buffer.Read(4, &value);
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