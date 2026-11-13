#ifndef FABLE_ENGINE_CTCCREATUREEXPRESSION_H
#define FABLE_ENGINE_CTCCREATUREEXPRESSION_H
// New shared header (w22 agent2007, row 0x00665860 CThingCreatureBase::UpdateAnimation): the creature facial-expression component,
// interface TCI_CREATURE_EXPRESSION (12: UpdateAnimation tests bit 12 of +0x20 and looks up key 0x0C). RTTI .?AVCTCCreatureExpression@@ :
// CTCBase. Methods: Ego_r.pdb / FableWin.pdb ?Update@CTCCreatureExpression@@QAEXAAVCAnimateThing@@ABVCAnimateThingUpdate@@@Z (out of
// line in retail at 0x00729200). Members are not modelled (the row only calls Update). Needs FABLE_RETAIL_CTCBASE.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CAnimateThing;
class CAnimateThingUpdate;
class CTCCreatureExpression : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CREATURE_EXPRESSION; }
    void Update(CAnimateThing& animation, const CAnimateThingUpdate& update);   // 0x00729200
};
#endif
