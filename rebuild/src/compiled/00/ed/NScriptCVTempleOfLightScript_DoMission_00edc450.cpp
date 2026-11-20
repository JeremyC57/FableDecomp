// NScript::CV_TempleOfLightScript::DoMission() -- wait until Witchwood3 is loaded, spawn the WatchForLeaving thread, hide the
// quest info, take a frame and run EndMission
// Leaf: ?DoMission@CV_TempleOfLightScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDC450, 324 bytes (the Witchwood3 twin of CV_ChapelOfEvilScript::DoMission 0x00E6A7F0).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, operator new @ 0x00BFEA1A, CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450
//          (CSpawnedFunc<NScript::CV_TempleOfLightScript> vtable 0x012F3C7C, WatchForLeaving 0x00EDC5A0), CScriptBase::AddSpawnedFunction
//          @ 0x00CB7E50, EndMission @ 0x00EDC640; CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 12 IsRegionLoaded
//          (+0x30), 321 DisplayQuestInfo (+0x504).
// Layout: interface at +0x40 (CLevelScriptBase<T>::Game); spawn wrapper as the landed CQ_HangingTreeEvilScript::
//          MonitorEnemies 0x00D6BE20 (inline `const char*` SpawnFunction, `__forceinline` CSpawnedFunc ctor).
// Shape: WAIT_UNTIL(loaded) is `while (!loaded)` (neg/sbb/inc); the thread name temporary is guarded by the
//          entry-zeroed flag (built only when operator new succeeds).
// Naming (NF162, 2026-11-20, q6275): was a row-local global `struct CV_ChapelOfEvilScript` (leaf ?DoMission@CV_ChapelOfEvilScript@@QAEXXZ,
//   the same symbol as 0x00E6A7F0's old leaf: one name on two addresses) with stand-in callees WatchForLeaving_00edc5a0 /
//   EndMission_00edc640 and CSpawnedFunc<CV_ChapelOfEvilScript> for vtable 0x012F3C7C; now on the shared engine/CV_TempleOfLightScript.h.
//   Identity: Ego_r.pdb ?DoMission@CV_TempleOfLightScript@NScript@@QAEXXZ, the thread "DoMission" CV_TempleOfLightScript::Main 0x00EDB190
//   spawns through CSpawnedFunc<CV_TempleOfLightScript> vtable 0x012F3C7C (the landed Main binds this spelling; its only referencer);
//   it calls the landed NScript::CV_TempleOfLightScript::EndMission 0x00EDC640 and waits for Witchwood3 (the Temple of Light region).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites: defined inline after the header.
inline CScriptThing::~CScriptThing() { }
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_TempleOfLightScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

void NScript::CV_TempleOfLightScript::DoMission()
{
    while (!Game->IsRegionLoaded("Witchwood3")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CV_TempleOfLightScript>("WatchForLeaving", 0, this, &CV_TempleOfLightScript::WatchForLeaving), CCharString(""));
    Game->DisplayQuestInfo(false);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    EndMission();
}
