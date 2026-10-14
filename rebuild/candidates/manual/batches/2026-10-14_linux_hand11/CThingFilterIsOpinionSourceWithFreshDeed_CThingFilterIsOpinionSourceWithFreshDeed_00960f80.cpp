// CThingFilter_IsOpinionSourceWithFreshDeed::CThingFilter_IsOpinionSourceWithFreshDeed(CThingAICreature*, long lastVDeedID, unsigned __int64 lastSourceUID) -- constructor: PCreature (CIntelligentPointer at +0) from the pointer, LastVDeedID +8, LastSourceUID +0x10 (two dwords)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00960F80, length 64 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// The manifest label (CActiveDialogueInfo) is propagated: the layout written (intelligent pointer
// at +0, a long at +8, an 8-byte value at +0x10 with +0xc untouched padding, ret 0x10 = pointer +
// long + 64-bit value) is the PDB's CThingFilter_IsOpinionSourceWithFreshDeed {PCreature,
// LastVDeedID, LastSourceUID}. The vtable stored at +0 (0x0123E7E0) is CIntelligentPointer's own
// (a member; the filter object itself is not polymorphic). The 64-bit argument's halves are stored
// from two registers.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PItem;                                          // +0x04 (the pointer is 8 bytes)
    CBaseIntelligentPointer();                                   // 0x00A01B10
    void SetPItem(CBaseObject* item);                            // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {           // vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CThingFilter_IsOpinionSourceWithFreshDeed {
    CIntelligentPointer PCreature;                               // +0x00 CIntelligentPointer<CThingAICreature>
    long                LastVDeedID;                             // +0x08
    unsigned __int64    LastSourceUID;                           // +0x10
    CThingFilter_IsOpinionSourceWithFreshDeed(CBaseObject* creature, long lastVDeedID, unsigned __int64 lastSourceUID);
};

CThingFilter_IsOpinionSourceWithFreshDeed::CThingFilter_IsOpinionSourceWithFreshDeed(CBaseObject* creature, long lastVDeedID, unsigned __int64 lastSourceUID)
    : PCreature(creature)
    , LastVDeedID(lastVDeedID)
    , LastSourceUID(lastSourceUID)
{
}
