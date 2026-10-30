// NScript::CV_FishermanScript::CFisherman::Init() -- the fisherman entity's Init
// Leaf: ?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7BD60, 680 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the CFisherman entity vtable 0x012EA0B4, stored by the landed CFisherman::Alloc
//   0x00E7B7B0; FableWin.pdb names ?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ. The spawned member is
//   ?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z 0x00E7C010 under "ParentClass." + "NPCAttitudeProcess".
//   Catalog label CScriptGameResourceObjectScriptedThingBase_HandleFishermanNPCLogic is wrong.
// Callees: CGameScriptInterface slots 517 EntitySetAsKillable, 72 GetThingWithScriptName, 472 EntityTeleportToThing, 474
//   EntitySetFacingAngle, 426 CloseDoor; CScriptThing slot GetAngleXY (virtual), copy 0x004ABE90 / ~CScriptThing 0x004AA840,
//   ~CBaseClass 0x0099A2E0, operator new 0x00BFEA1A (0x48) / delete 0x00BFE9BC, CCharString 0x0099EBF0 / 0x0099EAE0, operator+
//   0x0099F570, CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450, CScriptBase::AddSpawnedFunction 0x00CB7E50.
// Data: CSpawnedFunc1<CV_FishermanScript, CScriptThing> vtable 0x012EA164, "MK_FI_FISHER" 0x012EA194, "FishermanHutDoor"
//   0x012CA4E0, "ParentClass." 0x012C9934, "NPCAttitudeProcess" 0x012EA180, "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14): HeroHasSavedFishermanFromWasps +0x49, FishermanMustRunAway +0x50; CFisherman: Game +4,
//   Me +8, ParentClass +0x14.
// Shape (family L139, landed 0x00E0DAA0): `#pragma inline_depth(1)`; the CSpawnedFunc1 ctor must be __forceinline (a plain
//   inline ctor stays a call at depth 1), its member copy of Me is the out-of-line 0x004ABE90; the region name is SpawnFunction's
//   default CCharString argument (SpawnFunction is a source-level forceinline helper with no retail symbol).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo {
public:
    long References;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->References;
    }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }   // out of line 0x004ABE90
inline CScriptThing::~CScriptThing() { }                // out of line 0x004AA840

template <class T, class P1> class CSpawnedFunc1 : public CSpawnedFuncBase {
public:
    void (T::*Func)(P1);                                // +0x34
    T* PClass;                                          // +0x38
    P1 Par1;                                            // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), P1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishermanScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool IntroPlayed;                                   // +0x48
    bool HeroHasSavedFishermanFromWasps;                // +0x49
    long WaspsAlive;                                    // +0x4c
    bool FishermanMustRunAway;                          // +0x50
    bool QuestCardGiven;                                // +0x51
    long FishermanTasks;                                // +0x54
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const CCharString& regionName = CCharString("")) { AddSpawnedFunction(func, regionName); }
    void NPCAttitudeProcess(CScriptThing thing);        // 0x00E7C010
    class CFisherman;
};
class CV_FishermanScript::CFisherman : public CEntityScriptBase {
public:
    CV_FishermanScript*     ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void Init();
};

#pragma inline_depth(1)
void CV_FishermanScript::CFisherman::Init()
{
    ParentClass->FishermanMustRunAway = false;
    Game->EntitySetAsKillable(Me, false, true);
    if (!ParentClass->HeroHasSavedFishermanFromWasps) {
        Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("MK_FI_FISHER"), false);
        Game->EntitySetFacingAngle(Me, Game->GetThingWithScriptName("MK_FI_FISHER").GetAngleXY(), true);
        Game->CloseDoor(Game->GetThingWithScriptName("FishermanHutDoor"));
    }
    ParentClass->SpawnFunction(new CSpawnedFunc1<CV_FishermanScript, CScriptThing>(CCharString("ParentClass.") + CCharString("NPCAttitudeProcess"), 0, ParentClass, &CV_FishermanScript::NPCAttitudeProcess, Me));
}
}   // namespace NScript