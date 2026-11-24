// Leaf: ?GetYAxis@CJoystickDX@@MAEMXZ
// CJoystickDX::GetYAxis() -- return the axis value stored in the retail word +0xd1c (engine/CJoystickDX.h types it as the retail-only long `f_2`; it is a float: retail `fld dword ptr [ecx+0d1ch]`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6AE0, 7 bytes, plain `ret`.
// Layout: the header's long `f_2` at +0xd1c is read through a float view (a long member returned as float would emit fild).
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f_2` -> `YAxis`.
// Header route (NF319, 2026-11-23): defined on the real class of engine/CJoystickDX.h (was ?GetYAxis@CJoystickDX_Methods@@QAEMXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CJOYSTICKDX_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#define FABLE_RETAIL_CJOYSTICKDX
#define FABLE_RETAIL_CJOYSTICKDX_RTTI_DECLS
#include "engine/CJoystickDX.h"
float CJoystickDX::GetYAxis() { return *(const float*)&this->YAxis; }
