#ifndef FABLE_ENGINE_CCREATUREACTION_GETUPFROMSITTING_H
#define FABLE_ENGINE_CCREATUREACTION_GETUPFROMSITTING_H
// CCreatureAction_GetUpFromSitting (new shared header, chunk 1010b, row 0x008A8C60 CGameScriptInterface::EntitySetAsSittingOnFloor):
// the creature action that plays a sitting creature's stand-up animation. RTTI .?AVCCreatureAction_GetUpFromSitting@@ :
// CCombatActionBase : CCreatureActionBase : CBaseObject : CBaseClass (ghidra_out/rtti_bases.tsv). Retail vtable slots
// (rebuild/manifest/rtti-vtables.tsv): 0 scalar deleting dtor 0x008601F0, 3 PerformAction 0x008B79A0, 11 Clone 0x00860290, 12
// 0x008B7920, 49/50/59/68 further overrides. Ctor Ego_r.pdb / FableWin.pdb
// ??0CCreatureAction_GetUpFromSitting@@QAE@AAVCThingCreatureBase@@ABVCCharString@@J_N@Z (retail 0x00860150, `ret 10h`). The dtor
// is the implicit one: 0x008A8E1D calls ~CCombatActionBase 0x0062E900 straight on the temporary.
// Members (Ego_r.pdb SittingMode +0x12C / RemoveOnPerformAction +0x130 on the 0x12C Ego_r base) sit on the retail 0x114
// CCombatActionBase at +0x114 / +0x118; size 0x11C (the temporary at [esp+20h] fills 0x008A8C60's 0x12C frame).
#include "rebuild_abi.h"
#include "engine/CCombatActionBase.h"
class CThingCreatureBase;
class CCharString;
class CCreatureAction_GetUpFromSitting : public CCombatActionBase {
public:
    CCreatureAction_GetUpFromSitting(CThingCreatureBase& creature, const CCharString& animation, long sittingMode,
                                     bool removeOnPerformAction);        // 0x00860150
    virtual void PerformAction();                                        // slot 3  0x008B79A0
protected:
    virtual CCreatureActionBase* Clone() const;                          // slot 11 0x00860290
private:
    long SittingMode;                                                    // +0x114
    bool RemoveOnPerformAction;                                          // +0x118
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_GetUpFromSitting) == 0x11c);
#endif // FABLE_ENGINE_CCREATUREACTION_GETUPFROMSITTING_H
