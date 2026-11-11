// CWorld::EAMoveSpirit(const CGameEvent*) -- game-event action: if the event player's hero carries a co-op spirit component, read a 2D movement vector from the event and apply it to the spirit
// Leaf: EAMoveSpirit
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Flags: /GS
// Retail: Fable.exe @ 0x0062C0E0, 215 bytes (/GS cookie 0x0139C8A8). PDB ?EAMoveSpirit@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"), CPlayer::GetPControlledCreature 0x00487DC0 (twice),
// CInterfaceMap::LowerBound 0x0040F020, CGameEvent::GetData 0x009F17A0 (label "CEventPacket::CopyToBuffer"),
// CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0 (label "CircularBuffer_Read"),
// CTCCoopSpirit::ApplyMovementVector(const C3DVector&) 0x0066FAE0, __security_check_cookie 0x00BFE9F9.
// Layout (retail CThing): interface bitset +0x20 (TC 0xC7 = CTCCoopSpirit, word +0x38 bit 7 -> the byte sign test),
// interface map +0x44.
// Shape: no IS_THING_ALIVE here: the hero is fetched twice (null test, then a named local for HasTC/GetTC); the component is
// looked up before the payload buffer is built; X lands in the dead `event` slot (also the lookup key) and the
// movement vector (X, Y, 0) is a named C3DVector.
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

#include "engine/C3DVector.h"
class CTCCoopSpirit { public:
    void ApplyMovementVector(const C3DVector& movement);   // 0x0066FAE0
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };          // 0x00487DC0

class CWorld { public:
protected:
    CPlayer* GetPlayer(unsigned long number);               // 0x0049D850 ?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z (Ego_r/FableWin, protected)
public:
    void EAMoveSpirit(const CGameEvent* event);
};

void CWorld::EAMoveSpirit(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->GetPControlledCreature())
    {
        CThing* hero = player->GetPControlledCreature();
        if (hero->HasTC(TCI_COOP_SPIRIT))
        {
            CTCCoopSpirit* spirit = ((CTCCoopSpirit*)&hero->GetTC(TCI_COOP_SPIRIT));
            {
                CMemoryBuffer buffer;
                event->GetData(buffer);
                float x;
                buffer.Read(4, &x);
                float y;
                buffer.Read(4, &y);
                C3DVector movement(x, y, 0.0f);
                spirit->ApplyMovementVector(movement);
            }
        }
    }
}