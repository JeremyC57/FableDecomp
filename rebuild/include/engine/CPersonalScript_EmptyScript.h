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
class CPersonalScript_EmptyScript::CChatter2 : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CChatter2(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CChatter1 : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CChatter1(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
#ifdef FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES
// Retail opt-in (chunk 799): the other 27 entity classes bound by CPersonalScript_EmptyScript::Main 0x00EF0090, the same shape as
// CArcherMan above (Ego_r.pdb nested classes of CPersonalScript_EmptyScript, 0x1c each; every Alloc is a landed
// CPersonalScript_EmptyScript::C<Name>::Alloc row whose address Main stores in the binding, binding-name literal = class name less its C).
// Opt-in because the landed entity Mains (CWhispie 0x00EF1690, CTrappedMan 0x00EF1E10, ...) define their own class bodies.
class CPersonalScript_EmptyScript::CTestPerson : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CTestPerson(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CMeanBandit : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CMeanBandit(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CLeader : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CLeader(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CFollower : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CFollower(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CTestPos : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CTestPos(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::COpinionMan : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    COpinionMan(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CWalker : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CWalker(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CUndeadDude : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CUndeadDude(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CBeggar : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CBeggar(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CScrumpyCrate : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CScrumpyCrate(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CVillager : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CVillager(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CGhostDude : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CGhostDude(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CDoor : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CDoor(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CUndeadFellow : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CUndeadFellow(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CStealthGuard : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CStealthGuard(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CGoalChest : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CGoalChest(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CTriggerCrate : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CTriggerCrate(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CKillMan : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CKillMan(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CCinemaMan : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CCinemaMan(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CSwordChest : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CSwordChest(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CBanditWalker : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CBanditWalker(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CLookAtMe1 : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CLookAtMe1(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CTrappedMan : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CTrappedMan(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CWhispie : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CWhispie(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CCrate1 : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CCrate1(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CCrate2 : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CCrate2(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
class CPersonalScript_EmptyScript::CCrateDeathMarker : public CEntityScriptBase {
public:
    typedef CPersonalScript_EmptyScript TParentClass;
protected:
    CPersonalScript_EmptyScript& ParentClass;
    CEmptyData* PMasterData;
public:
    CCrateDeathMarker(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CPersonalScript_EmptyScript&,CScriptDataBase*,const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CEmptyData* GetMasterData();
private:
    virtual void Init();
    virtual void Main();
};
#endif // FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES
}
FABLE_STATIC_ASSERT(sizeof(NScript::CPersonalScript_EmptyScript)==0x4c);
FABLE_STATIC_ASSERT(sizeof(NScript::CPersonalScript_EmptyScript::CArcherMan)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CPersonalScript_EmptyScript::CChatter2)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CPersonalScript_EmptyScript::CChatter1)==0x1c);
#endif
