// NScript::CQ_HeroSoulsNostroScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script factory: new CQ_HeroSoulsNostroScript(data, game) with the constructor inlined (CScriptBase, Game / MasterScriptData, the own vtable, the NostroVulnerableTimer timer)
// Leaf: ?Alloc@CQ_HeroSoulsNostroScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D89CF0, 71 bytes (+ int3 padding to 80), static `__fastcall` (data in ecx, game in edx), plain `ret`.
// Identity (proven): NScript::CQ_HeroSoulsNostroScript::Alloc; evidence: the inlined constructor stores the RTTI-less vtable 0x012D4D2C, whose Main (slot 2, 0x00D87CB0) registers the entity scripts SummonerToKill, GuardNearGraveyardPath, UndeadSoldier, Nostro ..., all nested in Ego_r.pdb NScript::CQ_HeroSoulsNostroScript (?Alloc@C<name>@CQ_HeroSoulsNostroScript@NScript@@); its OnPersist (slot 4, 0x00D87C60) transfers SummonerCount / PlayedNostroCS, members of only CQ_HeroSoulsNostroScript; the timer lands at +0x54 = Ego_r CQ_HeroSoulsNostroScript::NostroVulnerableTimer (CTimer, +0x68) - 0x14 and `new` asks 0x58 = sizeof Ego_r CQ_HeroSoulsNostroScript (0x6C) - 0x14; this factory is stored at 0x00CD6AC3 (CScriptInfoManager registration 0x00CD52D0); the Ego_r/FableWin.pdb name sets hold ?Alloc@CQ_HeroSoulsNostroScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "Alloc" is generic (family J034: six one-timer quest-script factories).
// Callees: operator new 0x00BFEA1A (0x58 bytes), CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), the script
//   interface's RegisterTimer (slot 87, +0x15C; ?RegisterTimer@CGameScriptInterface@@UAEJXZ) through the global interface pointer
//   0x0143E8F8 (no PDB data name; the catalog spelling ?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A, reused).
// Layout: Ego_r.pdb CQ_HeroSoulsNostroScript (0x6C; retail 0x58 = Ego_r - 0x14, the script-base difference): CLevelScriptBase<T>
//   {CGameScriptInterfaceBase* Game, T* MasterScriptData} at retail +0x40 / +0x44, NostroVulnerableTimer (CTimer) Ego_r +0x68 -> retail +0x54.
// Model: agent952 H037 (the out-of-line constructor of the same classes); local classes -- Review at landing.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CGameScriptInterface {                            // vtable 0x01260F0C; vNNN are slots not used here
public:
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
class CQ_HeroSoulsNostroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HeroSoulsNostroScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_HeroSoulsNostroScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x54 - 0x48];          // POD members up to the timer
    CTimer        NostroVulnerableTimer;                            // +0x54
};
CScriptBase* __fastcall CQ_HeroSoulsNostroScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_HeroSoulsNostroScript(data, game);
}
}