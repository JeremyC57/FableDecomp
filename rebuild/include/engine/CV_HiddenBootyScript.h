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
#ifdef FABLE_RETAIL_CV_HIDDENBOOTYSCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00E93CF0 Alloc): FableWin.pdb ??_7CV_HiddenBootyScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: Alloc. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00E93CF0
#endif
};
}
FABLE_STATIC_ASSERT_AT(33, sizeof(NScript::CV_HiddenBootyScript)==0x4c);
FABLE_STATIC_ASSERT_AT(34, sizeof(NScript::CV_HiddenBootyScript::CM_HiddenBooty)==0x24);
#endif
