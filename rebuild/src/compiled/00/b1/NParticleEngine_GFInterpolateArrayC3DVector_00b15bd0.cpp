// NParticleEngine::GFInterpolateArray(const CArray<C3DVector>& from, const CArray<C3DVector>& to, float t, const C3DVector& defaultValue) -- blend two point arrays: the result has round(from.size()*(1-t) + to.size()*t) points, each the lerp of the matching points, a missing one replaced by defaultValue
// Leaf: ??$GFInterpolateArray@VC3DVector@@@NParticleEngine@@YI?AV?$CArray@VC3DVector@@@@ABV1@0MABVC3DVector@@@Z
// Naming wave 35c: the exact PDB symbol (Ego_r.pdb + FableWin.pdb); the catalog carries the name; body unchanged.
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B15BD0, 1361 bytes, __fastcall (hidden result in ecx, from in edx), `ret 0Ch`.
// Identity: FableWin.pdb and Ego_r.pdb ??$GFInterpolateArray@VC3DVector@@@NParticleEngine@@YI?AV?$CArray@VC3DVector@@@@ABV1@0MABVC3DVector@@@Z
//   (the template instance, in namespace NParticleEngine beside the GFInterpolate overloads); the catalog's "InterpolateAnimationSequence"
//   (_global) is propagated, and the landed caller 0x00B22310 binds it without the namespace (NAME_FIXES.tsv).
// Retail-asm: 0x00B15C37 `fstp dword ptr [esp+1Ch]` + 0x00B15C3B `fld` + 0x00B15C3F `fistp dword ptr [esp+14h]` -- the point count is
//   rounded by the engine's inline-asm float-to-long, the shared engine/GFFloatToLongNear.h (VC7.1 `(long)x` calls __ftol2,
//   /QIfist gives fistp qword); this row carries no asm of its own.
// Callees: STLport vector<C3DVector>(size_type) 0x007C2AB0 (CArray<C3DVector>(n), engine/CArray.h FABLE_RETAIL_CARRAY), the vector
//   copy constructor 0x006F0810 (the by-value return: VC7.1 has no NRVO), free 0x00BFEA14 (the local's storage).
// Inlined: GFInterpolate(const C3DVector&, const C3DVector&, float) (FableWin ?GFInterpolate@NParticleEngine@@YI?AVC3DVector@@ABV2@0M@Z,
//   __forceinline as in the landed InterpolateTo rows): a * (1 - t) + b * t; VC7.1 unrolls the point loop four times (as retail).
// Shape: the sizes are signed longs (`fild` with no unsigned fix-up); the two copied endpoints come from an if / else-if chain that
//   tests i >= from.size() first and only then i >= to.size() (the third arm reads both arrays), as retail's three paths show.
#include "fable_compiler.h"
#define FABLE_RETAIL_CARRAY
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongNear.h"
namespace NParticleEngine {
FABLE_FORCEINLINE C3DVector __fastcall GFInterpolate(const C3DVector& a, const C3DVector& b, float t) { return a * (1.0f - t) + b * t; }
template <class T> CArray<T> __fastcall GFInterpolateArray(const CArray<T>& from, const CArray<T>& to, float t, const T& defaultValue)
{
    long fromSize = from.size();
    long toSize = to.size();
    long count = GFFloatToLongNear(fromSize * (1.0f - t) + toSize * t);
    CArray<T> result(count);
    for (long i = 0; i < count; ++i) {
        T a, b;
        if (i >= fromSize) {
            a = defaultValue;
            b = to[i];
        } else if (i >= toSize) {
            a = from[i];
            b = defaultValue;
        } else {
            a = from[i];
            b = to[i];
        }
        result[i] = GFInterpolate(a, b, t);
    }
    return result;
}
template CArray<C3DVector> __fastcall GFInterpolateArray<C3DVector>(const CArray<C3DVector>&, const CArray<C3DVector>&, float, const C3DVector&);
}