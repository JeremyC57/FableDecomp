#ifndef FABLE_ENGINE_CCREATUREACTION_JUMPOUTFROMCOVER_H
#define FABLE_ENGINE_CCREATUREACTION_JUMPOUTFROMCOVER_H
// CCreatureAction_JumpOutFromCover (new, chunk Q20): RTTI .?AVCCreatureAction_JumpOutFromCover@@ : CCombatActionBase, own vtable
// 0x0129888C (slot 12 Initialise 0x008B7040; Ego_r.pdb / FableWin.pdb ?Initialise@CCreatureAction_JumpOutFromCover@@UAEXXZ). One member
// of its own: Ego_r.pdb JumpDirection (C3DVector, Ego_r +0x12C = retail +0x114: retail CCombatActionBase is 0x114 bytes, the PDB 0x18
// larger), so retail sizeof is 0x120 (Ego_r 312 = 0x138). Includers define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE and
// FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE and include engine/CCombatActionBase.h and engine/C3DVector.h first.
class CCreatureAction_JumpOutFromCover : public CCombatActionBase {
public:
    virtual void Initialise();                                           // 0x008B7040
private:
    C3DVector JumpDirection;                                             // +0x114
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_JumpOutFromCover) == 0x120);
#endif
