// GetDistanceBetweenThingAndPosition(const CScriptThing& thing, const C3DVector& position) -- the distance from the thing's position to the given position
// Leaf: ?GetDistanceBetweenThingAndPosition@@YIMABVCScriptThing@@ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE551, 50 bytes (`ret`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Identity: Ego_r.pdb `?GetDistanceBetweenThingAndPosition@@YIMABVCScriptThing@@ABVC3DVector@@@Z`. The manifest label
//   "DistanceCalculator_GetVectorDistance" is propagated and wrong.
// Callees: CScriptThing slot 6 GetPos (+0x18).
// Shape: as 0x00CBE512 with a position reference.
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

float __fastcall GetDistanceBetweenThingAndPosition(const CScriptThing& thing, const C3DVector& position)
{
    return (thing.GetPos() - position).GetLength();
}