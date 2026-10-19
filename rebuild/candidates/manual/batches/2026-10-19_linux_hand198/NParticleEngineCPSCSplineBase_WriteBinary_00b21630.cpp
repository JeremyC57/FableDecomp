// NParticleEngine::CPSCSplineBase::WriteBinary(CDataOutputStream&) -- write the spline's control-point count and points (from a copy of the array), then a presence flag and the body of the random-offset and pos-param data when they exist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21630, length 192 bytes. Virtual: slot 16 of
// NParticleEngine::CPSCSplineBase's vtable 0x012A0B14.
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0, Write3DVector @ 0x00993FD0, WriteBool @ 0x00993E30 (x2);
//          CArray<C3DVector> copy constructor @ 0x006F0810; CSplineRandomOffsetData::WriteBinary @ 0x00B20630
//          (manifest label correct); CSplinePosParamData::WriteBinary @ 0x00B20B10 (manifest label
//          "C3DMeshFileUniqueVerticesChunk::WriteToFile" is propagated); free @ 0x00BFEA14 (the copy's storage).
// Layout: as the landed CPSCSplineBase rows (0x00B215A0 Initialise, 0x00B21940 GetPointAtPos): CCardinalSpline
// at +0x10 (12-byte CArray<C3DVector> + tension + flag), two counted pointers at +0x24 / +0x2c.
// Shape: the count comes from the member array (signed 12-byte difference), the points are written from a
// local copy whose destructor (free when non-null) closes the function; each presence flag is a bool local
// homed in the dead stream-argument slot and pushed as a dword.
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

struct C3DVector { float X, Y, Z; };

template<class T> struct CArray {                          // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    CArray(const CArray& other);                           // 0x006F0810 (T = C3DVector)
    ~CArray() { if (m_pData) free(m_pData); }
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
    T& operator[](unsigned long i) { return m_pData[i]; }
};

struct CDataOutputStream {
    void WriteBool(bool value);                            // 0x00993E30
    void WriteULong(unsigned long value);                  // 0x00993EA0
    void Write3DVector(const C3DVector& value);            // 0x00993FD0
};

namespace NParticleEngine {
struct CParticleSystem;

struct CCardinalSpline {                    // 0x14 bytes in retail
    CArray<C3DVector> ControlPoints;        // +0x00
    float             Tension;              // +0x0c
    bool              EndPointsConnected;   // +0x10
};

struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};

struct CSplineRandomOffsetData {
    void WriteBinary(CDataOutputStream& stream);           // 0x00B20630
};

struct CSplinePosParamData {
    void WriteBinary(CDataOutputStream& stream);           // 0x00B20B10
};

struct CPSCBase {                           // vtable 0x0129F3F4
    virtual void v000();
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};

struct CPSCSplineBase : CPSCBase {          // vtable 0x012A0B14
    float                                     SplineAnimSpeed;        // +0x0c
    CCardinalSpline                           Spline;                 // +0x10
    CCountedPointer<CSplineRandomOffsetData>  SplineRandomOffsetData; // +0x24
    CCountedPointer<CSplinePosParamData>      SplinePosParamData;     // +0x2c
    virtual void WriteBinary(CDataOutputStream& stream);
};

void CPSCSplineBase::WriteBinary(CDataOutputStream& stream)
{
    unsigned long count = Spline.ControlPoints.Size();
    stream.WriteULong(count);
    CArray<C3DVector> points(Spline.ControlPoints);
    if (count > 0) {
        C3DVector* point = points.m_pData;
        for (unsigned long i = 0; i < count; ++i, ++point)
            stream.Write3DVector(*point);
    }

    bool hasRandomOffset = SplineRandomOffsetData.Data != 0;
    stream.WriteBool(hasRandomOffset);
    if (hasRandomOffset)
        SplineRandomOffsetData->WriteBinary(stream);

    bool hasPosParam = SplinePosParamData.Data != 0;
    stream.WriteBool(hasPosParam);
    if (hasPosParam)
        SplinePosParamData->WriteBinary(stream);
}
}
