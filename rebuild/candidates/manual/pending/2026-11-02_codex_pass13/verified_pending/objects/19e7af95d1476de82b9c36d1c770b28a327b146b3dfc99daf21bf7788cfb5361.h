#ifndef FABLE_ENGINE_CQ_OPENINGGRAVEYARDSECRETPASSAGESCRIPT_H
#define FABLE_ENGINE_CQ_OPENINGGRAVEYARDSECRETPASSAGESCRIPT_H
// Original Ego_r TPI12c06/12c51; retail base contraction: owner74, KeeperFriend1c.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OpeningGraveyardSecretPassageScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum { GHOST_DISTANCE=7 };
    bool finished_quest;
    bool DoorChange;
    bool ZombieCamPlayed;
    bool skeleton_visited;
    bool doors_open;
    bool GravekeeperIntroduced;
    bool GravekeeperCutsceneStart;
    bool UndeadCutscenePlayed;
    bool UndeadCutsceneStarted;
    bool SkeletonStuff[4];
    bool MissionSucceeded;
    bool MissionFailed;
    bool DoorClosed;
    CCharString hero_object[4];
    bool LieutenantAround;
    long FriendliesKilled;
    bool VisitorsAttacked;
    bool QuestStartScreenShown;
    bool HaveOpenedDemonDoor;
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    virtual void OnPersist(CPersistContext&);
    void InitializeVariables();
    void WatchForArmour();
    void WatchForTermination();
    void FadeObjectUp(CScriptThing);
    void CheckDoor();
    class CUndeadLieutenant : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool DoorOpen;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CSpeakingMarker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        unsigned long test;
        virtual void Init();
        virtual void Main();
    };
    class CSecretPassageGate : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CSecretPassageFirstGate : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CSecretPassageKeeperFriend : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CUndeadSpawnCutscene : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CStartGravekeeperCutsceneMarker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CSecretPassageWoman : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool AttackedByHero;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CSecretPassageMan : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool AttackedByHero;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CSecretPassageGraveyardKeeper : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool already_talked_to;
        bool Helmfound;
        bool Swordfound;
        bool Shieldfound;
        bool Armourfound;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CCeremonialEffect : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CSecretPassageSarcophagos : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CGraveyardDemon : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long TimesSpoken;
        virtual void Init();
        virtual void Main();
    };
    class CSecretPassageSkeleton : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OpeningGraveyardSecretPassageScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        enum ARTIFACTS { SWORD=0,SHIELD=1,ARMOUR=2,HELMET=3,LAST=4 };
        bool returned[4];
        CCharString skeleton_object[4];
        bool collected_all;
        virtual void Init();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OpeningGraveyardSecretPassageScript)==0x74);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageKeeperFriend)==0x1c);
#endif
