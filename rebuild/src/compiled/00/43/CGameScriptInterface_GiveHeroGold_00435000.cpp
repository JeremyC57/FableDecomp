// CGameScriptInterface::GiveHeroGold(long amount) -- when the main player's creature is alive and has its money component (interface type 0x8e), hand it the gold amount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00435000, length 98 bytes (+ int3 padding). `ret 4`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449960 (label "Catch@0040d959" is wrong; `GetPlayer([ecx+1Ch])`, as the landed
// CTCFishing::GoFishing 0x007B1EB0), CPlayer::PeekPControlledCreature @ 0x00487DD0 (label "_Cons_val<...>" propagated),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the inlined GetInterface for type 0x8e = bit 14 of the bitset word
// at +0x30, `test ch,40h`), the money component's gold call @ 0x00639E40 (label "CTCRegionDisplay::GlobalDraw" propagated; named
// descriptively).
// Layout: engine/CGameScriptInterface.h (PDB) puts PlayerManager at +0x14; retail reads it at +0xc, so `this` is modelled locally.
// Shape: the hero is a local (the alive idiom on a register), then the out-parameter GetInterface template as the condition (its out
// pointer shares the key's frame word); the amount is re-read from its slot after the lookup.
class CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_statusFlags;            // +0x91 (bit 0: dead)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };

struct CTCHeroMoney {                       // interface type 0x8e
    void GiveGold(long amount);             // 0x00639E40
};

class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;               // 0x00449960
};

class CGameScriptInterface { public:
    void*           __vftable;              // +0x00
    unsigned char   _pad04[8];              // +0x04
    CPlayerManager* PlayerManager;          // +0x0c (retail)
    void GiveHeroGold(long amount);
};

void CGameScriptInterface::GiveHeroGold(long amount)
{
    const CThing* hero = PlayerManager->GetMainPlayer()->PeekPControlledCreature();
    if (hero && !(hero->m_statusFlags & 1)) {
        CTCHeroMoney* money;
        if (hero->GetInterface(&money, 0x8e))
            money->GiveGold(amount);
    }
}