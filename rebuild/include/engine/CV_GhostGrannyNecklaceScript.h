#ifndef FABLE_ENGINE_CV_GHOSTGRANNYNECKLACESCRIPT_H
#define FABLE_ENGINE_CV_GHOSTGRANNYNECKLACESCRIPT_H
// Original Ego_r TPI11c50/11c6f; native daughter factory00e8be10 allocates24, vtable012ebb30.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_GhostGrannyNecklaceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    typedef CV_GhostGrannyNecklaceScript TMyClass;
    typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
    CV_GhostGrannyNecklaceScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CCharString FABLE_FASTCALL StatGetClassName();
    static CScriptDataBase* (FABLE_FASTCALL *FABLE_FASTCALL StatGetMasterScriptDataAlloc())();
    virtual void RegisterMain();
    bool WifeNeededForCutscene;
    bool HeroKnowsAboutNecklace;
    bool NecklaceReturned;
    bool BanditKilled;
    bool WarnedGettingCold;
    bool GuidedToBanditLocation;
    bool GhostToRest;
    bool QuestCardGiven;
    enum GRANNY_STATES { GS_NONE=0, GS_INTRO=1, GS_CANT_REST=2, GS_GIVING_CLUES=3, GS_NECKLACE_NOT_RETURNED=4, GS_GOING_TO_REST=5, GS_MAX=6 };
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
    void CreateBandit();
    void CheckBanditDead(CScriptThing);
    void GhostGrannyHints();
    class CGhostGrannyNecklace : public CEntityScriptBase {
    public:
        typedef CV_GhostGrannyNecklaceScript TParentClass;
        CGhostGrannyNecklace(CV_GhostGrannyNecklaceScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_GhostGrannyNecklaceScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_GhostGrannyNecklaceScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BrainState;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CGhostGrannyDaughterInLaw : public CEntityScriptBase {
    public:
        typedef CV_GhostGrannyNecklaceScript TParentClass;
        CGhostGrannyDaughterInLaw(CV_GhostGrannyNecklaceScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_GhostGrannyNecklaceScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_GhostGrannyNecklaceScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long TalkCounter;
        bool ShownItem;
        virtual void Init();
        virtual void Main();
    };
    class CGhostGrannySon : public CEntityScriptBase {
    public:
        typedef CV_GhostGrannyNecklaceScript TParentClass;
        CGhostGrannySon(CV_GhostGrannyNecklaceScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_GhostGrannyNecklaceScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_GhostGrannyNecklaceScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long TalkCounter;
        bool ShownItem;
        virtual void Init();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_GhostGrannyNecklaceScript)==0x50);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace)==0x20);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyDaughterInLaw)==0x24);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_GhostGrannyNecklaceScript::CGhostGrannySon)==0x24);
#endif
