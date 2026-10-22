#include "engine/CAnimComponentDelayTime.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimComponentDelayTime_Methods : CAnimComponentDelayTime {
    void SetDefaultValue();
};
void CAnimComponentDelayTime_Methods::SetDefaultValue(){ this->Delay = 2u; }