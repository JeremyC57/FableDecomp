// NParticleEngine::CPSCSplineBase::ReadBinary(CDataInputStream& in) -- load a spline component: the control-point count and points into the cardinal spline, then the optional random-offset data and the optional position-parameter data (each re-created bound to the spline and read by itself, or left released)
// Leaf: ?ReadBinary@CPSCSplineBase@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Name: FableWin.pdb / Ego_r.pdb ?ReadBinary@CPSCSplineBase@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of the CPSCSplineBase
//   vtable 0x012A0B14 (rtti-vtables.tsv); called non-virtually by 0x00B068F0 / 0x00B1B1B0. The two nested readers are the FableWin
//   ?ReadBinary@CSplineRandomOffsetData@...@QAEXAAVCDataInputStream@@@Z / ?ReadBinary@CSplinePosParamData@...@QAEXAAVCDataInputStream@@@Z
//   (reference, as their landed definitions 0x00B22100 / 0x00B21C90 spell them).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B22B40, 594 bytes, `ret 4` (virtual).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inlined Read) / Read3DVector 0x009950F0,
//   CArray<C3DVector>(unsigned long) 0x007C2AB0 / operator= 0x0075B8D0, operator new 0x00BFEA1A, free 0x00BFEA14,
//   operator delete 0x00BFE9BC, CSplineRandomOffsetData(CCardinalSpline*) 0x00B21DD0 / ReadBinary 0x00B22100,
//   CSplinePosParamData::ReadBinary 0x00B21C90 (its constructor is inline here), CArray<unsigned long>::resize 0x00464931,
//   CCountedPointer<..>::Reset(T*) 0x00B23240 / 0x00B23170.
// Layout: the landed CPSCSplineBase::DatabaseCopy 0x00B22530 / WriteBinary 0x00B21630 models: Spline +0x10,
//   SplineRandomOffsetData +0x24, SplinePosParamData +0x2c; CDataInputStream is the landed 0x00B16F60 model.
// Shape: the inverse of WriteBinary; the CSplinePosParamData constructor is the landed 0x00B21A00 body, inline in this TU.
// Prologue (what puts `this` in edi at the entry with the spill to [esp+10h], as retail): the point loop counts DOWN
//   (`for (unsigned long i = count; i != 0; --i, ++point)`); the up-counting loop compiles to the same `dec ebp` loop
//   but spills `this` straight from ecx and pushes edi later (592 vs 594).
#include "fable_compiler.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
#include "engine/C3DVector.h"
template <class T> class CArray { public:                     // STLport vector<T> (12 bytes in retail)
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) {}
    explicit CArray(unsigned long n);                  // 0x007C2AB0 (T = C3DVector)
    ~CArray() { if (m_begin) free(m_begin); }
    long Size() const { return (long)(m_end - m_begin); }
    T& operator[](unsigned long i) { return m_begin[i]; }
    CArray& operator=(const CArray& other);            // 0x0075B8D0 (T = C3DVector)
    void resize(unsigned long newSize, const T& value);   // 0x00464931 (T = unsigned long)
};
class CDataInputStream { public:
    virtual void v0();
    unsigned long StreamPos;                                           // +0x04
    long StreamSize;                                                   // +0x08
    const unsigned char* CurrentSourcePtr;                             // +0x0c
    unsigned long SourceChunkStreamPos;                                // +0x10
    long SourceChunkBytesRemaining;                                    // +0x14
    C3DVector Read3DVector();                                          // 0x009950F0
    static FABLE_FORCEINLINE bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    FABLE_FORCEINLINE void Read(void* dst, long size)
    {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    FABLE_FORCEINLINE unsigned char ReadUByte() { unsigned char value; Read(&value, 1); return value; }
    FABLE_FORCEINLINE unsigned long ReadULong() { unsigned long value; Read(&value, 4); return value; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};
class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;                   // +0x00
    float             Tension;                         // +0x0c
    bool              EndPointsConnected;              // +0x10
};
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
    void Clear()
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
        void ReadBinary(CDataInputStream& in);                     // 0x00B22100
    };
    class CSplinePosParamData { public:                       // 0x18 bytes
        CParticleSystem*      PParentParticleSystem;   // +0x00
        CCardinalSpline*      Spline;                  // +0x04
        bool                  MaintainSplineShape;     // +0x08
        CArray<unsigned long> SplinePosParam;          // +0x0c
        CSplinePosParamData(CCardinalSpline* spline)   // the landed 0x00B21A00 body
            : Spline(spline)
        {
            unsigned long count = Spline->ControlPoints.Size();
            SplinePosParam.resize(count, 0);
        }
        void ReadBinary(CDataInputStream& in);         // 0x00B21C90
    };
    float                                    SplineAnimSpeed;          // +0x0c
    CCardinalSpline                          Spline;                   // +0x10
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData>     SplinePosParamData;       // +0x2c
    virtual void ReadBinary(CDataInputStream& in);
};

void CPSCSplineBase::ReadBinary(CDataInputStream& in)
{
    unsigned long count = in.ReadULong();
    CArray<C3DVector> points(count);
    if (count > 0) {
        C3DVector* point = points.m_begin;
        for (unsigned long i = count; i != 0; --i, ++point)
            *point = in.Read3DVector();
    }
    Spline.ControlPoints = points;

    SplineRandomOffsetData.Clear();
    unsigned char hasRandomOffset = in.ReadUByte();
    if (hasRandomOffset) {
        SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&Spline));
        SplineRandomOffsetData->ReadBinary(in);
    }

    SplinePosParamData.Clear();
    unsigned char hasPosParam = in.ReadUByte();
    if (hasPosParam) {
        SplinePosParamData.Reset(new CSplinePosParamData(&Spline));
        SplinePosParamData->ReadBinary(in);
    }
}
}