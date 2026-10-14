// CCreatureAction_SummonerFlameSlice::FinishAction() -- base FinishAction, then if the summoned thing (+0x114) is alive (pointer set, dead bit clear), kill it silently
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CAD10, length 64 bytes. FinishAction is vtable slot 4 (protected `MAE`); the base is called directly here.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the member at +0x114, three
// times: the landed IS_THING_ALIVE idiom re-fetches, then the call target), CThing::Kill(bool) @ 0x004C9B80.
// Shape: `this` rides in esi for the base call and is then advanced to the member (`add esi,114h`); the body of the landed 0x008445A0 without the range test.
struct CThing {                             // only the dead bit and the call are modelled
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool silent);                 // 0x004C9B80
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _body[0x114 - 4];
};

struct CCreatureAction_SummonerFlameSlice : CCombatActionBase {   // vtable 0x01264214
    CBaseIntelligentPointer PSummonedThing; // +0x114 (default-built by the constructor @ 0x008CBCA0)
    virtual void FinishAction();
};

void CCreatureAction_SummonerFlameSlice::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PSummonedThing))
        PSummonedThing.GetPItem()->Kill(true);
}
