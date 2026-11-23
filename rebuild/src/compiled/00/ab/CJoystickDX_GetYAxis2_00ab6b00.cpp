// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `YAxis` -> `YAxis2` (+0xd24).
// Leaf: ?GetYAxis2@CJoystickDX@@MAEMXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CJoystickDX.h (was ?GetYAxis2@CJoystickDX_Methods@@QAEMXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CJOYSTICKDX_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#define FABLE_RETAIL_CJOYSTICKDX
#define FABLE_RETAIL_CJOYSTICKDX_RTTI_DECLS
#include "engine/CJoystickDX.h"  // retyped onto the PDB layout; byte parity re-verified
float CJoystickDX::GetYAxis2() { return this->YAxis2; }
