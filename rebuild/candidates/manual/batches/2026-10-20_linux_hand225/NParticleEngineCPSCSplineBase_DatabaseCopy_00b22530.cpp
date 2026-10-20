// NParticleEngine::CPSCSplineBase::DatabaseCopy(const CPSCBase* other) -- copy another spline component's settings: the cardinal spline, then the random-offset and position-parameter data (allocated on demand, bound to this spline, settings copied) or released when the source has none
// Leaf: DatabaseCopy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B22530, 308 bytes (+ int3 padding), `ret 4` / tail `jmp`.
// Callees: CArray<C3DVector>::operator= 0x0075B8D0 (spline points, offset limits / speeds), operator new 0x00BFEA1A,
//   CSplineRandomOffsetData(CCardinalSpline*) 0x00B21DD0, CSplinePosParamData(CCardinalSpline*) 0x00B21A00,
//   CCountedPointer<..>::Reset(T*) 0x00B23240 / 0x00B23170, operator delete 0x00BFE9BC, CArray<unsigned long>::operator=
//   0x0045BC09 (tail call).
// Layout (retail, Ego_r.pdb less 4 bytes per CArray): CPSCBase (vptr, ParticleSystem, flags), SplineAnimSpeed +0x0c,
//   Spline +0x10 (ControlPoints, Tension +0x1c, EndPointsConnected +0x20), SplineRandomOffsetData +0x24,
//   SplinePosParamData +0x2c; CSplineRandomOffsetData (0x48): OffsetOnlyAtStartup +0x2c, ResetEachFrame +0x2d,
//   SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3c (its out-of-line copy is 0x00B20B60); CSplinePosParamData
//   (0x18): MaintainSplineShape +8, SplinePosParam +0xc.
// Shape: the implicit spline assignment; the data copies are inline member assignments (ResetEachFrame first); the
//   release-to-null clears Info before Data.
void* __cdecl operator new(unsigned int size);       // 0x00BFEA1A
void __cdecl operator delete(void* p);               // 0x00BFE9BC
struct C3DVector { float X, Y, Z; };
template <class T> struct CArray {                   // STLport vector<T> (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray& operator=(const CArray& other);          // 0x0075B8D0 (C3DVector) / 0x0045BC09 (unsigned long)
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;                 // +0x00
    float             Tension;                       // +0x0C
    bool              EndPointsConnected;            // +0x10
};
struct SCountBlock {
    long  m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);          // +4
    void* m_pObject;                                 // +8
};
template <class T> struct CCountedPointer {
    T*           Data;
    SCountBlock* Info;
    void Reset(T* object);                           // 0x00B23240 / 0x00B23170
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
struct CParticleSystem;
struct CPSCBase {
    virtual void v00();
    CParticleSystem* ParticleSystem;                 // +0x04
    bool UpdateStarted : 1;                          // +0x08
    bool Initialised : 1;
};
struct CPSCSplineBase : CPSCBase {
    struct CSplineRandomOffsetData {
        CCardinalSpline*  InitialSpline;             // +0x00
        CCardinalSpline   ThisSpline;                // +0x04
        CCardinalSpline   PreviousSpline;            // +0x18
        bool              OffsetOnlyAtStartup;       // +0x2C
        bool              ResetEachFrame;            // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;     // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;      // +0x3C
        CSplineRandomOffsetData(CCardinalSpline* initialSpline);   // 0x00B21DD0
        CSplineRandomOffsetData& operator=(const CSplineRandomOffsetData& other)
        {
            ResetEachFrame = other.ResetEachFrame;
            OffsetOnlyAtStartup = other.OffsetOnlyAtStartup;
            SplinePosOffsetLimits = other.SplinePosOffsetLimits;
            SplinePosOffsetSpeed = other.SplinePosOffsetSpeed;
            return *this;
        }
    };
    struct CSplinePosParamData {
        CParticleSystem*      PParentParticleSystem; // +0x00
        CCardinalSpline*      Spline;                // +0x04
        bool                  MaintainSplineShape;   // +0x08
        CArray<unsigned long> SplinePosParam;        // +0x0C
        CSplinePosParamData(CCardinalSpline* spline);   // 0x00B21A00
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
    void DatabaseCopy(const CPSCBase* other);
};

void CPSCSplineBase::DatabaseCopy(const CPSCBase* other)
{
    const CPSCSplineBase* source = (const CPSCSplineBase*)other;
    Spline = source->Spline;
    if (source->SplineRandomOffsetData.Data) {
        if (!SplineRandomOffsetData.Data)
            SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&Spline));
        *SplineRandomOffsetData.Data = *source->SplineRandomOffsetData.Data;
    } else
        SplineRandomOffsetData.Clear();
    if (source->SplinePosParamData.Data) {
        if (!SplinePosParamData.Data)
            SplinePosParamData.Reset(new CSplinePosParamData(&Spline));
        *SplinePosParamData.Data = *source->SplinePosParamData.Data;
    } else
        SplinePosParamData.Clear();
}
}
