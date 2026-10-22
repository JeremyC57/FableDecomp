// NParticleEngine::CPSCSplineBase::InterpolateTo(const CPSCBase* other, float t) -- blend this spline component towards another: lerp the tension, interpolate the control points (GFInterpolateArray over copies of both point arrays), and blend the random-offset and position-parameter data (interpolated when both exist; otherwise past the halfway point the own data is released or a copy of the other's is created)
// Leaf: InterpolateTo
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
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
template <class T> struct CArray;
template <class T> CArray<T> __fastcall GFInterpolateArray(const CArray<T>& a, const CArray<T>& b, float t, const T& defaultValue);
template <class T> struct CArray {                     // STLport vector<T> (12 bytes in retail)
    T* m_begin; T* m_end; T* m_capacity;
    CArray(const CArray& other);                       // 0x006F0810 (T = C3DVector)
    ~CArray() { if (m_begin) free(m_begin); }
    long Size() const { return (long)(m_end - m_begin); }
    CArray& operator=(const CArray& other);            // 0x0075B8D0 (T = C3DVector)
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;                   // +0x00
    CArray<C3DVector> GetControlPoints() const { return ControlPoints; }
    float             Tension;                         // +0x0c
    bool              EndPointsConnected;              // +0x10
};
struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> struct CCountedPointer {
    T*           Data;
    SCountBlock* Info;
    void Reset(T* object);                             // 0x00B23240 / 0x00B23170
    T* operator->() const { return Data; }
    __forceinline void Clear()
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
    CParticleSystem* ParticleSystem;                   // +0x04
    bool UpdateStarted : 1;                            // +0x08
    bool Initialised : 1;
};
struct CPSCSplineBase : CPSCBase {
    struct CSplineRandomOffsetData {                   // 0x48 bytes
        unsigned char _body[0x48];
        CSplineRandomOffsetData(CCardinalSpline* initialSpline);   // 0x00B21DD0
        void InterpolateTo(const CSplineRandomOffsetData& other, float t);   // 0x00B211B0
        CSplineRandomOffsetData& operator=(const CSplineRandomOffsetData& other);   // 0x00B20B60
    };
    struct CSplinePosParamData {                       // 0x18 bytes
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
    void InterpolateTo(const CPSCBase* other, float t);
};

void CPSCSplineBase::InterpolateTo(const CPSCBase* other, float t)
{
    const CPSCSplineBase* source = (const CPSCSplineBase*)other;
    Spline.Tension = (1.0f - t) * Spline.Tension + t * source->Spline.Tension;
    Spline.ControlPoints = GFInterpolateArray(Spline.GetControlPoints(), source->Spline.GetControlPoints(), t, C3DVector(0.0f, 0.0f, 0.0f));

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