// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `YAxis` -> `YAxis2` (+0xd24).
#include "engine/CJoystickDX.h"  // retyped onto the PDB layout; byte parity re-verified
struct CJoystickDX_Methods : CJoystickDX {
    float M();
};
float CJoystickDX_Methods::M(){ return this->YAxis2; }