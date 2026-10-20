// NScript::CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary(const CScriptThing& thing) -- if the hero has set the thing alight, disables the hero's will for a three-second script timer (frame by frame) and re-enables it unless the thread is terminating
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB0700, 195 bytes (padded to 208), `ret 4`.
//   PDB name ?WaitForEnflameReactionIfNecessary@CV_MurderTwistFinalScript@NScript@@QAEXABVCScriptThing@@@Z.
// Callees: HasHeroEnflamedEntity(const CScriptThing&) 0x00EB07D0; CScriptBase::IsActiveThreadTerminating 0x00CB7940;
//   through the quest's interface (+0x40): slot 131 SetHeroWillAsUsable(bool) (+0x20c), slot 7 NewScriptFrame (+0x1c);
//   through the global interface 0x0143E8F8 (the CTimer local): slot 87 RegisterTimer (+0x15c), slot 89 SetTimer
//   (+0x164), slot 90 GetTimer (+0x168), slot 88 DeregisterTimer (+0x160).
// Shape: a CTimer local (index in esi) whose inline destructor deregisters it on both exits (the terminating exit
//   inside the loop gets its own copy); the `while (GetTime() > 0)` test is rotated by VC (test before the loop and
//   at the bottom); the post-loop `if (IsActiveThreadTerminating()) return;` before the re-enable (not an `if (!..)` guard) is
//   what keeps the in-loop exit as its own deregister tail.
struct CScriptThing;
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void SetTimer(long timer, long seconds);   // slot 89 (+0x164)
    virtual long GetTimer(long timer);   // slot 90 (+0x168)
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void SetHeroWillAsUsable(bool usable);   // slot 131 (+0x20c)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CV_MurderTwistFinalScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool HasHeroEnflamedEntity(const CScriptThing& thing);   // 0x00EB07D0
    void WaitForEnflameReactionIfNecessary(const CScriptThing& thing);
};
void CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary(const CScriptThing& thing)
{
    if (!HasHeroEnflamedEntity(thing))
        return;
    if (IsActiveThreadTerminating())
        return;
    Game->SetHeroWillAsUsable(false);
    CTimer timer;
    timer.SetTime(3);
    while (timer.GetTime() > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetHeroWillAsUsable(true);
}
}