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

class CTCInventory { public:
    void UseQuickAccessItemInCutscene(long slot);  // 0x005819A7
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };          // 0x00487DC0

class CWorld { public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAUseQuickAccessItemInCutscene(const CGameEvent* event);
};

void CWorld::EAUseQuickAccessItemInCutscene(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        if (hero->HasTC(TCI_INVENTORY))
        {
            CTCInventory* inventory = ((CTCInventory*)&hero->GetTC(TCI_INVENTORY));
            if (inventory)
            {
                long slot;
                buffer.Read(4, &slot);
                inventory->UseQuickAccessItemInCutscene(slot);
            }
        }
    }
}