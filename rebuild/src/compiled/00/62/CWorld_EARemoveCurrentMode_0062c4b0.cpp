// CWorld::EARemoveCurrentMode(const CGameEvent*) -- game-event action: remove the player mode named in the event from the event player and reset the hero's projectile-weapon mode when the hero is alive and has ranged attacks
// Leaf: EARemoveCurrentMode
// Flags: /GS
// Retail: Fable.exe @ 0x0062C4B0, 189 bytes (/GS cookie 0x0139C8A8). PDB ?EARemoveCurrentMode@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"), CPlayer::GetHeroThing 0x00487DC0,
// CGameEvent::GetData 0x009F17A0 (label "CEventPacket::CopyToBuffer"), CMemoryBuffer::Read(unsigned long, void*)
// 0x009F18F0 (label "CircularBuffer_Read"), CPlayer::RemoveMode(EPlayerMode) 0x00634190, CInterfaceMap::LowerBound
// 0x0040F020, CTCHeroRangedAttacks::ResetProjectileWeaponMode 0x00789B30, __security_check_cookie 0x00BFE9F9.
// Layout (retail CThing): interface bitset +0x20 (TC 0x81 = CTCHeroRangedAttacks, word +0x30 bit 1), interface map
// +0x44, Dead +0x91 bit 0.
// Shape: player and hero are named locals fetched before the payload; the mode is read into the dead `event` slot inside its own
// block, so the later GetTC key reuses that slot.
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
};

enum EPlayerMode { PLAYER_MODE_NONE = 0 };
class CTCHeroRangedAttacks { public:
    void ResetProjectileWeaponMode();              // 0x00789B30
};
class CPlayer { public:
    CThing* GetHeroThing();                        // 0x00487DC0
    void RemoveMode(EPlayerMode mode);             // 0x00634190
};

class CWorld { public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EARemoveCurrentMode(const CGameEvent* event);
};

void CWorld::EARemoveCurrentMode(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    CThing* hero = player->GetHeroThing();
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        EPlayerMode mode;
        buffer.Read(4, &mode);
        player->RemoveMode(mode);
    }
    if (hero && !hero->Dead && hero->HasTC(0x81))
        hero->GetTC<CTCHeroRangedAttacks>(0x81)->ResetProjectileWeaponMode();
}