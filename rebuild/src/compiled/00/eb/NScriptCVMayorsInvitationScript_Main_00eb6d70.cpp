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
class CSpawnedFuncBase { public:                               // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

struct __declspec(novtable) CEntityScriptBindingBase {
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
class CGameScriptInterface { public:                       // retail vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities(void) const;   // slot 64 (+0x100) 0x006E7460
};
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