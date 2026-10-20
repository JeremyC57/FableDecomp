// CWorld::EAMoveSpirit(const CGameEvent*) -- game-event action: if the event player's hero carries a co-op spirit component, read a 2D movement vector from the event and apply it to the spirit
// Leaf: EAMoveSpirit
// Flags: /GS
// Retail: Fable.exe @ 0x0062C0E0, 215 bytes (/GS cookie 0x0139C8A8). PDB ?EAMoveSpirit@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"), CPlayer::GetHeroThing 0x00487DC0 (twice),
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
struct CGameEvent {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

struct CThing {
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
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CTCCoopSpirit {
    void ApplyMovementVector(const C3DVector& movement);   // 0x0066FAE0
};
struct CPlayer { CThing* GetHeroThing(); };          // 0x00487DC0

struct CWorld {
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAMoveSpirit(const CGameEvent* event);
};

void CWorld::EAMoveSpirit(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->GetHeroThing())
    {
        CThing* hero = player->GetHeroThing();
        if (hero->HasTC(0xC7))
        {
            CTCCoopSpirit* spirit = hero->GetTC<CTCCoopSpirit>(0xC7);
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
