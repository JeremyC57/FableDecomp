#ifndef FABLE_ENGINE_CV_PARANOIDWHISPERSSCRIPT_H
#define FABLE_ENGINE_CV_PARANOIDWHISPERSSCRIPT_H
// Original Ego_r132b2/132e0; native man factory00ebfb00 allocates28 and stores vtable012f1010.
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ParanoidWhispersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    typedef CV_ParanoidWhispersScript TMyClass;
    typedef CQ_SunnyvaleMasterData TMyMasterScriptData;
    CV_ParanoidWhispersScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CCharString FABLE_FASTCALL StatGetClassName();
    static CScriptDataBase* (FABLE_FASTCALL *FABLE_FASTCALL StatGetMasterScriptDataAlloc())();
    virtual void RegisterMain();
    long ParanoidState;
    bool HideDiggingSpot;
    enum PARANOID_STATES { PS_NONE=0, PS_REPOSITION=1, PS_WHISPERING=2, PS_WHISTLING=3, PS_EXPLAINING_ALL=4, PS_ANGRY=5, PS_MAX=6 };
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
    void CheckForNecklaceAndQuit();
    void WatchForAngryTraders();
    class CSecretWhisperingMan : public CEntityScriptBase {
    public:
        typedef CV_ParanoidWhispersScript TParentClass;
        CSecretWhisperingMan(CV_ParanoidWhispersScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ParanoidWhispersScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_ParanoidWhispersScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        C3DVector StartPos;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CPW_NecklaceDiggingSpot : public CEntityScriptBase {
    public:
        typedef CV_ParanoidWhispersScript TParentClass;
        CPW_NecklaceDiggingSpot(CV_ParanoidWhispersScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ParanoidWhispersScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_ParanoidWhispersScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CSecretWhisperingFriend : public CEntityScriptBase {
    public:
        typedef CV_ParanoidWhispersScript TParentClass;
        CSecretWhisperingFriend(CV_ParanoidWhispersScript&,CScriptDataBase*,const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ParanoidWhispersScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CV_ParanoidWhispersScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        C3DVector StartPos;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ParanoidWhispersScript)==0x50);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ParanoidWhispersScript::CSecretWhisperingMan)==0x28);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ParanoidWhispersScript::CPW_NecklaceDiggingSpot)==0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_ParanoidWhispersScript::CSecretWhisperingFriend)==0x28);
#endif
