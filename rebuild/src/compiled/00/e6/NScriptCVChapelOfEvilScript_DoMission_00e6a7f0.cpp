// NScript::CV_ChapelOfEvilScript::DoMission() -- wait until DarkwoodChapel is loaded, spawn the WatchForLeaving thread, hide the
// quest info, take a frame and run EndMission
// Leaf: ?DoMission@CV_ChapelOfEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6A7F0, 324 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, operator new @ 0x00BFEA1A, CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450
//          (CSpawnedFunc<NScript::CV_ChapelOfEvilScript> vtable 0x012E8094, func 0x00E6A940), CScriptBase::AddSpawnedFunction
//          @ 0x00CB7E50, EndMission @ 0x00E6A9E0; CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 12 IsRegionLoaded
//          (+0x30), 321 DisplayQuestInfo (+0x504).
// Layout: interface at +0x40 (CLevelScriptBase<T>::Game); spawn wrapper as the landed CQ_HangingTreeEvilScript::
//          MonitorEnemies 0x00D6BE20 (inline `const char*` SpawnFunction, `__forceinline` CSpawnedFunc ctor).
// Shape: WAIT_UNTIL(loaded) is `while (!loaded)` (neg/sbb/inc); the thread name temporary is guarded by the
//          entry-zeroed flag (built only when operator new succeeds).
// Naming (NF162, 2026-11-20, q6272 / q6273): was a row-local global `struct CV_ChapelOfEvilScript : CScriptBase` (leaf
//   ?DoMission@CV_ChapelOfEvilScript@@QAEXXZ, also the CSpawnedFunc<CV_ChapelOfEvilScript> vtable / ctor spellings and the EndMission
//   callee ?EndMission@CV_ChapelOfEvilScript@@QAEXXZ); now on the shared engine/CV_ChapelOfEvilScript.h. Identity: Ego_r.pdb
//   ?DoMission@CV_ChapelOfEvilScript@NScript@@QAEXXZ, the thread "DoMission" Main 0x00E693F0 spawns (CSpawnedFunc<CV_ChapelOfEvilScript>
//   vtable 0x012E8094; the landed Main binds this spelling); EndMission 0x00E6A9E0 (Ego_r.pdb, NScript) moves in the same hand-back.
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
#include "engine/CV_ChapelOfEvilScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

void NScript::CV_ChapelOfEvilScript::DoMission()
{
    while (!Game->IsRegionLoaded("DarkwoodChapel")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("WatchForLeaving", 0, this, &CV_ChapelOfEvilScript::WatchForLeaving), CCharString(""));
    Game->DisplayQuestInfo(false);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    EndMission();
}
