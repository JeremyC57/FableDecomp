// NParticleEngine::CPSCSplineBase::CSplinePosParamData::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- persist MaintainSplineShape; when loading, read the parameter count, size SplinePosParam (at least the spline's point count, filled with the CRC of an empty name), load each indexed parameter name as its CRC, and clear the array if every CRC is zero
// Leaf: ?DatabaseSerialise@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B21A50, 565 bytes, thiscall `ret 8`.
// Name: Ego_r.pdb 0x00AE2010 / FableWin.pdb 0x02F5DD20 (the second parameter is NParticleEngine::CEmitterDatabaseSerialiseInfo, unused).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0, Transfer<CCharString> 0x004109A0,
//   CCRC::Calc 0x004014A0 (__fastcall), vector<unsigned long>::resize 0x00464931, CCharString() 0x0099E4B0, CCharString(const
//   CCharString&) 0x0099EC30, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, GFIntToCharString 0x0099F830,
//   operator+(const CCharString&, const CCharString&) 0x0099F570, operator const char* 0x0099E4C0, memmove 0x00BFEAE6 (clear()).
// Data: "MaintainSplineShape" 0x012A0BBC, "SplinePosParamSize" 0x012A0BA8, "SplinePosParamSize_" 0x012A0B94, NullChar 0x0129AAF4,
//   CRC-32 table 0x0129A168.
// Layout: Ego_r.pdb CSplinePosParamData (Spline +0x4, MaintainSplineShape +0x8, SplinePosParam +0xC; retail 12-byte STLport
//   vectors), CCardinalSpline (ControlPoints +0x0, Tension, EndPointsConnected), as the landed ReadBinary 0x00B21C90.
// Shape: retail is link-time code generated, so this TU sees the bodies of CCharString::GetCRC (0x00404310), CCRC::Calc and
//   STLport's resize / _M_fill_insert (stl/_vector.c, included by _vector.h). The inliner then expands GetCRC at both sites,
//   calls Calc out of line for the empty default (0x00B21AD0) and expands it in the load loop (0x00B21BEB); and since resize
//   (kept out of line, 0x00B21B1D) is seen never to keep its `const unsigned long&` argument, the CRC temporary is not exposed:
//   the point-count load `mov edx,[ecx+4]` (0x00B21AD8) schedules ahead of the temporary's store (0x00B21ADB). With resize
//   declaration-only the store comes first and the start pointer is loaded separately (12 lines). GetAsCharArray is the
//   early-return spelling (FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN): the expanded Calc then indexes the text as
//   [ecx+edi] (0x00B21BF1); the ternary gives [edi+ecx]. The names go through the two-argument Transfer (GetDefaultVal<CCharString>
//   builds a local and copies it out, 0x00B21B7D..0x00B21B94).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN
#define FABLE_RETAIL_CCRC_CALC_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"

CCharString FABLE_FASTCALL GFIntToCharString(long value);                            // 0x0099F830

// GetDefaultVal<CCharString> (Ego_r.pdb ??$GetDefaultVal@VCCharString@@@CPersistContext@@KI?AVCCharString@@XZ), inline in retail.
template <> inline CCharString FABLE_FASTCALL CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }

class CCardinalSpline {
public:
    CArray<C3DVector> ControlPoints;                // +0x00
    float             Tension;                      // +0x0C
    bool              EndPointsConnected;           // +0x10
};
class CParticleSystem;

namespace NParticleEngine {
class CEmitterDatabaseSerialiseInfo;
class CPSCSplineBase {
public:
    class CSplinePosParamData {
    public:
        CParticleSystem*      PParentParticleSystem;    // +0x00
        CCardinalSpline*      Spline;                   // +0x04
        bool                  MaintainSplineShape;      // +0x08
        CArray<unsigned long> SplinePosParam;           // +0x0C
        void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
    };
};

void CPSCSplineBase::CSplinePosParamData::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    context.Transfer("MaintainSplineShape", MaintainSplineShape, false);
    if (context.IsSaving())
        return;
    long count;
    context.Transfer("SplinePosParamSize", count, 0L);
    SplinePosParam.resize(count > (long)Spline->ControlPoints.size() ? count : Spline->ControlPoints.size(),
        CCharString().GetCRC());
    bool anySet = false;
    for (long i = 0; i < count; ++i) {
        CCharString name;
        context.Transfer(CCharString("SplinePosParamSize_") + GFIntToCharString(i), name);
        SplinePosParam[i] = name.GetCRC();
        anySet |= SplinePosParam[i] != 0;
    }
    if (!anySet)
        SplinePosParam.clear();
}
}