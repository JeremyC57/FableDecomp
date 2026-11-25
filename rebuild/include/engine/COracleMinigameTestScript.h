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
#ifdef FABLE_RETAIL_ORACLE_MAIN_TEST_PASS27
    COracleMinigameTestScript(CScriptDataBase*, CGameScriptInterfaceBase&);
#endif
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    bool MissionSucceeded;
    class COracleSpectator : public CEntityScriptBase {
    public:
#ifdef FABLE_RETAIL_ORACLE_MAIN_TEST_PASS27
        COracleSpectator(COracleMinigameTestScript&, CScriptDataBase*, const CScriptThing&);
#endif
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
#ifdef FABLE_RETAIL_CORACLEMINIGAMETESTSCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00F01330 Alloc): FableWin.pdb ??_7COracleMinigameTestScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: COracleMinigameTestScript, Alloc. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
#ifndef FABLE_RETAIL_ORACLE_MAIN_TEST_PASS27
    COracleMinigameTestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game); // FableWin.pdb ??0COracleMinigameTestScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
#endif
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00F01330
#endif
};
}
FABLE_STATIC_ASSERT_AT(57, sizeof(NScript::COracleMinigameTestScript)==0x4c);
FABLE_STATIC_ASSERT_AT(58, sizeof(NScript::COracleMinigameTestScript::COracleGamesmaster)==0x20);
#endif
