// NScript::CTestTheInventoryScript::TestForLampOn() -- test script thread: wait (frame by frame) until the hero's hand lamp is lit; then show the lamp-on message for 3 seconds, pause 3 seconds and spawn the TestForLampOff thread
// Leaf: ?TestForLampOn@CTestTheInventoryScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEEC00, 274 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced twice, both as the CSpawnedFunc member pointer stored at +0x34 next to the
//   "TestForLampOn" thread name 0x012F59DC: in the landed CTestTheInventoryScript::Main 0x00EEEA90 (site 0x00EEEB6B) and in
//   the landed twin TestForLampOff 0x00EEED20 (site 0x00EEEDED); this row in turn spawns "TestForLampOff" running 0x00EEED20.
//   Ego_r.pdb ?TestForLampOn@CTestTheInventoryScript@NScript@@QAEXXZ (0x00F69770, right before TestForLampOff 0x00F698F0, the
//   retail order). Catalog label sub_00eeec00.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50 (Ego_r), operator new 0x00BFEA1A,
//   CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   TestForLampOff 0x00EEED20 (the new thread's member pointer); CGameScriptInterface slots 7 NewScriptFrame,
//   111 ShowOnScreenMessage, 185 IsHeroHandLampLit, 376 Pause.
// Data: CSpawnedFunc<CTestTheInventoryScript> vtable 0x012F59AC (emitted inline).
// Literals: "You've turned the lamp on. Bully for you." 0x012F5A04, "TestForLampOff" 0x012F59F4, "" 0x0122D70E, 3.0f immediates.
// Layout: retail CScriptBase 0x40 bytes, CLevelScriptBase Game +0x40 / MasterScriptData +0x44, IgnoreNextLoad +0x48 (Ego_r
//   0x5c - 0x14; not touched here).
// Layout: engine/CSpawnedFuncBase.h is flattened (its vptr is a data member), so CSpawnedFuncBase is modelled locally as the
//   polymorphic base of CSpawnedFunc<T> (0x34 bytes; Func +0x34, PClass +0x38, new size 0x3c), as the landed quest Mains do.
// Shape: the landed twin TestForLampOff 0x00EEED20 with the wait negated; the spawn is the CV_RockTrollFirstEncounterScript::Main
//   0x00EC3D40 CSpawnedFunc new-expression (its name a conditional temporary guarded by bit 0 of ebx, the flag word zeroed at
//   entry; the empty region name an explicit temporary built after the new).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CTestTheInventoryScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool IgnoreNextLoad;                                // +0x48
    void TestForLampOn();
    void TestForLampOff();                              // 0x00EEED20
};

void CTestTheInventoryScript::TestForLampOn()
{
    while (!Game->IsHeroHandLampLit()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ShowOnScreenMessage("You've turned the lamp on. Bully for you.", 3.0f);
    Game->Pause(3.0f);
    AddSpawnedFunction(new CSpawnedFunc<CTestTheInventoryScript>("TestForLampOff", 0, this, &CTestTheInventoryScript::TestForLampOff),
                       CCharString(""));
}
}   // namespace NScript