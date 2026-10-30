// NScript::CQR_EscortTrader_ManagerScript::OnPersist(CPersistContext& context) -- persist the Orchard Farm escort counter (default 0), its phase (default 0) and Activated (default false)
// Leaf: ?OnPersist@CQR_EscortTrader_ManagerScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25750, 99 bytes, thiscall, `ret 4` (virtual, level-script vtable slot 4).
// Identity (proven): slot 4 (OnPersist) of the RTTI-less level-script vtable 0x012E1D48, which the landed NScript::CQR_EscortTrader_ManagerScript::CQR_EscortTrader_ManagerScript 0x00E24DD0 (and the landed Alloc 0x00E25970) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?OnPersist@CQR_EscortTrader_ManagerScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Layout: Ego_r.pdb CQR_EscortTrader_ManagerScript members at Ego_r offset - 0x14: OrchardFarmTraderEscortCounter +0x48,
//   DebugCounter +0x4c, Activated +0x50, OrchardFarmTraderEscortPhase +0x54.
// Callees: CPersistContext::Transfer<long> 0x00410BE0, Transfer<EscortTraderManagerPhase> 0x00E257C0 (this TU's instance;
//   Ego_r.pdb ??$Transfer@W4EscortTraderManagerPhase@CQR_EscortTrader_ManagerScript@NScript@@@CPersistContext@@QAEXPBDAAW4123@ABW4123@@Z,
//   the catalog's Transfer<EOrchardFarmTraderEscortPhase> is an invented enum name), Transfer<bool> 0x004045C0;
//   keys 0x012C2D20, 0x012E1D84, 0x012E1D78.
// Shape: the defaults live in the pushed-ecx slot and the dead `context` argument slot.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTrader_ManagerScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum EscortTraderManagerPhase { };               // Ego_r.pdb nested enum (value 0 unnamed here)
    long                     OrchardFarmTraderEscortCounter;   // +0x48
    long                     DebugCounter;                     // +0x4c
    bool                     Activated;                        // +0x50
    EscortTraderManagerPhase OrchardFarmTraderEscortPhase;     // +0x54
    virtual void OnPersist(CPersistContext& context);
};

void CQR_EscortTrader_ManagerScript::OnPersist(CPersistContext& context)
{
    context.Transfer("OrchardFarmTraderEscortCounter", OrchardFarmTraderEscortCounter, 0L);
    context.Transfer("OrchardFarmTraderEscortPhase", OrchardFarmTraderEscortPhase, (EscortTraderManagerPhase)0);
    context.Transfer("Activated", Activated, false);
}
}