// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `XAxis` -> `XAxis2` (+0xd20).
#include "engine/CJoystickDX.h"  // retyped onto the PDB layout; byte parity re-verified
struct CJoystickDX_Methods : CJoystickDX {
    float M();
};
float CJoystickDX_Methods::M(){ return this->XAxis2; }