// CWorld::EACloseHeroCentreDoor(const CGameEvent*) -- game-event action: if the event player's hero is alive, look up the thing whose UID the event carries and tell its hero-centre component that the hero closed the door
// Leaf: EACloseHeroCentreDoor
// Flags: /GS
// Retail: Fable.exe @ 0x0062C300, 216 bytes (/GS cookie 0x0139C8A8). PDB ?EACloseHeroCentreDoor@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"), CPlayer::GetPControlledCreature 0x00487DC0,
// CGameEvent::GetData 0x009F17A0 (label "CEventPacket::CopyToBuffer"), CMemoryBuffer::Read(unsigned long, void*)
// 0x009F18F0 (label "CircularBuffer_Read"), CWorld virtual slot 3 GetThingManager (vtable 0x012390F0 -> 0x0049E1B0),
// CThingManager::GetThingWithUID(__int64) 0x0051ED80 (label "CStopWatch::GetTicks"), CInterfaceMap::LowerBound
// 0x0040F020, CTCHeroCentre::OnHeroCloseDoor 0x0077D3E0, __security_check_cookie 0x00BFE9F9.
// Layout (retail CThing): interface bitset +0x20 (TC 0x6A = CTCHeroCentre, word +0x2C bit 10 -> `test ch,4`),
// interface map +0x44, Dead +0x91 bit 0.
// Shape: retail does not check HasTC before calling the component: the inline PeekTC returns an unset local when the
// thing lacks the component (VC reads the dead UID frame word on that path); the lookup key uses the dead `event` slot.
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
    template<class T> __forceinline T* PeekTC(long interfaceType)
    {
        T* component;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
class CThingPlayerCreature : public CThing { };

class CTCHeroCentre { public:
    void OnHeroCloseDoor();                        // 0x0077D3E0
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
    void EACloseHeroCentreDoor(const CGameEvent* event);
};

void CWorld::EACloseHeroCentreDoor(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetPControlledCreature();
    if (hero && !hero->Dead)
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        __int64 uid;
        buffer.Read(8, &uid);
        CThing* door = GetThingManager()->GetThingWithUID(uid);
        if (door)
            door->PeekTC<CTCHeroCentre>(0x6A)->OnHeroCloseDoor();
    }
}