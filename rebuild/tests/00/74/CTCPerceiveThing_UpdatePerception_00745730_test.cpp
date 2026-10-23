// Behaviour test for 00745730: src/00745730.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCPerceiveThing::UpdatePerception()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCPerceiveThing::UpdatePerception() -- an incapacitated parent loses all three lines of sight and its smell / hearing / direct awareness; otherwise update sight (unless lying down), smell and hearing, then become directly aware on sight or smell, or let the awareness timer run down (4 per second) and drop direct awareness when it expires
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
// the timer and drops the parent from the GUI perceiver drawing. The perceived test is a bool local (keeps the tail call
// as the fall-through and gives the timer arm its own epilogue); the frame time is an inline `1.0f / FrameRate` getter
// held in a float local (else VC7.1 folds `1.0f / rate * 4.0f` into `4.0f / rate`); the drain goes through an inline
// `float&` accessor, which makes VC7.1 store and reload the timer (`fstp ; fld`) as retail does.
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
    static float GetSecondsPerFrame() { return 1.0f / FrameRate; }
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
    void SetAsDirectlyAwareOfPerceivedThing_00744730();  // 0x00744730
    void UpdateSmell();                         // 0x00744B50
    void UpdateHearing();                       // 0x00744FB0
    void UpdateLineOfSight();                   // 0x00745120
    CGameSubsystem* GetGameSubsystem() const;   // 0x00686D60 (CTCBase)
    void UpdatePerception();
    float& GetAwarenessTimer() { return AwarenessTimer; }
};

bool __fastcall IsIncapacitated(CThing* thing);   // 0x0085F0C0
bool __fastcall IsLyingDown(CThing* thing);       // 0x0085F2B0
extern float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

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
        SetAsDirectlyAwareOfPerceivedThing_00744730();
    } else if (AwarenessTimer > g_epsilon_0129ba3c) {
        float frameTime = GetGameSubsystem()->GetSecondsPerFrame();
        GetAwarenessTimer() -= frameTime * 4.0f;
        if (AwarenessTimer < 0.0f)
            AwarenessTimer = 0.0f;
        if (AwarenessTimer < g_epsilon_0129ba3c) {
            DirectAwareness = false;
            AwarenessTimer = 0.0f;
            g_pPlayerGui->RemovePerceivingThing(ParentThing);
        }
    }
}

// ---- stubs and driver ----
#include <cstdlib>
#include <new>
static int g_removes = 0, g_adds = 0, g_los = 0, g_smell = 0, g_hear = 0, g_aware = 0, g_subsys = 0;
static bool g_incap = false, g_lying = false, g_setLOS = false, g_setSmelt = false;
static CThing* g_lastRemoved = 0;
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = item; }
void CPlayerGui::AddPerceivingThing(CThing*) { ++g_adds; }
void CPlayerGui::RemovePerceivingThing(CThing* thing) { ++g_removes; g_lastRemoved = thing; }
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
long CGameSubsystem::FrameRate = 20;
float g_epsilon_0129ba3c = 1e-4f;
void CTCPerceiveThing::SetAsDirectlyAwareOfPerceivedThing_00744730() { ++g_aware; }
void CTCPerceiveThing::UpdateSmell() { ++g_smell; Smelt = g_setSmelt; }
void CTCPerceiveThing::UpdateHearing() { ++g_hear; }
void CTCPerceiveThing::UpdateLineOfSight() { ++g_los; LineOfSight = g_setLOS; }
CGameSubsystem* CTCPerceiveThing::GetGameSubsystem() const { ++g_subsys; return 0; }
bool __fastcall IsIncapacitated(CThing*) { return g_incap; }
bool __fastcall IsLyingDown(CThing*) { return g_lying; }
static void Reset() { g_removes = g_adds = g_los = g_smell = g_hear = g_aware = g_subsys = 0; }
int main()
{
    CTCPerceiveThing* p = (CTCPerceiveThing*)calloc(1, sizeof(CTCPerceiveThing));
    CThing* parent = (CThing*)0x1234;
    p->ParentThing = parent;
    // incapacitated: the three sight flags drop (one GUI removal each), smell/hearing/awareness cleared
    g_incap = true;
    p->LineOfSight = true; p->ExtendedLineOfSight = true; p->IgnoreFOVLineOfSight = false;
    p->Smelt = true; p->Heard = true; p->DirectAwareness = true; p->FramesSinceDirectlyPerceivedThing = 9;
    p->UpdatePerception();
    CHECK(!p->LineOfSight && !p->ExtendedLineOfSight && !p->IgnoreFOVLineOfSight && !p->Smelt && !p->Heard && !p->DirectAwareness);
    CHECK(g_removes == 2 && g_lastRemoved == parent && g_los == 0 && g_smell == 0 && g_aware == 0);
    // line of sight: updates run, aware
    Reset(); g_incap = false; g_lying = false; g_setLOS = true; g_setSmelt = false;
    p->UpdatePerception();
    CHECK(g_los == 1 && g_smell == 1 && g_hear == 1 && g_aware == 1 && g_subsys == 0);
    // lying down: no sight update; smelt -> aware
    Reset(); g_lying = true; g_setSmelt = true; p->LineOfSight = false;
    p->UpdatePerception();
    CHECK(g_los == 0 && g_smell == 1 && g_hear == 1 && g_aware == 1);
    // nothing perceived, timer above epsilon: drains 4 per second (20 fps -> 0.2 per frame)
    Reset(); g_lying = false; g_setLOS = false; g_setSmelt = false; p->AwarenessTimer = 1.0f; p->DirectAwareness = true;
    p->UpdatePerception();
    CHECK(g_aware == 0 && g_subsys == 1 && p->AwarenessTimer > 0.79f && p->AwarenessTimer < 0.81f && p->DirectAwareness && g_removes == 0);
    // drains below zero: clamped, expiry clears awareness and removes from the GUI
    Reset(); p->AwarenessTimer = 0.1f;
    p->UpdatePerception();
    CHECK(p->AwarenessTimer == 0.0f && !p->DirectAwareness && g_removes == 1 && g_lastRemoved == parent);
    // timer already at zero: nothing happens
    Reset(); p->DirectAwareness = true;
    p->UpdatePerception();
    CHECK(g_subsys == 0 && g_removes == 0 && p->DirectAwareness);
    if (fails == 0) printf("00745730_TEST PASS\n");
    return fails;
}
