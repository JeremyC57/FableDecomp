// Leaf: ?GetRumbleWithID@CJoystickDX_Methods@@QAEHH@Z
#include "engine/CJoystickDX.h"  // retyped onto the PDB layout; byte parity re-verified
struct CJoystickDX_Methods : CJoystickDX {
    int GetRumbleWithID(int a0);
};
int CJoystickDX_Methods::GetRumbleWithID(int a0) { return 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetRumbleWithID@CJoystickDX@@UAEPAVCActiveRumble@CJoystick@@J@Z); evidence: RTTI vtable slot + PDB name set.
