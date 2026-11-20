// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::ReadBinary(CDataInputStream& in) -- read the two flags and the offset count, size and fill the offset limit/speed arrays, then either snapshot the current spline as the previous one or clear the previous spline's points
// Leaf: ?ReadBinary@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVCDataInputStream@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B22100, 528 bytes, `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of each inlined Read),
//   CArray<C3DVector>::resize(n, const C3DVector&) 0x0075C040, CDataInputStream::Read3DVector 0x009950F0 (C3DVector by
//   hidden pointer), CArray<C3DVector>::operator= 0x0075B8D0 (inside CCardinalSpline's implicit operator=).
// Layout: as the landed ctor 0x00B21DD0 (retail 12-byte STLport vectors): ThisSpline +4, PreviousSpline +0x18,
//   OffsetOnlyAtStartup +0x2C, ResetEachFrame +0x2D, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3C.
// Shape: inlined ReadBool/ReadULong (the CSplinePosParamData::ReadBinary 0x00B21C90 family), both resizes share one
//   uninitialised C3DVector() slot, the loop stores through `*(begin() + i)` (the array start is loaded before the
//   Read3DVector call), and the else arm is the inline STLport clear() = erase(begin(), end()) whose copy loop runs
//   over the empty [finish, finish) range.
// Naming (NF126, 2026-11-20, q4802): the local CArray<T> (hand-declared operator= / resize, inline erase / clear) and CCardinalSpline
//   models replaced by the shared engine/CCardinalSpline.h (the 0x00B211B0 recipe defines; DeleteAllControlPoints, opt-in
//   FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT): the calls now bind the self rows' STLport spellings vector<C3DVector>::operator=
//   0x0075B8D0 and vector<C3DVector>::resize 0x0075C040.
#include "fable_compiler.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)


#include "engine/C3DVector.h"
class CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14
    C3DVector Read3DVector();                                          // 0x009950F0
    static FABLE_FORCEINLINE bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    FABLE_FORCEINLINE void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    FABLE_FORCEINLINE unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    FABLE_FORCEINLINE bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
#include "engine/CCardinalSpline.h"


namespace NParticleEngine {
class CPSCSplineBase { public:
    class CSplineRandomOffsetData { public:
        CCardinalSpline*  InitialSpline;          // +0x00
        CCardinalSpline   ThisSpline;             // +0x04
        CCardinalSpline   PreviousSpline;         // +0x18
        bool              OffsetOnlyAtStartup;    // +0x2C
        bool              ResetEachFrame;         // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;  // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;   // +0x3C
        void ReadBinary(CDataInputStream& in);
    };
};

void CPSCSplineBase::CSplineRandomOffsetData::ReadBinary(CDataInputStream& in)
{
    OffsetOnlyAtStartup = in.ReadBool();
    ResetEachFrame = in.ReadBool();
    unsigned long count = in.ReadULong();
    SplinePosOffsetLimits.resize(count);
    SplinePosOffsetSpeed.resize(count);
    for (unsigned long i = 0; i < count; ++i) {
        SplinePosOffsetLimits[i] = in.Read3DVector();
        SplinePosOffsetSpeed[i] = in.Read3DVector();
    }
    if (OffsetOnlyAtStartup)
        PreviousSpline = ThisSpline;
    else
        PreviousSpline.DeleteAllControlPoints();
}
}