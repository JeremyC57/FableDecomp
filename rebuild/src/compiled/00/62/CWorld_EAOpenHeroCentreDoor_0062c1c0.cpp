// CWorld::EAOpenHeroCentreDoor(const CGameEvent*) -- game-event action: if the event player's hero is alive and not in the inventory screen, look up the door thing whose UID the event carries and tell its hero-centre component the hero walked through that door type
// Leaf: EAOpenHeroCentreDoor
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062C1C0, 305 bytes (320 with padding), `ret 4`, /GS (cookie 0x0139C8A8).
//   PDB ?EAOpenHeroCentreDoor@CWorld@@QAEXPBVCGameEvent@@@Z.
// Callees: CWorld::GetPlayer(long) 0x0049D850, CPlayer::GetPControlledCreature 0x00487DC0, the const interface-map LowerBound
//   0x004365B0 (the hero is a const CThing*), CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CWorld
//   virtual slot 3 GetThingManager, CThingManager::GetThingWithUID(__int64) 0x0051ED80, the non-const LowerBound
//   0x0040F020, CTCHeroCentre::OnHeroWalkedThroughDoor(EHeroCentreDoorType) 0x0077DB90, __security_check_cookie 0x00BFE9F9.
// Layout (retail CThing): interface bitset +0x20 (TC 0x11 CTCInventoryBase -> word 0 bit 17; TC 0x6A CTCHeroCentre ->
//   word +0x2C bit 10), interface map +0x44, Dead +0x91 bit 0; CTCInventoryBase InventoryOpen +0x58.
// Shape: the landed sibling EACloseHeroCentreDoor (0x0062C300): the inline PeekTC with an unset component when the
//   thing lacks the interface (VC reuses the door-type frame word on that path), both lookup keys in the dead `event`
//   slot; the inventory test is a nested `if` (esi pushed only on that path); door type then UID read in payload order.
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
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0 (the const instantiation)
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    CInterfaceMap Components;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                        // +0x91 bit 0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
    template<class T> __forceinline const T* PeekTC(long interfaceType) const
    {
        const T* component;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
    template<class T> __forceinline T* PeekTC(long interfaceType)
    {
        T* component;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
class CThingPlayerCreature : public CThing { };

struct CTCInventoryBase {                          // TC 0x11
    unsigned char _pad[0x58];
    bool          InventoryOpen;                   // +0x58 (Ego_r.pdb +0x60)
};
enum EHeroCentreDoorType { HERO_CENTRE_DOOR_NONE = 0 };
class CTCHeroCentre { public:                             // TC 0x6A
    void OnHeroWalkedThroughDoor(EHeroCentreDoorType door);   // 0x0077DB90
};
class CThingManager { public:
    CThing* GetThingWithUID(__int64 uid);          // 0x0051ED80
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };          // 0x00487DC0

class CWorld { public:
    virtual void _slot0();
    virtual void _slot1();
    virtual void _slot2();
    virtual CThingManager* GetThingManager();      // slot 3 (0x0049E1B0)
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAOpenHeroCentreDoor(const CGameEvent* event);
};

void CWorld::EAOpenHeroCentreDoor(const CGameEvent* event)
{
    const CThing* hero = GetPlayer(event->Player)->GetPControlledCreature();
    if (hero && !hero->Dead)
    {
        if (!hero->PeekTC<CTCInventoryBase>(0x11)->InventoryOpen)
        {
            CMemoryBuffer buffer;
            event->GetData(buffer);
            EHeroCentreDoorType door;
            buffer.Read(4, &door);
            __int64 uid;
            buffer.Read(8, &uid);
            CThing* thing = GetThingManager()->GetThingWithUID(uid);
            if (thing)
                thing->PeekTC<CTCHeroCentre>(0x6A)->OnHeroWalkedThroughDoor(door);
        }
    }
}