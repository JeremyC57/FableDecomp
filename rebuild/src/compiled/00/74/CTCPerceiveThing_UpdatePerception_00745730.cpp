// CTCPerceiveThing::UpdatePerception() -- an incapacitated parent loses all three lines of sight and its smell / hearing / direct awareness; otherwise update sight (unless lying down), smell and hearing, then become directly aware on sight or smell, or let the awareness timer run down (4 per second) and drop direct awareness when it expires
// Leaf: ?UpdatePerception@CTCPerceiveThing@@AAEXXZ
// NF64 (round 7, q10655): access-only fix: UpdatePerception (this row) and its callee UpdateHearing are private in Ego_r.pdb /
//   FableWin.pdb and engine/CTCPerceiveThing.h; the row's pre-existing local model now declares both under `private:`.
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
// LineOfSight), SetAsDirectlyAwareOfPerceivedThing @ 0x00744730 (tail-jumped), CTCBase::&PeekComponent() @ 0x00686D60
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
// NF51 (round 7): the player GUI and its singleton base come from the shared engine/CPlayerGui.h / engine/CTBaseSingletonNoDefaultAlloc.h
// (were local CTBaseSingletonNoDefaultAlloc / CPlayerGui models with CThing* parameters): the callees 0x004378F0 / 0x00437900 are
// Ego_r.pdb ?AddPerceivingThing@CPlayerGui@@QAEXAAVCThing@@@Z / ?RemovePerceivingThing@CPlayerGui@@QAEXAAVCThing@@@Z (header opt-in
// FABLE_RETAIL_CPLAYERGUI_PERCEIVERS), taking the parent thing by reference.
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PERCEIVERS
#include "engine/CPlayerGui.h"
class CDefPointeeBase { public:
    unsigned char _pad00[0x40];
    long StartWithoutTarget;                    // +0x40 CPerceivedThingDef word: 0 = start searching for a target
};
#include "engine/C3DVector.h"
class CMainGameComponent { public:
protected:
    static long ConstantFPS;                    // 0x01375550 (frames per second) ?ConstantFPS@CMainGameComponent@@1JA (agentNF16: was FrameRate)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r + FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
    static float GetSecondsPerFrame() { return 1.0f / GetConstantFPS(); }
};
class CTCBase { public:                                 // row-local view of the component base (Ego_r.pdb CTCBase: vptr, ParentThing)
    void*     __vftable;                        // +0x00 (CTCBase)
    CThing*   ParentThing;                      // +0x04
protected:
    const CMainGameComponent& PeekComponent() const;   // 0x00686D60 Ego_r.pdb ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ
};
class CTCPerceiveThing : public CTCBase { public:
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
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().AddPerceivingThing(*ParentThing);
        else
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(*ParentThing);
    }
    void SetPerceivedThing(const CThing* thing);
    void SetThingLineOfSight(bool lineOfSight)
    {
        if (LineOfSight != lineOfSight) {
            if (LineOfSight)
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(*ParentThing);
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
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(*ParentThing);
            ExtendedLineOfSight = lineOfSight;
            if (lineOfSight)
                UpdateDrawPerceivers();
        }
    }
    void SetThingIgnoreFOVLineOfSight(bool lineOfSight)
    {
        if (IgnoreFOVLineOfSight != lineOfSight) {
            if (IgnoreFOVLineOfSight)
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(*ParentThing);
            IgnoreFOVLineOfSight = lineOfSight;
            if (lineOfSight)
                UpdateDrawPerceivers();
        }
    }
private:
    void SetLastAwareInfo();                    // 0x00744730
    void UpdateSmell();                         // 0x00744B50
    void UpdateLineOfSight();                   // 0x00745120 ?UpdateLineOfSight@CTCPerceiveThing@@AAEXXZ (Ego_r.pdb / FableWin.pdb private;
                                                //   engine/CTCPerceiveThing.h; NF59 q10523: access-only callee fix, was public)
    void UpdateHearing();                       // 0x00744FB0 ?UpdateHearing@CTCPerceiveThing@@AAEXXZ (Ego_r.pdb private; NF64 q10655)
    void UpdatePerception();                    // ?UpdatePerception@CTCPerceiveThing@@AAEXXZ (Ego_r.pdb 0x00758320 / FableWin.pdb private; NF64 q10655)
public:
    float& GetAwarenessTimer() { return AwarenessTimer; }
};

// Naming (NF439 gen, 2026-11-25): callee 0x0085F0C0 binds its PDB spelling ?IsIncapacitated@NCreatureMode@@YI_NABVCThing@@@Z (T1 target; was the free stand-in ?IsIncapacitated@@YI_NPAVCThing@@@Z).
namespace NCreatureMode { bool __fastcall IsIncapacitated(CThing const& thing); }
// Naming (NF439 gen, 2026-11-25): callee 0x0085F2B0 binds its PDB spelling ?IsLyingDown@NCreatureMode@@YI_NABVCThing@@@Z (T1 target; was the free stand-in ?IsLyingDown@@YI_NPAVCThing@@@Z).
namespace NCreatureMode { bool __fastcall IsLyingDown(CThing const& thing); }
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C (1e-4f)

void CTCPerceiveThing::UpdatePerception()
{
    if (NCreatureMode::IsIncapacitated(*ParentThing)) {
        SetThingLineOfSight(false);
        SetThingExtendedLineOfSight(false);
        SetThingIgnoreFOVLineOfSight(false);
        Smelt = false;
        Heard = false;
        DirectAwareness = false;
        return;
    }
    if (!NCreatureMode::IsLyingDown(*ParentThing))
        UpdateLineOfSight();
    UpdateSmell();
    UpdateHearing();
    bool perceived = LineOfSight || Smelt;
    if (perceived) {
        SetLastAwareInfo();
    } else if (AwarenessTimer > DELTA_NOTIONAL_ZERO) {
        float frameTime = PeekComponent().GetSecondsPerFrame();
        GetAwarenessTimer() -= frameTime * 4.0f;
        if (AwarenessTimer < 0.0f)
            AwarenessTimer = 0.0f;
        if (AwarenessTimer < DELTA_NOTIONAL_ZERO) {
            DirectAwareness = false;
            AwarenessTimer = 0.0f;
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(*ParentThing);
        }
    }
}