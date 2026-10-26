// NParticleEngine::CSplinePosParamData::InterpolateTo(const CSplinePosParamData& other, float t) -- blend towards another spline-position parameter set: take the other's MaintainSplineShape past the halfway point, and interpolate the position-parameter array (default value = CRC of "")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B217D0, length 118 bytes (`ret 8`).
// Callees: Calc @ 0x004014A0 (`__fastcall(0, "", 0)`, the CRC), GFInterpolateArray<unsigned long> @ 0x00B22F30 (`__fastcall`, hidden
// result in ecx, first array in edx), CArray<unsigned long>::operator= @ 0x0045BC09 (catalog "CActiveFile::operator=", propagated),
// free @ 0x00BFEA14 (the temporary array's inline destructor). Float pool 0x0122F59C = 0.5.
// Layout: Ego_r.pdb CSplinePosParamData (MaintainSplineShape +8, SplinePosParam +0xc) as the landed 0x00B20B10 / 0x00B21940 rows.
// Shape: the flag is the conditional `t > 0.5f ? other.flag : flag` (stored back either way); the CRC default is a named local
// (homed in the dead `other` slot and passed by reference); the interpolated array is a by-value temporary assigned then freed.
// Leaf: InterpolateTo
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

namespace NParticleEngine {
class CParticleSystem;
struct CCardinalSpline;

template<class T> struct CArray {                    // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    ~CArray() { if (m_pData) free(m_pData); }
    CArray& operator=(const CArray& other);          // 0x0045BC09 (CArray<unsigned long>)
};

CArray<unsigned long> __fastcall GFInterpolateArray(const CArray<unsigned long>& a, const CArray<unsigned long>& b,
    float t, const unsigned long& defaultValue);     // 0x00B22F30

struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;    // +0x00
    CCardinalSpline*      Spline;                   // +0x04
    bool                  MaintainSplineShape;      // +0x08
    CArray<unsigned long> SplinePosParam;           // +0x0c
    void InterpolateTo(const CSplinePosParamData& other, float t);
};

void CSplinePosParamData::InterpolateTo(const CSplinePosParamData& other, float t)
{
    MaintainSplineShape = t > 0.5f ? other.MaintainSplineShape : MaintainSplineShape;
    unsigned long defaultValue = CCRC::Calc(0, "", 0);
    SplinePosParam = GFInterpolateArray(SplinePosParam, other.SplinePosParam, t, defaultValue);
}

}