// CV_MayorsInvitationScript::Main() -- the Mayor's Invitation quest main: resume the Thunder duel monitor if the quest was saved in that stage, bind the LadyGrey / WiseVillager / MI_InfoKeeper entity scripts and spawn the WatchForTermination, WatchExposeMayorProgress and DoMission threads
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB6D70, 0x369 bytes, plain `ret`.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase(const CCharString&, unsigned long)
//   0x00CDD450 (CSpawnedFunc<CV_MayorsInvitationScript> vtable 0x012F0548), AddSpawnedFunction 0x00CB7E50,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012F0590), CScriptBase::PostAddScriptedEntities 0x00CB8930;
//   threads MonitorThunderHeroDuel 0x00EB70F0, WatchForTermination 0x00EB7370 (manifest "MayorsInvitation_OnEnter" is
//   propagated), WatchExposeMayorProgress 0x00EB94A0, DoMission 0x00EB75F0; allocators CLadyGrey::Alloc 0x00EB78B0,
//   CWiseVillager::Alloc 0x00EB79F0, CMI_InfoKeeper::Alloc 0x00EB8810 (FableWin.pdb names; manifest labels propagated);
//   CGameScriptInterface slot 64 PostAddScriptedEntities.
// Shape: the landed CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620 model: one flag word in ebx guards every
//   name temporary that exists only when its operator new succeeded.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, HeroHasDefeatedThunder +0x4e, HeroHasLostToThunder +0x4f, WhichQuest +0x58.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
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

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

class CGameScriptInterface;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CV_MayorsInvitationScript : public CScriptBase {
public:
    struct CLadyGrey {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MayorsInvitationScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EB78B0
    };
    struct CWiseVillager {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MayorsInvitationScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EB79F0
    };
    struct CMI_InfoKeeper {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MayorsInvitationScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EB8810
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool MissionOver;                                    // +0x4a
    bool HeroAbortedQuest;                               // +0x4b
    bool HeroLearnedSecret;                              // +0x4c
    bool HeroReturnedFromParanoidQuest;                  // +0x4d
    bool HeroHasDefeatedThunder;                         // +0x4e
    bool HeroHasLostToThunder;                           // +0x4f
    bool QuestCardGiven;                                 // +0x50
    long SecretsLearned;                                 // +0x54
    long WhichQuest;                                     // +0x58
    void Main();
    void MonitorThunderHeroDuel();                       // 0x00EB70F0
    void WatchForTermination();                          // 0x00EB7370
    void WatchExposeMayorProgress();                     // 0x00EB94A0
    void DoMission();                                    // 0x00EB75F0
};
} // namespace NScript
using NScript::CV_MayorsInvitationScript;

void NScript::CV_MayorsInvitationScript::Main()
{
    if (WhichQuest == 5 && !HeroHasDefeatedThunder && !HeroHasLostToThunder) {
        if (IsActiveThreadTerminating())
            return;
        SpawnFunction(new CSpawnedFunc<CV_MayorsInvitationScript>("MonitorThunderHeroDuel", 0, this, &NScript::CV_MayorsInvitationScript::MonitorThunderHeroDuel));
    }
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MayorsInvitationScript>("LadyGrey", this, &CLadyGrey::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MayorsInvitationScript>("WiseVillager", this, &CWiseVillager::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MayorsInvitationScript>("MI_InfoKeeper", this, &CMI_InfoKeeper::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_MayorsInvitationScript>("WatchForTermination", 0, this, &NScript::CV_MayorsInvitationScript::WatchForTermination));
    SpawnFunction(new CSpawnedFunc<CV_MayorsInvitationScript>("WatchExposeMayorProgress", 0, this, &NScript::CV_MayorsInvitationScript::WatchExposeMayorProgress));
    SpawnFunction(new CSpawnedFunc<CV_MayorsInvitationScript>("DoMission", 0, this, &NScript::CV_MayorsInvitationScript::DoMission));
}