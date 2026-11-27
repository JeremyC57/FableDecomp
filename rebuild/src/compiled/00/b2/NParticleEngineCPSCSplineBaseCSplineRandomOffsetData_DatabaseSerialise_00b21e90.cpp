// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::DatabaseSerialise(CPersistContext& context, bool) -- persist the two flags, the offset count (the arrays sized to at least the initial spline's point count), each offset limit/speed by indexed name, then snapshot or clear the previous spline
// Leaf: ?DatabaseSerialise@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@3@@Z
// Flags: /d2inlS84
// Inline-budget: /d2inlS84 -- retail expands the vector<C3DVector> erase's random-access __copy (DeleteAllControlPoints, the only __copy site:
//   0x00B220B6; the function calls 0x0074CA20 nowhere) where the default budget calls it; sweep (NF138 / NF144 / NF147): /d2inlS60 DIFFER(582v619),
//   S80 / S82 / S83 DIFFER(573v619), S84 / S85 / S90 / S100 RELOCATION_MATCH, S150 DIFFER(874v619, other sites over-expanded), /d2inlT100 DIFFER(568v619),
//   /d2inlT200 DIFFER(874v619): 84 is the smallest S that matches.
// Naming (NF147, 2026-11-20; q4802): NF126's port onto engine/CCardinalSpline.h (the local CArray<C3DVector> / CCardinalSpline models replaced by the
//   shared STLport vector and CCardinalSpline): 0x0075B8D0 now binds the Ego_r.pdb
//   ??4?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEAAV01@ABV01@@Z and 0x0075C040 its self row's
//   ?resize@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEXIABVC3DVector@@@Z.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B21E90, 624 bytes, `ret 8`.
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer(name, long&, const long&) 0x00410BE0,
//   Transfer(name, C3DVector&, const C3DVector&) 0x004859AD, vector<C3DVector>::resize 0x0075C040,
//   vector<C3DVector>::operator= 0x0075B8D0, GFIntToCharString 0x0099F830 (__fastcall), CCharString(const char*, long)
//   0x0099EBF0, operator+(const CCharString&, const CCharString&) 0x0099F570 (__fastcall), operator const char*
//   0x0099E4C0, ~CCharString 0x0099EAE0.
// Data: names "OffsetOnlyAtStartup" 0x012A0C2C, "ResetEachFrame" 0x012A0C1C, "SplinePosOffsetLimitsSize" 0x012A0C00,
//   "SplinePosOffsetLimit_" 0x012A0BE8, "SplinePosOffsetSpeed_" 0x012A0BD0.
// Layout: as the landed ctor 0x00B21DD0 / ReadBinary 0x00B22100 (retail 12-byte STLport vectors).
// Shape: the sibling of ReadBinary 0x00B22100 (same clear()/snapshot tail); the element names are
//   `CCharString(prefix) + GFIntToCharString(i)` passed through a template __forceinline SaveValue wrapper (the 008006D0
//   idiom: the name chain is built before the value/default pushes), each element goes through `*(begin() + i)` so the
//   array start is read first, and the loop is `if (i < count) { two zero defaults; do ... while (++i < count); }`
//   (the six zero stores sit once inside the guard). The size is `count > size ? count : size` (retail `cmp ; jg`).
#include "fable_compiler.h"

#include "engine/C3DVector.h"
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: bool 0x004045C0; long 0x00410BE0; C3DVector 0x004859AD
};

#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
#include "engine/CCardinalSpline.h"


template <class T>
FABLE_FORCEINLINE void SaveValue(CPersistContext& context, const char* name, T& value, const T& defaultValue)
{
    context.Transfer(name, value, defaultValue);
}

namespace NParticleEngine {
class CEmitterDatabaseSerialiseInfo;   // NF479: the PDB parameter (Ego_r / FableWin ?DatabaseSerialise@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@3@@Z; was bool, unused)
class CPSCSplineBase { public:
    class CSplineRandomOffsetData { public:
        CCardinalSpline*  InitialSpline;          // +0x00
        CCardinalSpline   ThisSpline;             // +0x04
        CCardinalSpline   PreviousSpline;         // +0x18
        bool              OffsetOnlyAtStartup;    // +0x2C
        bool              ResetEachFrame;         // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;  // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;   // +0x3C
        void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
    };
};

void CPSCSplineBase::CSplineRandomOffsetData::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    context.Transfer("OffsetOnlyAtStartup", OffsetOnlyAtStartup, false);
    context.Transfer("ResetEachFrame", ResetEachFrame, false);
    long count = SplinePosOffsetLimits.size();
    context.Transfer("SplinePosOffsetLimitsSize", count, 0L);
    SplinePosOffsetLimits.resize(count > InitialSpline->GetControlPointCount()
        ? count : InitialSpline->GetControlPointCount());
    SplinePosOffsetSpeed.resize(SplinePosOffsetLimits.size());
    long i = 0;
    if (i < count) {
        C3DVector zeroLimit(0.0f, 0.0f, 0.0f);
        C3DVector zeroSpeed(0.0f, 0.0f, 0.0f);
        do {
            SaveValue(context, CCharString("SplinePosOffsetLimit_") + GFIntToCharString(i), SplinePosOffsetLimits[i], zeroLimit);
            SaveValue(context, CCharString("SplinePosOffsetSpeed_") + GFIntToCharString(i), SplinePosOffsetSpeed[i], zeroSpeed);
        } while (++i < count);
    }
    if (OffsetOnlyAtStartup)
        PreviousSpline = ThisSpline;
    else
        PreviousSpline.DeleteAllControlPoints();
}
}