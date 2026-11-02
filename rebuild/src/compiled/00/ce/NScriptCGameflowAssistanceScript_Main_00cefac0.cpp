// NScript::CGameflowAssistanceScript::Main() -- the gameflow assistance script's main thread: spawn the four tutorial watcher threads Tutorial_CombatMultiplier, Tutorial_LowHealth, Tutorial_LowMagic and Tutorial_RenownLevelIncrease (each a CSpawnedFunc<CGameflowAssistanceScript> with an empty region name)
// Leaf: ?Main@CGameflowAssistanceScript@NScript@@UAEXXZ
// Naming wave 29 (family sfbh): engine/CSpawnedFuncBase.h now carries retail's implicit destructor and the real CCharString /
//   CCountedPointer<const NScript::CPredicateBase> members; header contract met here: engine/CCountedPointer.h included before it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CEFAC0, 499 bytes, thiscall, plain `ret`. Slot 2 (Main) of the RTTI-less script vtable
//   0x012C5DE0 (word 0x012C5DE8), whose slot 1 is the landed RegisterMain 0x00CEFA20 (it spawns this Main as the "Main" thread).
// Identity (proven, two sources): (1) the vtable slot (CScriptBase slots: 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist;
//   the landed RegisterMain row proves the vtable is CGameflowAssistanceScript's); (2) the four thread names are the landed
//   members Tutorial_CombatMultiplier 0x00CEFCC0, Tutorial_LowHealth 0x00CEFFB0, Tutorial_LowMagic 0x00CF0180 and
//   Tutorial_RenownLevelIncrease 0x00CF02A0 (Ego_r.pdb ?Tutorial_*@CGameflowAssistanceScript@NScript@@QAEXXZ), and Ego_r.pdb holds
//   ?Main@CGameflowAssistanceScript@NScript@@UAEXXZ right after RegisterMain. Catalog row "_gapscan::sub_00cefac0" carried no name.
// Callees: scalar operator new 0x00BFEA1A (0x3c bytes), CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450, CScriptBase::AddSpawnedFunction 0x00CB7E50.
// Data: the CSpawnedFunc<CGameflowAssistanceScript> vtable 0x012C5DF4; the thread names 0x012C5E58 / 0x012C5E44 / 0x012C5E30 /
//   0x012C5E10; the empty region name 0x0122D70E.
// Shape: the landed RegisterMain model (0x00CEFA20) four times over: each new-expression's name temporary is conditional on the
//   allocation, so VC7.1 keeps one flag word in ebx (bits 1, 2, 4, 8, cleared after each temporary's destructor) and the region
//   name is a second CCharString temporary built after the new-expression. CSpawnedFunc<T> from the shared header (its ctor
//   expands inline: base ctor, vtable, Func, PClass).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CSpawnedFuncBase::PPredicate (header contract)
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; every Fable script derives from
//   CLevelScriptBase<CQ_SunnyvaleMasterData>: Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGameflowAssistanceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();                                 // slot 2
    void Tutorial_CombatMultiplier();                    // 0x00CEFCC0
    void Tutorial_LowHealth();                           // 0x00CEFFB0
    void Tutorial_LowMagic();                            // 0x00CF0180
    void Tutorial_RenownLevelIncrease();                 // 0x00CF02A0
};
void CGameflowAssistanceScript::Main()
{
    AddSpawnedFunction(new CSpawnedFunc<CGameflowAssistanceScript>("Tutorial_CombatMultiplier", 0, this, &CGameflowAssistanceScript::Tutorial_CombatMultiplier), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CGameflowAssistanceScript>("Tutorial_LowHealth", 0, this, &CGameflowAssistanceScript::Tutorial_LowHealth), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CGameflowAssistanceScript>("Tutorial_LowMagic", 0, this, &CGameflowAssistanceScript::Tutorial_LowMagic), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CGameflowAssistanceScript>("Tutorial_RenownLevelIncrease", 0, this, &CGameflowAssistanceScript::Tutorial_RenownLevelIncrease), CCharString(""));
}
}