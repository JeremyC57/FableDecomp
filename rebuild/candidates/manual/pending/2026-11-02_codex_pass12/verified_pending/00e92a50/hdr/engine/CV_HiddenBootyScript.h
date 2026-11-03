#ifndef FABLE_ENGINE_CV_HIDDENBOOTYSCRIPT_H
#define FABLE_ENGINE_CV_HIDDENBOOTYSCRIPT_H
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
// Original Ego_r TPI0x115fe and FableWin; native flags48..4b, size4c.
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HiddenBootyScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_HiddenBootyScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    bool QuestActivated,QuestComplete,FakeBootyAddedToDiggingSpot,RealBootyAddedToDiggingSpot;
    class CM_HiddenBooty:public CEntityScriptBase {
    protected:
        CV_HiddenBootyScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    public:
        CM_HiddenBooty(CV_HiddenBootyScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HiddenBootyScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript()const;
        CQ_SunnyvaleMasterData* GetMasterData();
    private:
        bool HasHeroGotAllClues,HasHeroGotAnyClues,PlayedIntroduction;
        CCharString ThingFromWhichHeroMightBeRewardedFrom;
        virtual void Init();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_HiddenBootyScript)==0x4c);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_HiddenBootyScript::CM_HiddenBooty)==0x24);
#endif
