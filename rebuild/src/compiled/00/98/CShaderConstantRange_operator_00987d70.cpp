// CShaderConstantRange::operator!=(const CShaderConstantRange& other) const -- true unless Offset (+4) and Count (+8) both match (the vptr
//   at +0 is not compared)
// Leaf: ??9CShaderConstantRange@@QBE_NABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987D70, length 33 bytes. No callees. Shape: no register saves, the `== && ==` chain falls
//   through to `xor eax,eax`, `ret 4`. The `||` return gives the full-width `xor eax,eax` / `mov eax,1` (the `if (== && ==) return
//   false; return true;` form gives `xor al,al` / `mov al,1`, DIFFER 30v33).
// Names (NF201, 2026-11-20): FableWin.pdb 0x03031230 ??9CShaderConstantRange@@QBE_NABV0@@Z, declared in engine/CShaderConstantRange.h
//   (Ego_r.pdb class CShaderConstantRange : CBaseClass, Offset +4 / Count +8); its caller 0x0098B5E0 passes &layout->LightGlobals (a
//   CShaderConstantRange) and already binds this spelling. Was the placeholder CPointerPair_00987d70::operator!= returning long.
#include "engine/CShaderConstantRange.h"

bool CShaderConstantRange::operator!=(const CShaderConstantRange& other) const
{
    return Offset != other.Offset || Count != other.Count;
}
