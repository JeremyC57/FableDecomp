// CQ_MinionCampScript::StartMinionSpawnTimer() -- set the camp's minion spawn timer to a random wait between the min and max spawn periods (whole seconds), so the next summoning comes at an unpredictable moment
// Leaf: StartMinionSpawnTimer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9F200, length 69 bytes (second piece of catalogue row 00d9f1b0,
// split via the skipped-row ledger, chunk 908). Called only as the tail jump of OnMinionKilled @ 0x00D9F1B0.
// Callees: rand @ 0x00BFEB16, __ftol2 @ 0x00BFEA70 (the `(long)` cast), the game-script interface global
// @ 0x0143E8F8 slot 89 SetTimer(long timer, long time).
// Shape: rand() is called first (its result is stored to the one frame word and `fild`ed), the interface and its
// vtable are loaded into edi/ebx across the x87 work, `(max - min) * r + min` is `fld max ; fsub min ; fmul st(1) ;
// fadd min`, the constant 0x0129C81C is 1.0f / RAND_MAX (3.0518509e-05) folded from the source, and the random
// factor stays on the FPU stack until the `fstp st(0)` after the cast.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped). Same bytes.
#include <stdlib.h>
class CGameScriptInterface { public:                     // vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv names)
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
    virtual void SetTimer(long timer, long time) const;                     // slot 89 (+0x164) ?SetTimer@CGameScriptInterface@@UAEXJJ@Z
    virtual long GetTimer(long timer) const;                                // slot 90 (+0x168) ?GetTimer@CGameScriptInterface@@UBEJJ@Z: time left on the timer
};
class CTimer { public:
    long Handle;                                   // +0x00
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CScriptBase {                                // the quest script base: vptr first; the thread-state query is a
public:                                            // non-virtual member @ 0x00CB7940 (`this` in ecx)
    void* __vftable;                               // +0x00
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {   // Ego_r.pdb CQ_MinionCampScript (208 bytes): the retail fields sit
public:                                            // 0x14 below the PDB offsets (NumberOfMinions is PDB +0x64, retail +0x50)
    unsigned char _pad04[0x4c];                    // +0x04 .. +0x4f: Game, Me, quest state flags
    long          NumberOfMinions;                 // +0x50 minions alive in the camp right now
    long          MaxNumberOfMinions;              // +0x54 how many the camp keeps summoning up to
    float         MinMinionSpawnPeriod;            // +0x58 shortest wait before the next minion rises
    float         MaxMinionSpawnPeriod;            // +0x5c longest wait
    unsigned char _pad60[0x10];                    // +0x60 .. +0x6f: ChantTimer, ChantTimerBar, ChantTimerPaused, PausedTime
    CTimer        MinionSpawnTimer;                // +0x70 the game-script timer that paces summoning
    void OnMinionKilled();                         // 0x00D9F1B0
    void StartMinionSpawnTimer();                  // 0x00D9F200
};
} // namespace NScript
using NScript::CQ_MinionCampScript;

void NScript::CQ_MinionCampScript::StartMinionSpawnTimer()
{
    float r = rand() * (1.0f / RAND_MAX);                 // 0..1, kept on the FPU stack until the call is made
    g_pGameScriptInterface->SetTimer(MinionSpawnTimer.Handle,
        (long)((MaxMinionSpawnPeriod - MinMinionSpawnPeriod) * r + MinMinionSpawnPeriod));
}