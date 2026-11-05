// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `YAxis` -> `YAxis2` (+0xd24).
// Leaf: ?GetYAxis2@CJoystickDX_Methods@@QAEMXZ
#include "engine/CJoystickDX.h"  // retyped onto the PDB layout; byte parity re-verified
struct CJoystickDX_Methods : CJoystickDX {
    float GetYAxis2();
};
float CJoystickDX_Methods::GetYAxis2(){ return this->YAxis2; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetYAxis2@CJoystickDX@@MAEMXZ); evidence: RTTI vtable slot + PDB name set.
