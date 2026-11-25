#ifndef FABLE_ENGINE_CQS_GUARDIANSISTERINFO2_SISTERINBANDITCAMPSCRIPT_H
#define FABLE_ENGINE_CQS_GUARDIANSISTERINFO2_SISTERINBANDITCAMPSCRIPT_H
// Original Ego_r10e29/10e39; native owner4c (Alloc00e277e0), child28 (Alloc00e26af0, vtable012e1f74).
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianSisterInfo2_SisterInBanditCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
    bool GuardianSpokeToHero; // native48; donor5c due to donor base
    class CMazeAtTavern : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQS_GuardianSisterInfo2_SisterInBanditCampScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQS_GuardianSisterInfo2_SisterInBanditCampScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long TalkCounter;
        bool WavedOver;
        long HitCount;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
#ifdef FABLE_RETAIL_CQS_GUARDIANSISTERINFO2_SISTERINBANDITCAMPSCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00E277E0 Alloc): FableWin.pdb ??_7CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: CQS_GuardianSisterInfo2_SisterInBanditCampScript, Alloc. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
    CQS_GuardianSisterInfo2_SisterInBanditCampScript(CScriptDataBase* data, CGameScriptInterfaceBase& game); // FableWin.pdb ??0CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00E277E0
#endif
};
}
FABLE_STATIC_ASSERT_AT(34, sizeof(NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript)==0x4c);
FABLE_STATIC_ASSERT_AT(35, sizeof(NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern)==0x28);
#endif
