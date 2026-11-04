#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONSUMMONERLIGHTNINGATTACKTARGET_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONSUMMONERLIGHTNINGATTACKTARGET_H
// NTCScriptedControl::CActionSummonerLightningAttackTarget (new, chunk 765): RTTI .?AVCActionSummonerLightningAttackTarget@
// NTCScriptedControl@@ : CTCScriptedControl::CActionBase; retail vtable 0x0126586C: 0 scalar deleting dtor 0x00719120, 1 Update
// 0x007175D0, 2 OnActivate 0x007122B0, 3 OnAdd 0x007122A0 (ret), 4 GetActionName 0x00718DD0, 5 base OnCleanup. Ego_r.pdb members
// (sizeof 0x38): PTarget (CScriptThing, 0xc bytes, opaque here) +0x0C, StartedAttack +0x18, TurnedToFace +0x19,
// TurnToFaceAction (CActionTurnToFacePosition) +0x1C.
// Includer contract: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE.
#include "engine/NTCScriptedControl_CActionTurnToFacePosition.h"
namespace NTCScriptedControl {
class CActionSummonerLightningAttackTarget : public CTCScriptedControl::CActionBase {
public:
    CActionSummonerLightningAttackTarget(const CThing&);
    virtual ~CActionSummonerLightningAttackTarget();
    virtual void Update();                                               //  1 0x007175D0
    virtual void OnActivate();                                           //  2 0x007122B0
    virtual void OnAdd();                                                //  3 0x007122A0
    virtual CCharString GetActionName() const;                           //  4 0x00718DD0
private:
    unsigned char PTarget[0xc];                                          // +0x0c CScriptThing
    bool StartedAttack;                                                  // +0x18
    bool TurnedToFace;                                                   // +0x19
    CActionTurnToFacePosition TurnToFaceAction;                          // +0x1c
};
}
FABLE_STATIC_ASSERT(sizeof(NTCScriptedControl::CActionSummonerLightningAttackTarget) == 0x38);
#endif
