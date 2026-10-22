#include "engine/CAnimComponentCombatRecoil.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimComponentCombatRecoil_Methods : CAnimComponentCombatRecoil {
    void SetDefaultValue();
};
void CAnimComponentCombatRecoil_Methods::SetDefaultValue(){ this->RecoilAnimIndex = 0xffffffffu; }