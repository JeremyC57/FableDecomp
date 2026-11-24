// NParticleEngine::CPSCSplineBase::CSplinePosParamData::InterpolateTo(const CSplinePosParamData& other, float t) -- blend towards another spline-position parameter set: take the other's MaintainSplineShape past the halfway point, and interpolate the position-parameter array (default value = CRC of "")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B217D0, length 118 bytes (`ret 8`).
// Callees: Calc @ 0x004014A0 (`__fastcall(0, "", 0)`, the CRC), GFInterpolateArray<unsigned long> @ 0x00B22F30 (`__fastcall`, hidden
// result in ecx, first array in edx), STLport vector<unsigned long>::operator= @ 0x0045BC09 (catalog "CActiveFile::operator=", propagated),
// free @ 0x00BFEA14 (the temporary array's inline destructor). Float pool 0x0122F59C = 0.5.
// Layout: the shared engine/CPSCSplineBase.h CSplinePosParamData (both original PDBs; MaintainSplineShape +8, SplinePosParam +0xc).
// Shape: the flag is the conditional `t > 0.5f ? other.flag : flag` (stored back either way); the CRC default is a named local
// (homed in the dead `other` slot and passed by reference); the interpolated array is a by-value temporary assigned then freed.
// Leaf: ?InterpolateTo@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV123@M@Z
// Name fix (NF393, R7 pilot; T3 nf383 0x009B7870): self respelled from ?InterpolateTo@CSplinePosParamData@NParticleEngine@@QAEXABU12@M@Z
//   to the nested class Ego_r.pdb / FableWin.pdb (0x02F5E500) name; sole retail caller CPSCSplineBase::InterpolateTo (call at
//   0x00B22483 inside 0x00B22310). Declared through the new opt-in FABLE_RETAIL_CPSCSPLINEBASE_POSPARAM_INTERPOLATETO.
// Name fix (NF390 / NF393): SplinePosParam is the shared engine/CArray.h CArray<unsigned long> (Lionhead CArray<T> : STLport
//   vector<T>); its generated assignment expands to STLport vector<unsigned long>::operator=, called out of line at 0x0045BC09
//   (FABLE_RETAIL_STLP_VECTOR_ASSIGN), so the call binds ??4?$vector@KV?$allocator@K@std@@@std@@QAEAAV01@ABV01@@Z; the temporary's
//   inline destructor is STLport's ~vector (free). GFInterpolateArray is the NParticleEngine function template (Ego_r.pdb + FableWin.pdb
//   ??$GFInterpolateArray@K@NParticleEngine@@YI?AV?$CArray@K@@ABV1@0MABK@Z; the landed 0x00B22310 declares the same template).
//   CCRC from the shared engine/CCRC.h (declaration-only default).
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSPLINEBASE_POSPARAM_INTERPOLATETO
#include "engine/CArray.h"
#include "engine/CCRC.h"
#include "engine/CPSCSplineBase.h"
namespace NParticleEngine {
template <class T> CArray<T> __fastcall GFInterpolateArray(const CArray<T>& a, const CArray<T>& b, float t, const T& defaultValue);   // 0x00B22F30 (<unsigned long>)
}
void NParticleEngine::CPSCSplineBase::CSplinePosParamData::InterpolateTo(const CSplinePosParamData& other, float t)
{
    MaintainSplineShape = t > 0.5f ? other.MaintainSplineShape : MaintainSplineShape;
    unsigned long defaultValue = CCRC::Calc(0, "", 0);
    SplinePosParam = GFInterpolateArray(SplinePosParam, other.SplinePosParam, t, defaultValue);
}
