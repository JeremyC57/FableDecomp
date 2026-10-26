// CTCInventoryMap::TeleportHeroToTeleporter(CThing* teleporter) -- while the hero creature is alive, sends it through the teleporter thing's teleporter component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FBD20, length 98 bytes (+ int3), `ret 4`; `this` only feeds the player-manager getter.
// Callees: CTCBase::PeekPlayerManager() const @ 0x00686D50 (manifest "CViewBase::GetDisplayEngine", propagated),
// CPlayerManager::GetMainPlayer @ 0x00449960, CPlayer::GetControlledCreature @ 0x00487DD0 (manifest a propagated _Cons_val),
// CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface for interface 0xDD: bit 29 of the bitset dword at +0x38),
// CTCTeleporter::TeleportHero() @ 0x007DFCF0.
// Shape: the inline bitset test (`test dword ptr [eax+38h],20000000h`) and the GetInterface key homed in the dead `teleporter`
// argument slot ([esp+8] after `push esi`); every miss shares the single `ret 4`.
// Layout: CThing flag byte +0x91 bit 0 (dead), interface bitset +0x20, interface map +0x44 (local models).
struct CTCBase_Any;
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char IsDead : 1;                   // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CPlayer { public:
    CThing* GetControlledCreature() const;      // 0x00487DD0
};
class CPlayerManager { public:
public:
    const CPlayer& GetMainPlayer() const;   // 0x00449960
};
class CTCTeleporter { public:                          // interface type 0xDD
    void TeleportHero();                        // 0x007DFCF0
};

class CTCBase
{
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryMap : public NInventory::CTCInventoryBase { public:
    void TeleportHeroToTeleporter(CThing* teleporter);
};

void CTCInventoryMap::TeleportHeroToTeleporter(CThing* teleporter)
{
    CThing* hero = PeekPlayerManager().GetMainPlayer().GetControlledCreature();
    if (hero && !hero->IsDead) {
        CTCTeleporter* teleporterTC;
        if (teleporter->GetInterface(&teleporterTC, 0xDD))
            teleporterTC->TeleportHero();
    }
}