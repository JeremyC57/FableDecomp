#include "engine/CAnimComponentTransitionInTime.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimComponentTransitionInTime_Methods : CAnimComponentTransitionInTime {
    void SetDefaultValue();
};
void CAnimComponentTransitionInTime_Methods::SetDefaultValue(){ this->TransitionInTime = 3u; }