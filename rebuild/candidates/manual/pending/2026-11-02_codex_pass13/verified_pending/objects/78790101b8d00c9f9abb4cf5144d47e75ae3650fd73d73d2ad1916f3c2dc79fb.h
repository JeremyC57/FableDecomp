#ifndef FABLE_ENGINE_CQ_BOWERSTONETOWNLIFEINTROSCRIPT_H
#define FABLE_ENGINE_CQ_BOWERSTONETOWNLIFEINTROSCRIPT_H
// Retail model (Codex pass13): original Ego_r TPI11c93/11ca3; native parent flags48/49/4a.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BowerstoneTownLifeIntroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    virtual void OnPersist(CPersistContext&);
    bool ArenaFinished;
    bool DoneGuardSpeech;
    bool PoshGuardComplete;
    class CBowerstoneIntroGuardToPosh : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BowerstoneTownLifeIntroScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_BowerstoneTownLifeIntroScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CBowerstoneIntroGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BowerstoneTownLifeIntroScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_BowerstoneTownLifeIntroScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPersist();
        virtual void OnPredicateFail();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BowerstoneTownLifeIntroScript)==0x4c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh)==0x1c);
#endif
