// Is2DDistanceBetweenThingsUnder(const CScriptThing& thing1, const CScriptThing& thing2, float distance) -- true when both things are alive and their XY positions are closer than distance
// Leaf: ?Is2DDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE371, 121 bytes (`ret 4`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Identity: Ego_r.pdb `?Is2DDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z`.
// Callees: CScriptThing slots 75 IsAlive (+0x12C), 6 GetPos (+0x18).
// Shape: both positions go through a __forceinline C2DVector(const C3DVector&) (the first one is copied to the frame,
//   hence `push ebp ; mov ebp,esp`), then the squared 2D length against `distance * distance`.
#pragma optimize("s", on)
class CCharString {
public:
    void* m_pStringData;
};
class C3DVector {
public:
    float X, Y, Z;
};
class C2DVector {
public:
    float X, Y;
    __forceinline C2DVector(float x, float y) : X(x), Y(y) { }
    __forceinline C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    __forceinline C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    __forceinline float GetLengthSquared() const { return X * X + Y * Y; }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

bool __fastcall Is2DDistanceBetweenThingsUnder(const CScriptThing& thing1, const CScriptThing& thing2, float distance)
{
    return (thing1.IsAlive() && thing2.IsAlive() && (C2DVector(thing1.GetPos()) - C2DVector(thing2.GetPos())).GetLengthSquared() < distance * distance) ? true : false;
}