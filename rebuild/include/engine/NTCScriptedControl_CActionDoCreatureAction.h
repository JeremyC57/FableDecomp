#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONDOCREATUREACTION_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONDOCREATUREACTION_H
// NTCScriptedControl::CActionDoCreatureAction (new shared header, agentNF79, row 0x00713140 constructor): the scripted-control action that
// hands a creature action to the creature. RTTI .?AVCActionDoCreatureAction@NTCScriptedControl@@ (vtable 0x01265838: 1 Update 0x00712BB0,
// 2 OnActivate 0x00712B60, 4 GetActionName 0x00718DB0; rebuild/manifest/rtti-vtables.tsv) over CTCScriptedControl::CActionBase.
// Ego_r.pdb members (struct_layouts_egor.tsv, sizeof 28): ActionID +0x0C, ActionSet +0x10, Action (CCountedPointer<CCreatureActionBase>)
// +0x14. Methods: Ego_r.pdb + FableWin.pdb ??0CActionDoCreatureAction@NTCScriptedControl@@QAE@ABVCCreatureActionBase@@@Z, ?Update@..@@UAEXXZ,
// ?OnActivate@..@@UAEXXZ, ?GetActionName@..@@UBE?AVCCharString@@XZ; FableWin.pdb ??1..@@UAE@XZ. Declarations only.
// (engine/CActionDoCreatureAction.h is the older generated flat layout of the same class, without its namespace.)
// Includer contract: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE.
#include "engine/CTCScriptedControl.h"
#include "engine/CCountedPointer.h"
class CCharString;
class CCreatureActionBase;
namespace NTCScriptedControl {
class CActionDoCreatureAction : public CTCScriptedControl::CActionBase {
public:
    CActionDoCreatureAction(const CCreatureActionBase& action);         // 0x00713140
    virtual ~CActionDoCreatureAction();
    virtual void Update();                                               //  1 0x00712BB0
    virtual void OnActivate();                                           //  2 0x00712B60
    virtual CCharString GetActionName() const;                           //  4 0x00718DB0
private:
    long ActionID;                                                       // +0x0c
    bool ActionSet;                                                      // +0x10
    CCountedPointer<CCreatureActionBase> Action;                         // +0x14
};
}
FABLE_STATIC_ASSERT(sizeof(NTCScriptedControl::CActionDoCreatureAction) == 0x1c);
#endif
