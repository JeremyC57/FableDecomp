// NParticleEngine::CPSCSplineBase::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend this spline component towards another: lerp the tension, interpolate the control points (GFInterpolateArray over copies of both point arrays), and blend the random-offset and position-parameter data (interpolated when both exist; otherwise past the halfway point the own data is released or a copy of the other's is created)
// Leaf: ?InterpolateTo@CPSCSplineBase@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 35c (caller respell): calls 0x00B15BD0 as ??$GFInterpolateArray@VC3DVector@@@NParticleEngine@@YI?AV?$CArray@VC3DVector@@@@ABV1@0MABVC3DVector@@@Z (Ego_r.pdb + FableWin.pdb; was the global template).
// Name: FableWin.pdb / Ego_r.pdb ?InterpolateTo@CPSCSplineBase@NParticleEngine@@UAEXABVCPSCBase@2@M@Z; slot 14 of the CPSCSplineBase
//   vtable 0x012A0B14 (rtti-vtables.tsv); called non-virtually with `const CPSCBase&` by 0x00B062A0 / 0x00B1AF50.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B22310, 537 bytes (+ int3 padding), `ret 8` (virtual).
// Callees: CArray<C3DVector> copy 0x006F0810 / operator= 0x0075B8D0, GFInterpolateArray<C3DVector> 0x00B15BD0 (__fastcall,
//   hidden result in ecx, first array in edx), free 0x00BFEA14, operator new 0x00BFEA1A / delete 0x00BFE9BC,
//   CSplineRandomOffsetData(CCardinalSpline*) 0x00B21DD0 / InterpolateTo 0x00B211B0 / operator= 0x00B20B60,
//   CSplinePosParamData(CCardinalSpline*) 0x00B21A00 / InterpolateTo 0x00B217D0, CArray<unsigned long>::operator=
//   0x0045BC09, CCountedPointer<..>::Reset(T*) 0x00B23240 / 0x00B23170. Floats 1.0 0x0122DED8, 0.5 0x0122F59C.
// Layout: the landed CPSCSplineBase::DatabaseCopy 0x00B22530 model (Spline +0x10, Tension +0x1c, SplineRandomOffsetData
//   +0x24, SplinePosParamData +0x2c).
// Shape: the point arrays reach GFInterpolateArray as by-value copies (an inline GetControlPoints), the zero vector is the
//   default-value argument; the random-offset branch tests own data before t, the position-parameter branch t first.
// Naming (NF126, 2026-11-20, q4802): the local CArray<T> (hand-declared copy ctor / operator=) and CCardinalSpline models replaced by the
//   shared engine/CCardinalSpline.h (the 0x00B211B0 recipe defines; Get/SetTension, Get/SetControlPoints): the calls now bind the self rows'
//   STLport spellings vector<C3DVector> copy 0x006F0810 / operator= 0x0075B8D0 and vector<unsigned long>::operator= 0x0045BC09.
#include "fable_compiler.h"
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#include "engine/CCardinalSpline.h"
namespace NParticleEngine { template <class T> CArray<T> __fastcall GFInterpolateArray(const CArray<T>& a, const CArray<T>& b, float t, const T& defaultValue); }
struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> class CCountedPointer { public:
    T*           Data;
    SCountBlock* Info;
    void Reset(T* object);                             // 0x00B23240 / 0x00B23170
    T* operator->() const { return Data; }
    FABLE_FORCEINLINE void Clear()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
};
namespace NParticleEngine {
class CParticleSystem;
class CPSCBase { public:
    virtual void v00();
    CParticleSystem* ParticleSystem;                   // +0x04
    bool UpdateStarted : 1;                            // +0x08
    bool Initialised : 1;
};
class CPSCSplineBase : public CPSCBase { public:
    class CSplineRandomOffsetData { public:                   // 0x48 bytes
        unsigned char _body[0x48];
        CSplineRandomOffsetData(CCardinalSpline* initialSpline);   // 0x00B21DD0
        void InterpolateTo(const CSplineRandomOffsetData& other, float t);   // 0x00B211B0
        CSplineRandomOffsetData& operator=(const CSplineRandomOffsetData& other);   // 0x00B20B60
    };
    class CSplinePosParamData { public:                       // 0x18 bytes
        CParticleSystem*      PParentParticleSystem;   // +0x00
        CCardinalSpline*      Spline;                  // +0x04
        bool                  MaintainSplineShape;     // +0x08
        CArray<unsigned long> SplinePosParam;          // +0x0c
        CSplinePosParamData(CCardinalSpline* spline);  // 0x00B21A00
        void InterpolateTo(const CSplinePosParamData& other, float t);   // 0x00B217D0
        CSplinePosParamData& operator=(const CSplinePosParamData& other)
        {
            MaintainSplineShape = other.MaintainSplineShape;
            SplinePosParam = other.SplinePosParam;
            return *this;
        }
    };
    float                                    SplineAnimSpeed;          // +0x0c
    CCardinalSpline                          Spline;                   // +0x10
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData>     SplinePosParamData;       // +0x2c
    virtual void InterpolateTo(const CPSCBase& other, float t);
};

void CPSCSplineBase::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCSplineBase* source = static_cast<const CPSCSplineBase*>(&other);
    Spline.SetTension((1.0f - t) * Spline.GetTension() + t * source->Spline.GetTension());
    Spline.SetControlPoints(NParticleEngine::GFInterpolateArray(Spline.GetControlPoints(), source->Spline.GetControlPoints(), t, C3DVector(0.0f, 0.0f, 0.0f)));

    if (SplineRandomOffsetData.Data && source->SplineRandomOffsetData.Data)
        SplineRandomOffsetData->InterpolateTo(*source->SplineRandomOffsetData.Data, t);
    else if (SplineRandomOffsetData.Data) {
        if (t > 0.5f)
            SplineRandomOffsetData.Clear();
    } else if (source->SplineRandomOffsetData.Data) {
        if (t > 0.5f) {
            SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&Spline));
            *SplineRandomOffsetData.Data = *source->SplineRandomOffsetData.Data;
        }
    }

    if (SplinePosParamData.Data && source->SplinePosParamData.Data)
        SplinePosParamData->InterpolateTo(*source->SplinePosParamData.Data, t);
    else if (t > 0.5f) {
        if (SplinePosParamData.Data)
            SplinePosParamData.Clear();
        else if (source->SplinePosParamData.Data) {
            SplinePosParamData.Reset(new CSplinePosParamData(&Spline));
            *SplinePosParamData.Data = *source->SplinePosParamData.Data;
        }
    }
}
}