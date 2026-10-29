// CCreatureAction_SummonerFlameSlice::FrameUpdate() -- base FrameUpdate, then while the flame-slice thing is alive and carries its obstruction (interface type 0xed), stretch the obstruction over the summoner (start, end, twice the summoner's radius)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CAF10, length 130 bytes (+ int3 padding).
// Callees: CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the slice thing at +0x114
// three times, PCreature at +8 three times), CThing::GetPos @ 0x004C73D0 (const C3DVector&, twice), the obstruction shape setter @ 0x007FD3F0
// (start -> +0x0c, end -> +0x18, width -> +0x28; the manifest "CInputManager::UpdateChecksum" label is propagated).
// Shape: the landed IS_THING_ALIVE idiom, then the slice thing held from its third fetch: the inlined HasTC probe (`mov ecx,[eax+3Ch] ; test ch,20h`
// = type 0xed) and its obstruction pointer (+0x5c) read straight away into edi. The setter is reached through an inline forwarder whose by-value
// arguments are evaluated first, right to left (the summoner's placement +0x60 into ebx, the summoner twice into ebp/esi); inside it the width
// `radius * 2.0f` (`fadd st(0),st(0)`) is pushed, then the two positions.
// No engine header for this class matches retail (CCombatActionBase is 0x114 bytes); modelled locally.
class C3DVector { public: float X, Y, Z; };
struct CTCPlacement {
    unsigned char _pad0[0x34];
    float m_radius;                         // +0x34
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    unsigned char _pad44[0x5c - 0x44];
    struct CObstructionShape* m_pObstruction;   // +0x5c
    CTCPlacement* m_pPlacement;             // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    const C3DVector& GetPos() const;   // 0x004C73D0
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct CObstructionShape {
    void Set(const C3DVector& start, const C3DVector& end, float width);   // 0x007FD3F0
    void SetBetween(CThing* start, CThing* end, CTCPlacement* placement)
    {
        Set(start->GetPos(), end->GetPos(), placement->m_radius * 2.0f);
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseObject* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void FrameUpdate();             // 0x00859D80
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};
class CCreatureAction_SummonerFlameSlice : public CCombatActionBase { public:
    CBaseIntelligentPointer PFlameSlice;    // +0x114 CIntelligentPointer<CThing>
    virtual void FrameUpdate();
};

void CCreatureAction_SummonerFlameSlice::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    if (IS_THING_ALIVE(PFlameSlice)) {
        CThing* slice = ((CThing*)PFlameSlice.GetPItem());
        if (slice->HasTC(0xed)) {
            CObstructionShape* obstruction = slice->m_pObstruction;
            obstruction->SetBetween(((CThing*)PCreature.GetPItem()), ((CThing*)PCreature.GetPItem()), ((CThing*)PCreature.GetPItem())->m_pPlacement);
        }
    }
}