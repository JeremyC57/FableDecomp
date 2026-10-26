// CAIStateGroup_JackDragon::Cleanup() -- kill the invisible helper creature if it is still alive.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009500A0, length 49 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on PInvisibleBodgeCreature at +0x44),
// CThing::Kill(bool) @ 0x004C9B80.
//
// Shape: the landed IS_THING_DEAD pattern (GetPItem null, then the dead bit +0x91 & 1) guards a
// `GetPItem()->Kill(true)`; `this` is only needed to form the member address (`lea esi,[ecx+44h]`). The
// engine header keeps the pointer as an opaque byte array, so the layout is modelled locally with its name.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool instant);                    // 0x004C9B80
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem();                         // 0x00A01B50
};

class CAIStateGroupBase { public:
    virtual void v00();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    unsigned char _pointers[0x30];              // +0x14 PEnemy, PLandedMarker, PHoverMarker1/2, PStrafeMarker1/2
    CBaseIntelligentPointer PInvisibleBodgeCreature;   // +0x44 CIntelligentPointer<CThingAICreature>
    void Cleanup();
};

void CAIStateGroup_JackDragon::Cleanup()
{
    if (PInvisibleBodgeCreature.GetPItem() != 0) {
        if (!PInvisibleBodgeCreature.GetPItem()->IsDead())
            PInvisibleBodgeCreature.GetPItem()->Kill(true);
    }
}