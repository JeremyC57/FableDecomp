#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONTURNTOFACEPOSITION_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONTURNTOFACEPOSITION_H
// NTCScriptedControl::CActionTurnToFacePosition (new, chunk 765): RTTI .?AVCActionTurnToFacePosition@NTCScriptedControl@@ :
// CTCScriptedControl::CActionBase; retail vtable 0x01265534: 0 scalar deleting dtor 0x00718630, 1 Update 0x00714CC0, 2 base OnActivate,
// 3 base OnAdd, 4 GetActionName 0x00718610, 5 base OnCleanup. Ego_r.pdb members (sizeof 0x1c): Position +0x0C, RequiredFOV +0x18.
// Includer contract: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE.
#include "engine/CTCScriptedControl.h"
#include "engine/C3DVector.h"
namespace NTCScriptedControl {
class CActionTurnToFacePosition : public CTCScriptedControl::CActionBase {
public:
    virtual ~CActionTurnToFacePosition();
    virtual void Update();                                               //  1 0x00714CC0
    virtual CCharString GetActionName() const;                           //  4 0x00718610
private:
    C3DVector Position;                                                  // +0x0c
    float RequiredFOV;                                                   // +0x18
};
}
FABLE_STATIC_ASSERT(sizeof(NTCScriptedControl::CActionTurnToFacePosition) == 0x1c);
#endif
