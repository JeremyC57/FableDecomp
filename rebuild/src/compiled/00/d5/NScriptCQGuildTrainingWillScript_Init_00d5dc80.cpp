// NScript::CQ_GuildTrainingWillScript::Init() -- reset the will tutorial: TargetsHit 0, TutorialState 1, both counters 0, TestFinished and BanditsDefeated false
// Leaf: ?Init@CQ_GuildTrainingWillScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D5DC80, 25 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the quest vtable 0x012D1038 (slot 1 = `NScript::CQ_GuildTrainingWillScript::RegisterMain`), stored by the quest ctor 0x00D5DC50; ?Init@CQ_GuildTrainingWillScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb. Label: the manifest calls that ctor "CEngineSubPrimitiveBase" (propagated).
// Label: the manifest's "sub_00d5dc80" is a gap-scan placeholder.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets until the first STL container. TargetsHit +0x48, TutorialState +0x4c, TotalTrainingDummiesCounter +0x50, GenericTutorialCounter +0x54, HeroHit +0x58, TestFinished +0x5c, BanditsDefeated +0x5d.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
class CGameScriptInterfaceBase;
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingWillScript : public CScriptBase {
public:
    virtual void Init();
    CGameScriptInterfaceBase* Game;                     // +0x40 (the ctor's CGameScriptInterfaceBase&; descriptive name)
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44 (the ctor's CScriptDataBase*; descriptive name)
    long TargetsHit;                                    // +0x48
    long TutorialState;                                 // +0x4c
    long TotalTrainingDummiesCounter;                   // +0x50
    long GenericTutorialCounter;                        // +0x54
    long HeroHit;                                       // +0x58
    bool TestFinished;                                  // +0x5c
    bool BanditsDefeated;                               // +0x5d
};
}

void NScript::CQ_GuildTrainingWillScript::Init()
{
    TargetsHit = 0;
    TutorialState = 1;
    TotalTrainingDummiesCounter = 0;
    GenericTutorialCounter = 0;
    TestFinished = false;
    BanditsDefeated = false;
}