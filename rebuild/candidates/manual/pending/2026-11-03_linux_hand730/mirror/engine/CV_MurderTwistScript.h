#ifndef FABLE_ENGINE_CV_MURDERTWISTSCRIPT_H
#define FABLE_ENGINE_CV_MURDERTWISTSCRIPT_H
// Retail model (Codex pass13): original Ego_r TPI11749/11786, owner fields48/4c/50/54 and nested BrainState1c.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    virtual void OnPersist(CPersistContext&);
    void MagicRuneProcess();
    void GemsFoundProcess();
    enum MURDER_TWIST_STATES { MTS_NONE=0, MTS_AWAITING_HELP=1, MTS_AVENGE_YOUNGER_BROTHER=2, MTS_AVENGE_BROTHER=3, MTS_AVENGE_OLDER_BROTHER=4, MTS_REVENGE_COMPLETE=5, MTS_MAX=6 };
    enum MURDER_TWIST_BROTHER_STATES { MTBS_NONE=0, MTBS_FIGHTING=1, MTBS_NEARLY_DEAD=2, MTBS_DEAD=3, MTBS_MAX=4 };
    enum MURDER_TWIST_EVENTS { MTE_NONE=0, MTE_MEET_LEADER=1, MTE_FIND_BROTHER_OV=2, MTE_FIND_BROTHER_BS=3, MTE_FIND_BROTHER_BS_FRIEND=4, MTE_FIND_BROTHER_KHG=5, MTE_FIND_BROTHER_KHG_FRIEND=6, MTE_FIND_HIDDEN_GEM=7, MTE_MAX=8 };
    long MurderTwistItemsFound;
    long MurderTwistState;
    long MurderTwistEvent;
    std::vector<CScriptThing> MagicRuneRumbleList;
    class CMurderTwistBuriedGem : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_MurderTwistScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CMurderTwistBrotherOlderFriend : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_MurderTwistScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BrainState;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CMurderTwistBrotherFriend : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_MurderTwistScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BrainState;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CMurderTwistBrotherOlder : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_MurderTwistScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BrainState;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CMurderTwistBrother : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_MurderTwistScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BrainState;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CMurderTwistBrotherYounger : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_MurderTwistScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long BrainState;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CMurderTwistLeader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_MurderTwistScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MurderTwistScript)==0x60);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_MurderTwistScript::CMurderTwistBrotherOlder)==0x20);
#endif
