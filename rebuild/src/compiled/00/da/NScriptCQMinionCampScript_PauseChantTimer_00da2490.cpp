// CQ_MinionCampScript::PauseChantTimer() -- record the chant timer's time into PausedTime and raise ChantTimerPaused, then wait (one script frame at a time) while Briar Rose is in state 1; if the script thread is not being terminated afterwards, clear ChantTimerPaused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA2490, length 92 bytes.
// Callees: the thing manager global @ 0x0143E8F8 vtable slot 90 (+0x168, one long argument: the value of the
// CTimer index), CGameScriptInterface::NewScriptFrame (slot 7, +0x1c) through the interface at +0x40,
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (twice).
// Layout (retail; PDB CQ_MinionCampScript is 0xd0 bytes with its first member at +0x5c): the quest-script base
// holds the game interface at +0x40 (PDB CLevelScriptNoMasterBase::Game +0x54) and the quest members start at
// +0x48, 0x14 lower than the PDB: ChantTimer +0x60, ChantTimerBar +0x64, ChantTimerPaused +0x68, PausedTime +0x6c;
// the two spawn-point lists are 4 bytes in the shipped build (PDB 12), so BriarState sits at +0xa8 (PDB +0xcc).
// Shape: `mov ebx,1` serves both the `BriarState == 1` compare and the `ChantTimerPaused = true` byte store; the
// wait is the script WAIT macro (`while (cond) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`),
// rotated by the compiler with an aligned loop head, followed by the macro's trailing terminate test. The final
// `ChantTimerPaused = false` reuses al (known zero after the test).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CThingManager_Calls {             // retail vtable of the global @ 0x0143E8F8
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual long GetTimerValue(long timer);                         // slot 90 (+0x168)
};
extern CThingManager_Calls* g_pThingManager_0143e8f8;   // 0x0143E8F8

struct CGameScriptInterface_Calls {      // retail vtable @ 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();                                    // slot 7 (+0x1c) @ 0x006E7410
};

namespace NScript {
class CQ_MinionCampScript {
public:
    unsigned char               _pad_0x00[0x40];
    CGameScriptInterface_Calls* Game;                   // +0x40
    unsigned char               _pad_0x44[0x1c];
    long                        ChantTimer;             // +0x60 CTimer (timer index)
    long                        ChantTimerBar;          // +0x64
    bool                        ChantTimerPaused;       // +0x68
    long                        PausedTime;             // +0x6c
    unsigned char               _pad_0x70[0x38];
    long                        BriarState;             // +0xa8 BriarRoseState
    bool IsActiveThreadTerminating();                   // 0x00CB7940 (CScriptBase)
    void PauseChantTimer();
};
} // namespace NScript
using NScript::CQ_MinionCampScript;

void NScript::CQ_MinionCampScript::PauseChantTimer()
{
    PausedTime = g_pThingManager_0143e8f8->GetTimerValue(ChantTimer);
    ChantTimerPaused = true;
    while (BriarState == 1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ChantTimerPaused = false;
}