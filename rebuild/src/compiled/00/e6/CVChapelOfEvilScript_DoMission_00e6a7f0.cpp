// CV_ChapelOfEvilScript::DoMission() -- wait until DarkwoodChapel is loaded, spawn the WatchForLeaving thread, hide the
// quest info, take a frame and run EndMission
// Leaf: DoMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6A7F0, 324 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, operator new @ 0x00BFEA1A, CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450
//          (CSpawnedFunc<CV_ChapelOfEvilScript> vtable 0x012E8094, func 0x00E6A940), CScriptBase::AddSpawnedFunction
//          @ 0x00CB7E50, EndMission @ 0x00E6A9E0; CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 12 IsRegionLoaded
//          (+0x30), 321 DisplayQuestInfo (+0x504).
// Layout: interface at +0x40 (CQuestScriptBase model); spawn wrapper as the landed CQ_HangingTreeEvilScript::
//          MonitorEnemies 0x00D6BE20 (inline `const char*` SpawnFunction, `__forceinline` CSpawnedFunc ctor).
// Shape: WAIT_UNTIL(loaded) is `while (!loaded)` (neg/sbb/inc); the thread name temporary is guarded by the
//          entry-zeroed flag (built only when operator new succeeds).
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CV_ChapelOfEvilScript : CScriptBase {
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
    void WatchForLeaving();
    void EndMission();                                   // 0x00E6A9E0
    void DoMission();
};

void CV_ChapelOfEvilScript::DoMission()
{
    while (!GSI->IsRegionLoaded("DarkwoodChapel")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("WatchForLeaving", 0, this, &CV_ChapelOfEvilScript::WatchForLeaving), CCharString(""));
    GSI->DisplayQuestInfo(false);
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    EndMission();
}