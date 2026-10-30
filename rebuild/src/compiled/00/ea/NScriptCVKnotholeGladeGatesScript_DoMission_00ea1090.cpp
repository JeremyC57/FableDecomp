// CV_KnotholeGladeGatesScript::DoMission() -- wait for KnotholeGlade, look up and close the gate, spawn the WatchForGates and WatchForCutscene1 threads, run phase one and end the mission unless it failed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA1090, 0x249 bytes, plain `ret`. PDB ?DoMission@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   operator new 0x00BFEA1A, CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450 (CSpawnedFunc<CV_KnotholeGladeGatesScript>
//   vtable 0x012ED9E8; WatchForGates 0x00EA12E0, WatchForCutscene1 0x00EA2720), AddSpawnedFunction 0x00CB7E50, PhaseOne
//   0x00EA1630, EndMission 0x00EA1360, ~CScriptThing 0x004AA840, operator delete 0x00BFE9BC; CGameScriptInterface slots
//   7 NewScriptFrame, 11 IsLevelLoaded, 72 GetThingWithScriptName, 426 CloseDoor.
// Shape: WAIT_UNTIL(level loaded) is `while (!loaded)` (neg/sbb/inc); `TheGate = GetThingWithScriptName(..)` expands the
//   inline counted-pointer assignment; each spawn passes the region as an implicit conversion temporary (built before
//   operator new) and the thread name lives only when new succeeded (flag bits in ebx), as the landed PhaseOne 0x00EA1630.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, MissionFailed +0x49, TheGate +0x5c.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    virtual ~CScriptThing();                             // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other)
    {
        PImp = other.PImp;
        return *this;
    }
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                   // +0x34
    T* PClass;                                           // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_KnotholeGladeGatesScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    void* Data;                                          // +0x44
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    unsigned char _pad4a[0x5c - 0x4a];
    CScriptThing TheGate;                                // +0x5c
    void WatchForGates();                                // 0x00EA12E0
    void WatchForCutscene1();                            // 0x00EA2720
    void PhaseOne();                                     // 0x00EA1630
    void EndMission();                                   // 0x00EA1360
    void DoMission();
};
} // namespace NScript
using NScript::CV_KnotholeGladeGatesScript;

void NScript::CV_KnotholeGladeGatesScript::DoMission()
{
    while (!Game->IsLevelLoaded("KnotholeGlade")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    TheGate = Game->GetThingWithScriptName("KG_Gate");
    Game->CloseDoor(TheGate);
    AddSpawnedFunction(new CSpawnedFunc<CV_KnotholeGladeGatesScript>("WatchForGates", 0, this,
                                                                    &NScript::CV_KnotholeGladeGatesScript::WatchForGates),
                       "KnotholeGlade");
    AddSpawnedFunction(new CSpawnedFunc<CV_KnotholeGladeGatesScript>("WatchForCutscene1", 0, this,
                                                                    &NScript::CV_KnotholeGladeGatesScript::WatchForCutscene1),
                       "KnotholeGlade");
    PhaseOne();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!MissionFailed)
        EndMission();
}