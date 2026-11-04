// NScript::CQ_HangingTreeEvilScript::SpawnEnemies(const C3DVector&, CCharString, long) -- create count enemies of the given creature type near the position (HTE_AssaultGuard script), then the carried-over guards, attaching each to the active quest; collect the HTE_AssaultGuard things and start the MonitorEnemies thread
// Leaf: ?SpawnEnemies@CQ_HangingTreeEvilScript@NScript@@QAEXABVC3DVector@@VCCharString@@J@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6BB80, 662 bytes, `ret 0Ch` (the by-value CCharString is destroyed at the exit).
// Identity: Ego_r.pdb + FableWin.pdb ?SpawnEnemies@CQ_HangingTreeEvilScript@NScript@@QAEXABVC3DVector@@VCCharString@@J@Z.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CGameScriptInterface slots 92 CreateCreatureNearby, 655
//   GetActiveQuestName, 465 EntityAttachToScript, 75 GetAllThingsWithScriptName; CSpawnedFuncBase 0x00CDD450,
//   CScriptBase::AddSpawnedFunction 0x00CB7E50, MonitorEnemies 0x00D6BE20.
// Strings: "HTE_AssaultGuard" 0x012D2064, "MonitorEnemies" 0x012D22F0, "" 0x0122D70E.
// Layout (retail = Ego_r - 0x14): CarryOverGuards +0x70, CurrentEnemyVector +0xac.
// Shape: the landed SpawnParty 0x00D6C4A0 model (inline ~CScriptThing, flag word guarding the thread name).
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
#include "engine/CRGBColour.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x70 - 0x48];
    long                  CarryOverGuards;              // +0x70
    unsigned char         _pad74[0xac - 0x74];
    std::vector<CScriptThing, std::allocator<CScriptThing> > CurrentEnemyVector;   // +0xac
    void SpawnEnemies(const C3DVector& position, CCharString creature_type, long count);
    void MonitorEnemies();                              // 0x00D6BE20
};
void CQ_HangingTreeEvilScript::SpawnEnemies(const C3DVector& position, CCharString creature_type, long count)
{
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing enemy = Game->CreateCreatureNearby(creature_type, position, 5.0f, "HTE_AssaultGuard", false);
        Game->EntityAttachToScript(enemy, Game->GetActiveQuestName());
    }
    if (IsActiveThreadTerminating())
        return;
    for (long j = 0; j < CarryOverGuards; ++j) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing enemy = Game->CreateCreatureNearby(creature_type, position, 5.0f, "HTE_AssaultGuard", false);
        Game->EntityAttachToScript(enemy, Game->GetActiveQuestName());
    }
    if (IsActiveThreadTerminating())
        return;
    CarryOverGuards = 0;
    Game->GetAllThingsWithScriptName("HTE_AssaultGuard", CurrentEnemyVector);
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("MonitorEnemies", 0, this, &CQ_HangingTreeEvilScript::MonitorEnemies), CCharString(""));
}
}