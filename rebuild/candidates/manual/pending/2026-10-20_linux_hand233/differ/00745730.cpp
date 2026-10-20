// CTCPerceiveThing::UpdatePerception() -- an incapacitated parent loses all three lines of sight and its smell / hearing / direct awareness; otherwise update sight (unless lying down), smell and hearing, then become directly aware on sight or smell, or let the awareness timer run down (4 per second) and drop direct awareness when it expires
// Grade: DIFFER(263v272) -- block layout: retail keeps the SetAsDirectlyAwareOfPerceivedThing tail call as the fall-through after the LineOfSight || Smelt test and gives the awareness-timer arm its own epilogue (VC7.1 here moves the tail call to the end and shares the incapacitated epilogue); retail also spills the decremented timer (fstp/fld) where this keeps it (fst). Tried: if/else, negated test, return forms, inline DecayAwareness, extern decay constant.
// Leaf: UpdatePerception
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00745730, length 272 bytes, plain `ret` (one `jmp` tail call).
// FableWin.pdb ?UpdatePerception@CTCPerceiveThing@@AAEXXZ (private).
// Callees: IsIncapacitated @ 0x0085F0C0 and IsLyingDown @ 0x0085F2B0 (`__fastcall`, the thing in ecx), CPlayerGui::
// RemovePerceivingThing @ 0x00437900 (g_pPlayerGui 0x013B8790), UpdateLineOfSight @ 0x00745120, UpdateSmell @ 0x00744B50,
// UpdateHearing @ 0x00744FB0 (PDB private members; addresses follow the PDB declaration order Smell, Hearing,
// LineOfSight), SetAsDirectlyAwareOfPerceivedThing @ 0x00744730 (tail-jumped), CTCBase::GetGameSubsystem() @ 0x00686D60
// (only evaluated: the frame rate is a static long, 0x01375550). Constants: 1e-4f 0x0129BA3C, 1.0f 0x0122DED8,
// 4.0f 0x0123088C, 0.0f 0x0122DEDC.
// Shape: the landed ResetPerceptionVariables (0x00744DB0) model and its three inline setters with a constant false (the
// line-of-sight one keeps the folded `mov al,bl ; cmp al,bl` re-read); the three flag clears are plain stores; the
// timer drain is `AwarenessTimer -= 1.0f / rate * 4.0f`, clamped at 0, then the expiry arm clears DirectAwareness and
// the timer and drops the parent from the GUI perceiver drawing.
// Layout: engine/CTCPerceiveThing.h names (AwarenessTimer +0x18, LineOfSight .. Heard +0x2d..+0x32); the header flattens
// the CIntelligentPointer member to bytes, so the class is modelled locally as in the landed relatives.
struct CThing;
struct CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CPlayerGui {                             // the global g_pPlayerGui @ 0x013B8790
    void AddPerceivingThing(CThing* thing);     // 0x004378F0 (forwards to CDrawPerceivers @ +0x1b8; catalog "_Cons_val<...>")
    void RemovePerceivingThing(CThing* thing);  // 0x00437900 (forwards to CDrawPerceivers @ +0x1b8; catalog "_Cons_val<...>")
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
struct CDefPointeeBase {
    unsigned char _pad00[0x40];
    long StartWithoutTarget;                    // +0x40 CPerceivedThingDef word: 0 = start searching for a target
};
struct C3DVector { float X, Y, Z; };
struct CGameSubsystem {
    static long FrameRate;                      // 0x01375550 (frames per second)
};
struct CTCPerceiveThing {
    void*     __vftable;                        // +0x00 (CTCBase)
    CThing*   ParentThing;                      // +0x04
    bool      ToBeRemoved;                      // +0x08
    bool      IsInGlobalUpdateTCs;              // +0x09
    CIntelligentPointer PerceivedThing;         // +0x0c
    float     DistanceFromPerceivedThingSquared;// +0x14
    float     AwarenessTimer;                   // +0x18
    long      FramesSinceDirectlyPerceivedThing;// +0x1c
    C3DVector LastPerceivedPos;                 // +0x20
    bool      Perceiving;                       // +0x2c
    bool      LineOfSight;                      // +0x2d
    bool      ExtendedLineOfSight;              // +0x2e
    bool      IgnoreFOVLineOfSight;             // +0x2f
    bool      DirectAwareness;                  // +0x30
    bool      Smelt;                            // +0x31
    bool      Heard;                            // +0x32
    bool      SearchingForThingToPerceive;      // +0x33
    CDefPointeeBase* PDef;                      // +0x34 CDefPointer<CPerceivedThingDef const>::Object
    long      UpdateOffset;                     // +0x38
    float     SoundRadius;                      // +0x3c
    float     SightRadius;                      // +0x40
    float     ExtendedSightRadius;              // +0x44
    float     SmellRadius;                      // +0x48
    float     GiveUpChaseRadius;                // +0x4c
    bool      PerceptionCombat;                 // +0x50
    long      FrameHitByPerceivedThing;         // +0x54
    void UpdateDrawPerceivers()
    {
        if (PerceivedThing.GetPItem())
            g_pPlayerGui->AddPerceivingThing(ParentThing);
        else
            g_pPlayerGui->RemovePerceivingThing(ParentThing);
    }
    void SetPerceivedThing(const CThing* thing);
    void SetThingLineOfSight(bool lineOfSight)
    {
        if (LineOfSight != lineOfSight) {
            if (LineOfSight)
                g_pPlayerGui->RemovePerceivingThing(ParentThing);
            LineOfSight = lineOfSight;
            if (lineOfSight)
                UpdateDrawPerceivers();
        }
        if (LineOfSight)
            FramesSinceDirectlyPerceivedThing = 0;
    }
    void SetThingExtendedLineOfSight(bool lineOfSight)
    {
        if (ExtendedLineOfSight != lineOfSight) {
            if (ExtendedLineOfSight)
                g_pPlayerGui->RemovePerceivingThing(ParentThing);
            ExtendedLineOfSight = lineOfSight;
            if (lineOfSight)
                UpdateDrawPerceivers();
        }
    }
    void SetThingIgnoreFOVLineOfSight(bool lineOfSight)
    {
        if (IgnoreFOVLineOfSight != lineOfSight) {
            if (IgnoreFOVLineOfSight)
                g_pPlayerGui->RemovePerceivingThing(ParentThing);
            IgnoreFOVLineOfSight = lineOfSight;
            if (lineOfSight)
                UpdateDrawPerceivers();
        }
    }
    void SetAsDirectlyAwareOfPerceivedThing();  // 0x00744730
    void UpdateSmell();                         // 0x00744B50
    void UpdateHearing();                       // 0x00744FB0
    void UpdateLineOfSight();                   // 0x00745120
    CGameSubsystem* GetGameSubsystem() const;   // 0x00686D60 (CTCBase)
    void UpdatePerception();
};

bool __fastcall IsIncapacitated(CThing* thing);   // 0x0085F0C0
bool __fastcall IsLyingDown(CThing* thing);       // 0x0085F2B0
extern float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)
extern float g_AwarenessDecayPerSecond;           // 0x0123088C (4.0f)

void CTCPerceiveThing::UpdatePerception()
{
    if (IsIncapacitated(ParentThing)) {
        SetThingLineOfSight(false);
        SetThingExtendedLineOfSight(false);
        SetThingIgnoreFOVLineOfSight(false);
        Smelt = false;
        Heard = false;
        DirectAwareness = false;
        return;
    }
    if (!IsLyingDown(ParentThing))
        UpdateLineOfSight();
    UpdateSmell();
    UpdateHearing();
    if (LineOfSight || Smelt) {
        SetAsDirectlyAwareOfPerceivedThing();
    } else if (AwarenessTimer > g_epsilon_0129ba3c) {
        AwarenessTimer = AwarenessTimer - 1.0f / GetGameSubsystem()->FrameRate * g_AwarenessDecayPerSecond;
        if (AwarenessTimer < 0.0f)
            AwarenessTimer = 0.0f;
        if (AwarenessTimer < g_epsilon_0129ba3c) {
            DirectAwareness = false;
            AwarenessTimer = 0.0f;
            g_pPlayerGui->RemovePerceivingThing(ParentThing);
        }
    }
}
