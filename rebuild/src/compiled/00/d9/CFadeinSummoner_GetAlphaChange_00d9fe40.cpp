// CFadeinSummoner::GetAlphaChange() -- the per-frame alpha step of the summoner fade: while the camp's chant timer still runs, its remaining time scaled by AlphaGradient plus 0.12, else a flat 0.02; 0.0 once the script is done
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9FE40, length 112 bytes.
// Callees: the script timer service (global 0x0143E8F8) virtual slot 90 (+0x168, GetTimer, twice), IsDone @ 0x00F35B30 (on `this`;
// manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated). Constants: 0x0122DEDC = 0.0f, 0x012C26F8 = 0.02f,
// 0x012C26FC = 0.12f.
// Layout: Ego_r.pdb CFadeinSummoner (40 bytes): ParentClass +0x14 (CQ_MinionCampScript*), MyAlpha +0x1c, AlphaGradient +0x20, AlphaMax +0x24
// (as the landed Init 0x00D9F660). The parent's timer at retail +0x60 is ChantTimer (PDB +0x74: the retail quest-script base is 0x14 smaller).
// Shape: every script query is guarded by IsDone (the script macro returns the default 0.0f); written as `if (time <= 0) {...} else
// {...}` with each arm `if (IsActiveThreadTerminating()) return 0.0f; else return value;` -- the `> 0` test then branches forward to the timer arm
// and the shared `return 0.0f` exit stays in the fall-through arm (an if/return without `else` moves it to the later arm);
// `fild t ; fmul AlphaGradient ; fadd 0.12`.
namespace NScript { class CPredicateBase; }
struct CTimerManager {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088();
    virtual void SetTimer(long index, long time);   // slot 89 (+0x164)
    virtual long GetTimer(long index);        // slot 90 (+0x168)
};
extern CTimerManager* g_pScriptTimerManager;  // 0x0143E8F8

class CTimer { public:
    long TimerIndex;                          // +0x00
    long GetTime() const { return g_pScriptTimerManager->GetTimer(TimerIndex); }
};

struct CQ_MinionCampScript {
    unsigned char _pad[0x60];
    CTimer        ChantTimer;                 // +0x60 (retail)
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CFadeinSummoner : public CEntityScriptBase {
public:
    void*                __vftable;           // +0x00
    void*                Game;                // +0x04
    unsigned char        Me[0x0c];            // +0x08
    CQ_MinionCampScript* ParentClass;         // +0x14
    void*                PMasterData;         // +0x18
    float                MyAlpha;             // +0x1c
    float                AlphaGradient;       // +0x20
    bool                 AlphaMax;            // +0x24
    float GetAlphaChange();
};

float CFadeinSummoner::GetAlphaChange()
{
    if (ParentClass->ChantTimer.GetTime() <= 0) {
        if (IsActiveThreadTerminating())
            return 0.0f;
        else
            return 0.02f;
    } else {
        if (IsActiveThreadTerminating())
            return 0.0f;
        else
            return AlphaGradient * ParentClass->ChantTimer.GetTime() + 0.12f;
    }
}