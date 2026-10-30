// GetDistanceBetweenThings(const CScriptThing& thing1, const CScriptThing& thing2) -- the distance between the two things' positions
// Leaf: ?GetDistanceBetweenThings@@YIMABVCScriptThing@@0@Z
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
class C3DVector {
public:
    float X, Y, Z;
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

float __fastcall GetDistanceBetweenThings(const CScriptThing& thing1, const CScriptThing& thing2)
{
    return (thing1.GetPos() - thing2.GetPos()).GetLength();
}