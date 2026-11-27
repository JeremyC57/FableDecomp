#ifndef FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONDRAINLIFE_H
#define FABLE_ENGINE_NTHINGSHOT_CTCSHOTDEFINITIONDRAINLIFE_H
// NThingShot::CTCShotDefinitionDrainLife (new, chunk Q20): RTTI : NThingShot::CTCShotDefinitionBase at 0. Ego_r.pdb members after the
// base (Ego_r 0xA0, retail 0x9C): MaxCosAngleToTurn +0x9C, MaxAngleToTurn +0xA0, AmountToHeal +0xA4, Speed +0xA8,
// PSoulEffect (CIntelligentPointer<CThing>) +0xAC; Ego_r sizeof 184 = retail 0xB4. InitMovement is the protected virtual
// ?InitMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ (retail 0x007BD845). Needs FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT.
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#ifdef FABLE_RETAIL_DRAIN_PULSE_PASS29
class CMeleeCombatAbilityDef;
namespace NThingShot {
CThing* FABLE_FASTCALL CreateShot(long,CThing*,CThing*,const CThing*,float,float,float,
    const CDefPointer<const CMeleeCombatAbilityDef>&,const C3DVector*,const CRightHandedSet*);
}

#endif
namespace NThingShot {
class CTCShotDefinitionDrainLife : public CTCShotDefinitionBase {
#ifdef FABLE_RETAIL_DRAIN_PULSE_PASS29
public:
    void SetAmountToHeal(float value) { AmountToHeal = value; }
    void SetSoulEffect(long effect);
#endif
protected:
#ifdef FABLE_RETAIL_SHOT_UPDATEMOVEMENT_PASS26
    // Both original PDBs protected virtual MAE; native caller's virtual slot.
    virtual void UpdateMovement();
#endif
    virtual void InitMovement();                                         // 0x007BD845
private:
    float MaxCosAngleToTurn;                                             // +0x9c
    float MaxAngleToTurn;                                                // +0xa0
    float AmountToHeal;                                                  // +0xa4
    float Speed;                                                         // +0xa8
    CIntelligentPointer<CThing> PSoulEffect;                             // +0xac
};
FABLE_STATIC_ASSERT_AT(19, sizeof(CTCShotDefinitionDrainLife)==0xb4);
}
#endif
