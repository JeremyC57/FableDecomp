// Behaviour test for CCreatureAction_DragonFlyInto::Initialise (0x008F4B00): FLY_INTO, base Initialise, running time from
// GetStartRunningTimeSeconds; a single anim set starts at clamp((run - delay/fps) / (total/fps - delay/fps)); with a mode
// manager, PreviousPoint falls back to the first movement point unless UsePreviousPoint, then a CModeDragonFlying is added.
#include <cstdio>
#include <cstring>
#include <cmath>

struct C3DVector { float X, Y, Z; };
struct CThing;
static char g_anim[32]; static int g_baseInit = 0, g_modesAdded = 0;
static C3DVector g_modePos; static float g_modeSpeed = 0; static CThing* g_modeCreature = 0;
struct CCharString { const char* s; CCharString(const char* t) : s(t) {} };
struct CAnimSet { float StartFraction, CurrentFraction; };
static CAnimSet* g_set = 0; static long g_bank = -1;
struct CTCCreatureModeManager;
struct CThing { CTCCreatureModeManager* Modes; bool GetInterface(CTCCreatureModeManager** out, long) { if (!Modes) return false; *out = Modes; return true; } };
CAnimSet* GetSingleActionAnimSet(CThing*, long bank) { g_bank = bank; return g_set; }
struct CAnimationEntry { long BankIndex; };
static long g_framesPerSecond = 10;
struct C3DVectorVector { C3DVector Data[4]; C3DVector& operator[](long i) { return Data[i]; } };
struct CCountedActionPointer { int Id; };
struct CModeDragonFlying {
    CModeDragonFlying(CThing* c, const C3DVectorVector&, const C3DVector& pos, float speed, CCountedActionPointer)
    { g_modeCreature = c; g_modePos = pos; g_modeSpeed = speed; }
};
struct CCountedModePointer { CModeDragonFlying* m_pObject; CCountedModePointer(CModeDragonFlying* o) : m_pObject(o) {} };
struct CTCCreatureModeManager { bool AddMode(CCountedModePointer m) { ++g_modesAdded; delete m.m_pObject; return true; } };
struct CPtr { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCombatActionBase {
    CPtr PCreature; long TotalTime; float RunningTimeSeconds; CAnimationEntry* PAnimEntry; long Delay; float StartRun;
    long GetAnimDelayTime() { return Delay; }
    float GetStartRunningTimeSeconds() { return StartRun; }
    void SetAnimationName(const CCharString& n) { std::strcpy(g_anim, n.s); }
    void Initialise() { ++g_baseInit; }
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_DragonFlyInto : CCombatActionBase {
    C3DVector Dest, StartPos, PreviousPoint; C3DVectorVector MovementPoints; float Speed; bool UsePreviousPoint; CCountedActionPointer PEndAction;
    void Initialise();
};

void CCreatureAction_DragonFlyInto::Initialise()
{
    {
        CCharString name("FLY_INTO");
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    RunningTimeSeconds = GetStartRunningTimeSeconds();
    CAnimSet* set = GetSingleActionAnimSet(PCreature.GetPItem(), PAnimEntry->BankIndex);
    float fps = (float)g_framesPerSecond;
    if (set) {
        float delay = GetAnimDelayTime() / fps;
        float fraction = (RunningTimeSeconds - delay) / (TotalTime / fps - delay);
        if (fraction < 0.0f)
            fraction = 0.0f;
        else if (fraction > 1.0f)
            fraction = 1.0f;
        set->StartFraction = fraction;
        set->CurrentFraction = fraction;
    }
    CTCCreatureModeManager* modes;
    if (PCreature.GetPItem()->GetInterface(&modes, 0x31)) {
        if (!UsePreviousPoint)
            PreviousPoint = MovementPoints[0];
        modes->AddMode(CCountedModePointer(new CModeDragonFlying(GetCreature(), MovementPoints, PreviousPoint, Speed, PEndAction)));
    }
}

static int near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int ok = 1;
    CTCCreatureModeManager modes; CThing dragon; dragon.Modes = &modes;
    CAnimationEntry entry; entry.BankIndex = 5; CAnimSet set; set.StartFraction = set.CurrentFraction = -9;
    CCreatureAction_DragonFlyInto a; a.PCreature.m_pItem = &dragon; a.PAnimEntry = &entry;
    a.TotalTime = 30; a.Delay = 10; a.StartRun = 1.5f; a.Speed = 4.0f; a.UsePreviousPoint = false;
    a.PreviousPoint.X = 99; a.MovementPoints.Data[0].X = 1; a.MovementPoints.Data[0].Y = 2; a.MovementPoints.Data[0].Z = 3;
    g_set = &set;
    a.Initialise();   // delay 1.0, total 3.0 -> (1.5-1)/(3-1) = 0.25
    if (std::strcmp(g_anim, "FLY_INTO") != 0 || g_baseInit != 1 || g_bank != 5 || !near(a.RunningTimeSeconds, 1.5f)) ok = 0;
    if (!near(set.StartFraction, 0.25f) || !near(set.CurrentFraction, 0.25f)) ok = 0;
    if (g_modesAdded != 1 || g_modeCreature != &dragon || !near(g_modePos.X, 1) || !near(a.PreviousPoint.X, 1) || !near(g_modeSpeed, 4.0f)) ok = 0;
    a.StartRun = 0.5f; a.Initialise();   // below the delay -> 0
    if (!near(set.StartFraction, 0.0f)) ok = 0;
    a.StartRun = 9.0f; a.Initialise();   // past the end -> 1
    if (!near(set.CurrentFraction, 1.0f)) ok = 0;
    // UsePreviousPoint keeps PreviousPoint; no anim set leaves the set alone
    a.UsePreviousPoint = true; a.PreviousPoint.X = 77; g_set = 0; set.StartFraction = -9;
    a.Initialise();
    if (!near(g_modePos.X, 77) || !near(set.StartFraction, -9) || g_modesAdded != 4) ok = 0;
    dragon.Modes = 0; a.Initialise();
    if (g_modesAdded != 4) ok = 0;
    std::printf("008f4b00_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
