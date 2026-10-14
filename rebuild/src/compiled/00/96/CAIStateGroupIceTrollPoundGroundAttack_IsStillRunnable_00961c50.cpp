// CAIStateGroup_IceTrollPoundGroundAttack::IsStillRunnable() -- keep pounding while the target is alive and in range.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961C50, length 70 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on PTarget at +0x14),
// IsThingInRangeOfPound(CThing* creature, CThing* target) @ 0x00961B90 (__fastcall: ecx = creature, edx = target).
//
// Shape: the liveness test is the landed IS_THING_DEAD pattern (GetPItem null, then the dead bit +0x91 & 1,
// nested ifs with one shared false tail), then the brain's creature is read into a local BEFORE the third
// GetPItem (so it survives the call in edi) and the range predicate decides. `mov al,1` / `xor al,al`: bool.
// The engine header keeps PTarget as an opaque byte array, so the layout is modelled locally with its name.
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem();                         // 0x00A01B50
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    bool IsStopping();                          // 0x008FD010
};

extern bool __fastcall IsThingInRangeOfPound(CThing* pCreature, CThing* pTarget);   // 0x00961B90

struct CAIStateGroup_IceTrollPoundGroundAttack : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14 CIntelligentPointer<CThing>
    bool IsStillRunnable();
};

bool CAIStateGroup_IceTrollPoundGroundAttack::IsStillRunnable()
{
    if (PTarget.GetPItem() != 0) {
        if (!PTarget.GetPItem()->IsDead()) {
            CThing* pCreature = PBrain->PCreature;
            if (IsThingInRangeOfPound(pCreature, PTarget.GetPItem()))
                return true;
        }
    }
    return false;
}