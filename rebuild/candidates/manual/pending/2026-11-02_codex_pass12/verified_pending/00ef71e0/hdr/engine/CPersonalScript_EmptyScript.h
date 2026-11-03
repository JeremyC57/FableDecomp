#ifndef FABLE_ENGINE_CPERSONALSCRIPT_EMPTYSCRIPT_H
#define FABLE_ENGINE_CPERSONALSCRIPT_EMPTYSCRIPT_H
#include "engine/CEmptyData.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
// Original PDB owner 0x60; retail CScriptBase is 0x14 bytes smaller, so ReachedGoalInStealthMission is +0x48.
class CPersonalScript_EmptyScript : public CLevelScriptBase<CEmptyData> {
public:
    typedef CPersonalScript_EmptyScript TMyClass;
    typedef CEmptyData TMyMasterScriptData;
    CPersonalScript_EmptyScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CCharString FABLE_FASTCALL StatGetClassName();
    static CScriptDataBase* (FABLE_FASTCALL * FABLE_FASTCALL StatGetMasterScriptDataAlloc())();
    virtual void RegisterMain();
    virtual void Main();
    bool ReachedGoalInStealthMission;
    void InitialiseVariables();
    void CheckForHeroActions();
    class CCrate1;
    class CCrate2;
    class CCrateDeathMarker;
    class CWhispie;
    class CBanditWalker;
    class CLookAtMe1;
    class CTrappedMan;
    class CCinemaMan;
    class CKillMan;
    class CSwordChest;
    class CArcherMan;
    class CTriggerCrate;
    class CStealthGuard;
    class CGoalChest;
    class CScrumpyCrate;
    class CDoor;
    class CBeggar;
    class CUndeadDude;
    class COpinionMan;
    class CTestPos;
    class CWalker;
    class CChatter1;
    class CChatter2;
    class CLeader;
    class CFollower;
    class CMeanBandit;
    class CGhostDude;
    class CUndeadFellow;
    class CVillager;
    class CTestPerson;
    void ChangeAlpha(CScriptThing);
    void TestNewThreadSpawningStuff();
};
// PDB: CArcherMan 0x1c, ParentClass is a reference +0x14 and PMasterData a pointer +0x18.
// Native Alloc00ef2420 allocates0x1c, stores vtable012f61ec, ParentClass and PMasterData. Its slot1 is00ef71e0.
class CPersonalScript_EmptyScript::CArcherMan : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CArcherMan(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CPersonalScript_EmptyScript)==0x4c);
FABLE_STATIC_ASSERT(sizeof(NScript::CPersonalScript_EmptyScript::CArcherMan)==0x1c);
#endif
