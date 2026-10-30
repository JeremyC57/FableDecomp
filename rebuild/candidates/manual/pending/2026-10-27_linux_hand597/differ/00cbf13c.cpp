// GetBestThingFromMarkerList(const vector<CScriptThing>& markers, const CScriptThing& target, const CScriptThing& origin, bool preferLargest) -- of the markers, return the one whose flat (XY) direction from origin best lines up with origin's flat direction to target (largest dot product when preferLargest, else the smallest); a null thing when target or origin is dead
// Grade: DIFFER(355v355) -- same size, every instruction and frame slot match except one callee-saved register tie: retail keeps target in edi (reused for the loop's origin position) and origin in ebx, this build the reverse (16 instructions differ by register name only). 3 grades + ~40 compile-only probes (alive-test forms, named bool / helper, aliases of origin, marker reference, iterator typedef, /O1 /Oy-, /Os, /G6) never flipped it. Next lever: a use-count change on the target web (GOTCHAS 'callee-saved ties follow use weight')
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?GetBestThingFromMarkerList@@YI?AVCScriptThing@@ABV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@ABV1@1_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF13C, 355 bytes, `__fastcall` global returning CScriptThing by value
//   (hidden result pointer in ecx, markers in edx, three stack arguments: `ret 0Ch`); frame-pointer size TU like its
//   landed neighbours CutsceneMacroReadLightScene 0x00CBF050 and MoveAllThingsInVectorToPosition 0x00CBF679. The start is
//   real: 0x00CBF139 is the `ret 0Ch` of the previous function, then `push ebp ; mov ebp,esp`.
// Identity (proven): Ego_r.pdb (donor 0x00CA0632) and FableWin.pdb (0x035A0FDB) both carry this mangled name, the only
//   script-library global returning CScriptThing from (vector<CScriptThing>, CScriptThing, CScriptThing, bool), which is
//   exactly retail's argument shape (12-byte CScriptThing elements, two CScriptThing references, a byte flag). Only caller
//   0x00CC9BE2. The catalog / autoname label FindNearestEntity_ByDirection is descriptive and wrong.
//   Parameter names are descriptive (the PDB name sets carry no argument names).
// Callees: CScriptThing slot 75 IsAlive (+0x12c), slot 6 GetPos (+0x18), ctor 0x006E7B40, copy ctor 0x004ABE90,
//   operator= 0x008AB980, dtor 0x004AA840 (engine/CScriptThing.h, Ego_r + FableWin spellings);
//   C3DVector::AccurateNormalise 0x00A14440 (?AccurateNormalise@C3DVector@@QAEAAV1@XZ). C3DVector operator-, Dot,
//   GetSquaredMagnitude and Normalise are the Ego_r.pdb members, expanded inline in retail (no calls).
// Data: 0x0129BA3C 1e-4f, an extern const float (no PDB name; spelled as GOTCHAS chunk 200).
// Shape: `to.GetPos() - origin.GetPos()` evaluates the argument (origin) first; the Z difference is dead under the
//   `Z = 0` store, and the inline Normalise tests the squared magnitude against epsilon squared before AccurateNormalise.
//   The member Dot gives retail's Y, X, Z term order (a free Dot gives X, Y, Z under every association). The best dot
//   (-2.0f, homed in the dead target argument slot) is declared before the best thing, so its store precedes the ctor
//   call; no NRVO (the best local is copied out).
extern const float g_epsilon_0129ba3c;                    // 0x0129BA3C (1e-4f)
class C3DVector { public:
    float X, Y, Z;
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline const C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    __forceinline float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
    __forceinline const float Dot(const C3DVector& other) const { return X * other.X + Y * other.Y + Z * other.Z; }
    C3DVector& AccurateNormalise();                       // 0x00A14440
    __forceinline C3DVector& Normalise()
    {
        if (GetSquaredMagnitude() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            AccurateNormalise();
        return *this;
    }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
};
}


CScriptThing __fastcall GetBestThingFromMarkerList(const std::vector<CScriptThing>& markers, const CScriptThing& target,
                                                   const CScriptThing& origin, bool preferLargest)
{
    if (!target.IsAlive() || !origin.IsAlive())
        return CScriptThing();
    C3DVector direction = target.GetPos() - origin.GetPos();
    direction.Z = 0.0f;
    direction.Normalise();
    float bestDot = -2.0f;
    CScriptThing best;
    for (const CScriptThing* it = markers.begin(); it != markers.end(); ++it) {
        C3DVector markerDirection = it->GetPos() - origin.GetPos();
        markerDirection.Z = 0.0f;
        markerDirection.Normalise();
        float dot = markerDirection.Dot(direction);
        if (preferLargest ? dot > bestDot : dot < bestDot) {
            bestDot = dot;
            best = *it;
        }
    }
    return best;
}
