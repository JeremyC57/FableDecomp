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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer {
public:
    long TimerIndex;                                    // +0x00
    CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer();
};
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
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