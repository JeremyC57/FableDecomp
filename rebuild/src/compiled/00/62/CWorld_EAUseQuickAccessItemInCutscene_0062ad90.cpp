// CWorld::EAUseQuickAccessItemInCutscene(const CGameEvent*) -- game-event action: if the event player's hero is alive and has an inventory component, read the quick-access slot from the event and hand it to the inventory
// Leaf: EAUseQuickAccessItemInCutscene
// Flags: /GS
// Retail: Fable.exe @ 0x0062AD90, 184 bytes (/GS cookie 0x0139C8A8). PDB ?EAUseQuickAccessItemInCutscene@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"), CPlayer::GetHeroThing 0x00487DC0,
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

class CTCInventory { public:
    void UseQuickAccessItemInCutscene(long slot);  // 0x005819A7
};
class CPlayer { public: CThing* GetHeroThing(); };          // 0x00487DC0

class CWorld { public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAUseQuickAccessItemInCutscene(const CGameEvent* event);
};

void CWorld::EAUseQuickAccessItemInCutscene(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetHeroThing();
    if (hero && !hero->Dead)
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        if (hero->HasTC(0x11))
        {
            CTCInventory* inventory = hero->GetTC<CTCInventory>(0x11);
            if (inventory)
            {
                long slot;
                buffer.Read(4, &slot);
                inventory->UseQuickAccessItemInCutscene(slot);
            }
        }
    }
}