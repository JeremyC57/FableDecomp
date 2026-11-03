#ifndef FABLE_ENGINE_CORACLEMINIGAMETESTSCRIPT_H
#define FABLE_ENGINE_CORACLEMINIGAMETESTSCRIPT_H
// Retail model (Codex pass13): original Ego_r TPI12cee/12d1d; retail vtable012f7100.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class COracleMinigameTestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    bool MissionSucceeded;
    class COracleSpectator : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(COracleMinigameTestScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        COracleMinigameTestScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long NextLine;
        virtual void Init();
        virtual void Main();
    };
    class CGamesmasterOracleMarker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(COracleMinigameTestScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        COracleMinigameTestScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Main();
    };
    class COracleGamesmaster : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(COracleMinigameTestScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        COracleMinigameTestScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool SaidIntro;
        virtual void Init();
        virtual void OnPersist (CPersistContext&);
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::COracleMinigameTestScript)==0x4c);
FABLE_STATIC_ASSERT(sizeof(NScript::COracleMinigameTestScript::COracleGamesmaster)==0x20);
#endif
