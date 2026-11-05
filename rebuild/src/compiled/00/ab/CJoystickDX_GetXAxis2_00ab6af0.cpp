// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `XAxis` -> `XAxis2` (+0xd20).
// Leaf: ?GetXAxis2@CJoystickDX_Methods@@QAEMXZ
#include "engine/CJoystickDX.h"  // retyped onto the PDB layout; byte parity re-verified
struct CJoystickDX_Methods : CJoystickDX {
    float GetXAxis2();
};
float CJoystickDX_Methods::GetXAxis2(){ return this->XAxis2; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetXAxis2@CJoystickDX@@MAEMXZ); evidence: RTTI vtable slot + PDB name set.
