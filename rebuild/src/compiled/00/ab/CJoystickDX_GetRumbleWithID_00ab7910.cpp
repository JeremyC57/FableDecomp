// Leaf: ?GetRumbleWithID@CJoystickDX@@UAEPAVCActiveRumble@CJoystick@@J@Z
// Header route (NF323, 2026-11-24; NF320 Q3): defined on the real class of engine/CJoystickDX.h (was ?GetRumbleWithID@CJoystickDX_Methods@@QAEHH@Z); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address (+ FableWin.pdb); declared by the add-only opt-in
//   FABLE_RETAIL_CJOYSTICKDX_RUMBLE_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#define FABLE_RETAIL_CJOYSTICKDX
#define FABLE_RETAIL_CJOYSTICKDX_RUMBLE_DECLS
#include "engine/CJoystickDX.h"
CJoystick::CActiveRumble* CJoystickDX::GetRumbleWithID(long a0) { return 0; }
