// 00d259a0 NScript::CQ_DragonBossFightScript::CDragon::OnPredicateFail
// Codex parked-50 best attempt: DIFFER(64v64); residue: 64/64 bytes; retail reuses saved-this ESI for the parent while the model uses EAX.
// Flags:
// Leaf: ?OnPredicateFail@CDragon@CQ_DragonBossFightScript@NScript@@EAEXXZ
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
// Layout: donor CCharString PStringData0, sizeof4; native ctor/dtor receive frame word.
class CCharStringData;
class CCharString {
 CCharStringData* PStringData;
public:
 CCharString(const char*,long=-1);
 ~CCharString();
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_DragonBossFightScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
 class CDragon;
 enum EDragonState { DS_HEALTH_HIGH,DS_HEALTH_MEDIUM,DS_HEALTH_LOW,DS_HEALTH_VERY_LOW,DS_DEAD };
 // Layout: donor DragonState+0x5c follows level-script base; retail base is0x48.
 EDragonState DragonState;
 // Remaining parent fields are unobserved by this leaf; no complete-size claim.
 void SetDragonState(EDragonState state) { DragonState=state; }
};
class CQ_DragonBossFightScript::CDragon:public CEntityScriptBase {
protected:
 // Layout: raw Ego_r proves a reference, not a pointer, at0x14; master-data pointer0x18.
 CQ_DragonBossFightScript& ParentClass;
 CQ_SunnyvaleMasterData* PMasterData;
private:
 float MediumHealth,LowHealth,VeryLowHealth;
 virtual void Main();
 virtual void Init();
 virtual void OnPredicateFail();
public:
 virtual CScriptBase* GetParentScript() const;
};
void CQ_DragonBossFightScript::CDragon::OnPredicateFail() {
 if(Me.MsgIsKilledBy("")) ParentClass.SetDragonState(DS_DEAD);
}
}
