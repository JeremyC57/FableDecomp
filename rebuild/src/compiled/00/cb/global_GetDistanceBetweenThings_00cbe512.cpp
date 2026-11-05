// GetDistanceBetweenThings(const CScriptThing& thing1, const CScriptThing& thing2) -- the distance between the two things' positions
// Leaf: ?GetDistanceBetweenThings@@YIMABVCScriptThing@@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE512, 63 bytes (`ret`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Identity: Ego_r.pdb `?GetDistanceBetweenThings@@YIMABVCScriptThing@@0@Z` (sqrt'd: `fsqrt`). The catalog name
//   "GetSquaredDistanceBetweenThings" is wrong -- the squared variants are the next functions, 0x00CBE583 / 0x00CBE5C0.
// Callees: CScriptThing slot 6 GetPos (+0x18).
// Shape: `(float)sqrt(X*X + Y*Y + Z*Z)` through the sqrt intrinsic (VC7.1 then multiplies z, y, x in that order).
#pragma optimize("s", on)
#include <math.h>
class CCharString {
public:
    void* m_pStringData;
};
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

float __fastcall GetDistanceBetweenThings(const CScriptThing& thing1, const CScriptThing& thing2)
{
    return (thing1.GetPos() - thing2.GetPos()).GetMagnitude();
}