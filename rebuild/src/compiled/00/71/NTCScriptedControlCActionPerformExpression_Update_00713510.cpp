// NTCScriptedControl::CActionPerformExpression::Update() -- refresh the target script thing, then once look the expression def up by name and set the parent creature performing it at the target (terminate when the def is missing); afterwards terminate once the creature has no action left
// Leaf: ?Update@CActionPerformExpression@NTCScriptedControl@@UAEXXZ
// Name: Ego_r.pdb ?Update@CActionPerformExpression@NTCScriptedControl@@UAEXXZ (0x00727660), slot 1 of the CTCScriptedControl::CActionBase vtable.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00713510, plain `ret`, 0xCC-byte frame.
// Callees: CScriptThing slot 77 IsNull / slot 71 UpdateThingAttachment / slot 11 GetPThing (virtual, through the member's vptr),
//   CGameDefinitionManager::Get @ 0x0044C6B0, ??$GetDef@VCExpressionDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCExpressionDef@@@@@Z
//   @ 0x00593666, ??0CCreatureAction_PerformExpression@@QAE@AAVCThingCreatureBase@@PAVCThing@@ABV?$CDefPointer@$$CBVCExpressionDef@@@@JJJJ@Z
//   @ 0x008430B0, ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z @ 0x006644F0, the temporary's destructor
//   expanded (~CIntelligentPointer @ 0x00A01C10, the def pointer released in place, ~CCreatureActionBase @ 0x00693EF0),
//   ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ @ 0x006622A0 (all Ego_r.pdb / FableWin.pdb spellings).
// Layout: Ego_r.pdb NTCScriptedControl::CActionPerformExpression (40 bytes): CTCScriptedControl::CActionBase +0x00, PTargetThing
//   (CScriptThing) +0x0C, Performed +0x18, Expression +0x1C, ExtraData1 +0x20, ExtraData2 +0x24. No shared header for this class.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CCREATUREACTION_PERFORMEXPRESSION_INLINE_DTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCScriptedControl.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCreatureAction_PerformExpression.h"
inline CThingCreatureBase& CTCScriptedControl::GetParentCreature() { CThingCreatureBase& t=static_cast<CThingCreatureBase&>(GetParentThing()); return t; }
inline CThingCreatureBase& CTCScriptedControl::CActionBase::GetParentCreature() { return PTCScript->GetParentCreature(); }
inline void CTCScriptedControl::CActionBase::Terminate() { Terminated=true; }
namespace NTCScriptedControl {
class CActionPerformExpression : public CTCScriptedControl::CActionBase {
public:
    virtual void Update();                                               //  1 0x00713510
    virtual CCharString GetActionName() const;                           //  4
private:
    CScriptThing PTargetThing;                                           // +0x0c
    bool Performed;                                                      // +0x18
    CCharString Expression;                                              // +0x1c
    long ExtraData1;                                                     // +0x20
    long ExtraData2;                                                     // +0x24
};
void CActionPerformExpression::Update()
{
    if (!PTargetThing.IsNull())
        PTargetThing.UpdateThingAttachment();
    if (!Performed)
    {
        CDefPointer<const CExpressionDef> expression_def;
        CGameDefinitionManager::Get().GetDef(Expression, &expression_def);
        if (!expression_def.IsNull())
        {
            CThingCreatureBase& creature = GetParentCreature();
            creature.SetCurrentAction(CCreatureAction_PerformExpression(creature, PTargetThing.GetPThing(), expression_def, 0, 0, ExtraData1, ExtraData2));
            Performed = true;
        }
        else
            Terminate();
    }
    else if (!GetParentCreature().IsPerformingAnyAction())
        Terminate();
}
}
