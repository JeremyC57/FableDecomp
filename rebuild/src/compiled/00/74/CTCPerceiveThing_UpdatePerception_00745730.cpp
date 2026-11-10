// CTCPerceiveThing::UpdatePerception() -- an incapacitated parent loses all three lines of sight and its smell / hearing / direct awareness; otherwise update sight (unless lying down), smell and hearing, then become directly aware on sight or smell, or let the awareness timer run down (4 per second) and drop direct awareness when it expires
// Leaf: UpdatePerception
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00745730, length 272 bytes, plain `ret` (one `jmp` tail call).
// FableWin.pdb ?UpdatePerception@CTCPerceiveThing@@AAEXXZ (private).
// Callees: IsIncapacitated @ 0x0085F0C0 and IsLyingDown @ 0x0085F2B0 (`__fastcall`, the thing in ecx), CPlayerGui::
// RemovePerceivingThing @ 0x00437900 ((&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790), UpdateLineOfSight @ 0x00745120, UpdateSmell @ 0x00744B50,
// UpdateHearing @ 0x00744FB0 (PDB private members; addresses follow the PDB declaration order Smell, Hearing,
// LineOfSight), SetAsDirectlyAwareOfPerceivedThing @ 0x00744730 (tail-jumped), CTCBase::GetGameSubsystem() @ 0x00686D60
// (only evaluated: the frame rate is a static long, 0x01375550). Constants: 1e-4f 0x0129BA3C, 1.0f 0x0122DED8,
// 4.0f 0x0123088C, 0.0f 0x0122DEDC.
// Shape: the landed ResetPerceptionVariables (0x00744DB0) model and its three inline setters with a constant false (the
// line-of-sight one keeps the folded `mov al,bl ; cmp al,bl` re-read); the three flag clears are plain stores; the
// timer drain is `AwarenessTimer -= 1.0f / rate * 4.0f`, clamped at 0, then the expiry arm clears DirectAwareness and
// the timer and drops the parent from the GUI perceiver drawing. The perceived test is a bool local (keeps the tail call
// as the fall-through and gives the timer arm its own epilogue); the frame time is an inline `1.0f / FrameRate` getter
// held in a float local (else VC7.1 folds `1.0f / rate * 4.0f` into `4.0f / rate`); the drain goes through an inline
// `float&` accessor, which makes VC7.1 store and reload the timer (`fstp ; fld`) as retail does.
// Layout: engine/CTCPerceiveThing.h names (AwarenessTimer +0x18, LineOfSight .. Heard +0x2d..+0x32); the header flattens
// the CIntelligentPointer member to bytes, so the class is modelled locally as in the landed relatives.
// Name fix (agentNF6 2026-11-10): CTCPerceiveThing::SetLastAwareInfo 0x00744730 (?SetLastAwareInfo@CTCPerceiveThing@@AAEXXZ) and UpdateSmell 0x00744B50 (?UpdateSmell@CTCPerceiveThing@@AAEXXZ) are private (Ego_r.pdb / FableWin.pdb); engine/CTCPerceiveThing.h.
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:                             // the global (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) @ 0x013B8790
    void AddPerceivingThing(CThing* thing);     // 0x004378F0 (forwards to CDrawPerceivers @ +0x1b8; catalog "_Cons_val<...>")
    void RemovePerceivingThing(CThing* thing);  // 0x00437900 (forwards to CDrawPerceivers @ +0x1b8; catalog "_Cons_val<...>")
};
class CDefPointeeBase { public:
    unsigned char _pad00[0x40];
    long StartWithoutTarget;                    // +0x40 CPerceivedThingDef word: 0 = start searching for a target
};
#include "engine/C3DVector.h"
struct CGameSubsystem {
    static long FrameRate;                      // 0x01375550 (frames per second)
    static float GetSecondsPerFrame() { return 1.0f / FrameRate; }
};
class CTCPerceiveThing { public:
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
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().AddPerceivingThing(ParentThing);
        else
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(ParentThing);
    }
    void SetPerceivedThing(const CThing* thing);
    void SetThingLineOfSight(bool lineOfSight)
    {
        if (LineOfSight != lineOfSight) {
            if (LineOfSight)
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(ParentThing);
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
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(ParentThing);
            ExtendedLineOfSight = lineOfSight;
            if (lineOfSight)
                UpdateDrawPerceivers();
        }
    }
    void SetThingIgnoreFOVLineOfSight(bool lineOfSight)
    {
        if (IgnoreFOVLineOfSight != lineOfSight) {
            if (IgnoreFOVLineOfSight)
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(ParentThing);
            IgnoreFOVLineOfSight = lineOfSight;
            if (lineOfSight)
                UpdateDrawPerceivers();
        }
    }
private:
    void SetLastAwareInfo();                    // 0x00744730
    void UpdateSmell();                         // 0x00744B50
public:
    void UpdateHearing();                       // 0x00744FB0
    void UpdateLineOfSight();                   // 0x00745120
    CGameSubsystem* GetGameSubsystem() const;   // 0x00686D60 (CTCBase)
    void UpdatePerception();
    float& GetAwarenessTimer() { return AwarenessTimer; }
};

bool __fastcall IsIncapacitated(CThing* thing);   // 0x0085F0C0
bool __fastcall IsLyingDown(CThing* thing);       // 0x0085F2B0
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C (1e-4f)

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
    bool perceived = LineOfSight || Smelt;
    if (perceived) {
        SetLastAwareInfo();
    } else if (AwarenessTimer > DELTA_NOTIONAL_ZERO) {
        float frameTime = GetGameSubsystem()->GetSecondsPerFrame();
        GetAwarenessTimer() -= frameTime * 4.0f;
        if (AwarenessTimer < 0.0f)
            AwarenessTimer = 0.0f;
        if (AwarenessTimer < DELTA_NOTIONAL_ZERO) {
            DirectAwareness = false;
            AwarenessTimer = 0.0f;
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(ParentThing);
        }
    }
}