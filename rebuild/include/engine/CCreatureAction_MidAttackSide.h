#ifndef FABLE_ENGINE_CCREATUREACTION_MIDATTACKSIDE_H
#define FABLE_ENGINE_CCREATUREACTION_MIDATTACKSIDE_H
// CCreatureAction_MidAttackSide (new, chunk Q12): RTTI .?AVCCreatureAction_MidAttackSide@@ : CCombatActionBase (ghidra_out/rtti_bases.tsv),
// no members of its own (Ego_r.pdb 300 bytes = CCombatActionBase; retail 0x114). Initialise is vtable slot 0x0127AB6C's entry
// 0x008B6AD0 (Ego_r.pdb / FableWin.pdb ?Initialise@CCreatureAction_MidAttackSide@@UAEXXZ). Includers define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
// and FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE and include engine/CCombatActionBase.h first.
class CCreatureAction_MidAttackSide : public CCombatActionBase {
public:
    virtual void Initialise();                                           // 0x008B6AD0
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_MidAttackSide) == 0x114);
#endif
