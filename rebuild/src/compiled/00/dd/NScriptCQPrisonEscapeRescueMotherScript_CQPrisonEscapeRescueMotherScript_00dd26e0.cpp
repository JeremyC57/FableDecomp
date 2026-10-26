// NScript::CQ_PrisonEscapeRescueMotherScript::CQ_PrisonEscapeRescueMotherScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: CScriptBase, the level-script Game / MasterScriptData pointers, the own vtable, then the RunningTimer timer registered with the script interface
// Leaf: ??0CQ_PrisonEscapeRescueMotherScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD26E0, 49 bytes (64 with int3 padding), thiscall `ret 8`.
// Identity (proven): CQ_PrisonEscapeRescueMotherScript::CQ_PrisonEscapeRescueMotherScript; evidence: the body stores the RTTI-less vtable 0x012DB86C, whose Main (slot 2, 0x00DD2CE0)
//   registers the entity scripts PrisonGuard, PrisonerAccomplice, Prisoner, PrisonEscapeGuard ..., all nested in Ego_r.pdb NScript::CQ_PrisonEscapeRescueMotherScript (?Alloc@C<name>@CQ_PrisonEscapeRescueMotherScript@NScript@@) and its OnPersist (slot 4, 0x00DD2C80) transfers CurrentTask / HeroHasFoundWeapons / MotherRescuedFromPrisonAutoSaveCheckPoint, members of only CQ_PrisonEscapeRescueMotherScript;
//   FableWin.pdb has ??0CQ_PrisonEscapeRescueMotherScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. The registered timer lands at
//   +0x7C, Ego_r.pdb CQ_PrisonEscapeRescueMotherScript::RunningTimer (CTimer, Ego_r +0x90; quest-script members sit 0x14 lower in retail).
// Label: the manifest's "CV_AmbushScamScript" is propagated across family H037 (seven quest-script constructors).
// Callees: CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), the script interface's RegisterTimer (slot 87, +0x15C)
//   through the global interface pointer 0x0143E8F8 (catalog ?g_pGameScriptInterface@@3PAUCGameScriptInterface@@A, reused).
// Model: Ego_r.pdb CLevelScriptBase<T> {CGameScriptInterfaceBase* Game, T* MasterScriptData} (retail +0x40 / +0x44), CTimer
//   {long TimerIndex} with an inline constructor; local classes -- Review at landing.
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CGameScriptInterface { public:                           // vtable 0x01260F0C; vNNN are slots not used here
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
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                       // slot 87 (+0x15C)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

class CTimer {
public:
    long TimerIndex;                                    // +0x00
    CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer();
};

class CScriptBase {
public:
    CScriptBase();                                      // 0x00CB8110
    virtual ~CScriptBase();
    unsigned char _base[0x40 - 4];                      // Threads, EntityScripts, EntityScriptBindings, LastWorldUpdateFrame,
                                                        // PActiveThread, RuntimeScriptedEntityPersist
};

template <class TMasterData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TMasterData*              MasterScriptData;         // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : Game(&game), MasterScriptData((TMasterData*)data) { }
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonEscapeRescueMotherScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_PrisonEscapeRescueMotherScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_PrisonEscapeRescueMotherScript();
    unsigned char _members[0x7C - 0x48];           // POD members up to the timer
    CTimer        RunningTimer;                            // +0x7C
};

CQ_PrisonEscapeRescueMotherScript::CQ_PrisonEscapeRescueMotherScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}