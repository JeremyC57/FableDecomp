// CCreatureAction_TentacleStrikeLoop::FrameUpdate() -- interrupt the strike (Interrupt = true, virtual FinishAction) when the owner's health dropped below StartHealth or the tentacle's kraken reports its target hit; then the base FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF460, length 212 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xE3 =
// CTCKrakenTentacle, bit 3 of +0x3c, then 0xE2 = CTCKraken on the kraken, bit 2 of +0x3c), CTCKraken::HasTargetBeenHitByKraken
// @ 0x007FB500, the action's virtual FinishAction (slot 4, +0x10; 0x008CF7D0 in this class's vtable 0x01284EF4),
// CCombatActionBase::FrameUpdate @ 0x00859D80.
// Shape: the tentacle component comes from the UNCHECKED out-parameter lookup in an inline accessor (absent arm reads the key
// word); `StartHealth > health` loads StartHealth first (`test ah,41h ; je` = ordered greater); the kraken is the tentacle's
// CIntelligentPointer at +0x10, fetched once for the present / not dead test (inline IsThingAlive), then the CHECKED lookup feeds the bool call (`||` chain).
// Layout: owner CIntelligentPointer +8; Ego_r.pdb StartTick / Interrupt / StartHealth +0x12c / +0x130 / +0x134, retail
// +0x114 / +0x118 / +0x11c (CCombatActionBase is 0x114 in retail); the creature's health float at +0xb4.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

static inline bool IsThingAlive(const CThing* thing)    // one fetch for the null and dead tests
{
    return thing && !(thing->m_flags91 & 1);
}

class CTCKraken { public:
    bool HasTargetBeenHitByKraken();            // 0x007FB500
};
class CTCKrakenTentacle { public:
    unsigned char           _pad[0x10];
    CBaseIntelligentPointer PKraken;            // +0x10
};

class CCombatActionBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void FinishAction();                // slot 4 (+0x10)
    virtual void FrameUpdate();                 // 0x00859D80
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCKrakenTentacle* GetTentacle() const
    {
        CTCKrakenTentacle* tentacle;
        PCreature.GetPItem()->GetInterface(&tentacle, 0xe3);
        return tentacle;
    }
};

class CCreatureAction_TentacleStrikeLoop : public CCombatActionBase { public:
    long  StartTick;                            // +0x114 (PDB +0x12c)
    bool  Interrupt;                            // +0x118 (PDB +0x130)
    float StartHealth;                          // +0x11c (PDB +0x134)
    virtual void FrameUpdate();
};

void CCreatureAction_TentacleStrikeLoop::FrameUpdate()
{
    CTCKrakenTentacle* tentacle = GetTentacle();
    CTCKraken* kraken;
    if (StartHealth > PCreature.GetPItem()->m_health ||
        (IsThingAlive(tentacle->PKraken.GetPItem()) &&
         tentacle->PKraken.GetPItem()->GetInterface(&kraken, 0xe2) && kraken->HasTargetBeenHitByKraken())) {
        Interrupt = true;
        FinishAction();
    }
    CCombatActionBase::FrameUpdate();
}