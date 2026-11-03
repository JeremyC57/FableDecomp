#ifndef FABLE_ENGINE_CV_ASSASSINATTACKSSCRIPT_H
#define FABLE_ENGINE_CV_ASSASSINATTACKSSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CCharString.h"
#include "stlport/fable_containers.h"
// Original PDB owner and nested classes; native owner0xb8, RegionInfo58, RegionKillCountsa8.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_AssassinAttacksScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum AA_LocationIdentifier {
        AA_LocationHookCoast=0,
        AA_LocationDesertedFarm=1,
        AA_LocationWitchwood=2,
        AA_LocationKnotholeGlade=3,
        AA_LocationPrisonPath=4,
        AA_LocationMax=5
    };
    struct RegionData { CCharString name;long count,max;bool assassinActive; };
    CV_AssassinAttacksScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void InitialiseRegionSpecificInfo();
    AA_LocationIdentifier RegionIndex;
    long AssassinObjectCount;
    bool Activated;
    CCharString RegionOfMostRecentKill;
    RegionData RegionInfo[AA_LocationMax];
    std::vector<long> RegionKillCounts;
    bool QuestComplete,RewardsGiven,GivenQuestCard;
    class CM_AssassinTrigger:public CEntityScriptBase {
    protected:
        CV_AssassinAttacksScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CM_AssassinTrigger(CV_AssassinAttacksScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_AssassinAttacksScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        CCharString regionName;
        AA_LocationIdentifier regionId;
        virtual void Init();
        virtual void Main();
    };
    class CAttackingAssassin:public CEntityScriptBase {
    protected:
        CV_AssassinAttacksScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CAttackingAssassin(CV_AssassinAttacksScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_AssassinAttacksScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        bool Screamed;
        virtual void Init();
        virtual void Main();
    };
    class CChestHidingAssassin:public CEntityScriptBase {
    protected:
        CV_AssassinAttacksScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CChestHidingAssassin(CV_AssassinAttacksScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_AssassinAttacksScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        bool Screamed;
        virtual void Init();
        virtual void Main();
    };
    class CAssassinChest:public CEntityScriptBase {
    protected:
        CV_AssassinAttacksScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CAssassinChest(CV_AssassinAttacksScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_AssassinAttacksScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        virtual void Init();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_AssassinAttacksScript)==0xb8);
#endif
