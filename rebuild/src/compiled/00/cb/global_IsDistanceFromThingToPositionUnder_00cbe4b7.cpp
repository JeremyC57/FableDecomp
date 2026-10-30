// IsDistanceFromThingToPositionUnder(const CScriptThing& thing, const C3DVector& position, float distance) -- true when the thing is alive and closer than distance to the position
// Leaf: ?IsDistanceFromThingToPositionUnder@@YI_NABVCScriptThing@@ABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE4B7, 91 bytes (`ret 4`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Identity: Ego_r.pdb `?IsDistanceFromThingToPositionUnder@@YI_NABVCScriptThing@@ABVC3DVector@@M@Z`.
// Callees: CScriptThing slots 75 IsAlive (+0x12C), 6 GetPos (+0x18).
// Shape: 0x00CBE2FF with one thing and a position reference.
#pragma optimize("s", on)
class CCharString {
public:
    void* m_pStringData;
};
class C3DVector {
public:
    float X, Y, Z;
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

bool __fastcall IsDistanceFromThingToPositionUnder(const CScriptThing& thing, const C3DVector& position, float distance)
{
    return (thing.IsAlive() && (thing.GetPos() - position).GetLengthSquared() < distance * distance) ? true : false;
}