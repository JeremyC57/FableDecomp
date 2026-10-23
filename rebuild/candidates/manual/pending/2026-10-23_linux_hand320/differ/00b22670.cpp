// NParticleEngine::CPSCSplineBase::DatabaseSerialise(CPersistContext& context, CEmitterDatabaseSerialiseInfo* info) -- persist the spline tension, connected-end-points flag and user points (old "+i" names or the _BugFixed indexed names when loading, at least two points), then the optional random-offset and position-parameter data
// Grade: DIFFER(1201v1217) -- structure matches; residuals: this in ebx (retail edi, add edi,10h), save-loop points/zero slots swapped, push_back copy not constant-folded to the zero register, overflow tag slot
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B22670, 1232 bytes, `ret 8` (virtual).
// Callees: CPersistContext::Transfer float 0x00410620 / bool 0x004045C0 / long 0x00410BE0 / C3DVector 0x004859AD,
//   CArray<C3DVector> copy ctor 0x006F0810 / resize 0x0075C040 / _M_insert_overflow 0x0066E370 / operator= 0x0075B8D0,
//   GFIntToCharString 0x0099F830, CCharString(const char*, long) 0x0099EBF0, operator+ 0x0099F570, ~CCharString
//   0x0099EAE0, free 0x00BFEA14, operator new 0x00BFEA1A, operator delete 0x00BFE9BC, CSplineRandomOffsetData ctor
//   0x00B21DD0 / DatabaseSerialise 0x00B21E90, CSplinePosParamData::DatabaseSerialise 0x00B21A50, CArray<unsigned long>::
//   resize 0x00464931, CCountedPointer::Reset 0x00B23240 / 0x00B23170.
// Data: "SplineTension" 0x012A0CD8, "SplineConnectedEndPoints" 0x012A0CBC, "SplineUserPointsArraySize_BugFixed"
//   0x012A0C98, "SplineUserPointsArray_" 0x012A0C80, "SplineUserPointsArraySize" 0x012A0C64, "SplineUserPointsArray"
//   0x012A0C4C (advanced by the index: the original bug the _BugFixed names replace), "OffsetPos" 0x012A0C40.
// Layout: the landed ReadBinary 0x00B22B40 / DatabaseCopy 0x00B22530 model.
// Leaf: DatabaseSerialise@CPSCSplineBase
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CCharStringData { const char* Data; long Length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const char* c_str() const { return !PStringData ? "" : PStringData->Data; }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);             // 0x004045C0
    void Transfer(const char* name, float& value, const float& defaultValue);           // 0x00410620
    void Transfer(const char* name, long& value, const long& defaultValue);             // 0x00410BE0
    void Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue);   // 0x004859AD
};
struct __false_type { };
template <class T> struct CArray {                     // STLport vector<T> (12 bytes in retail)
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CArray(const CArray& other);                       // 0x006F0810 (T = C3DVector)
    ~CArray() { if (_M_start) free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return _M_start[n]; }
    CArray& operator=(const CArray& other);            // 0x0075B8D0
    void resize(unsigned int newSize, const T& value = T());   // 0x0075C040 / 0x00464931
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int n, bool atend);   // 0x0066E370
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else {
            __false_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1, true);
        }
    }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;                   // +0x00
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
struct CEmitterDatabaseSerialiseInfo;
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
        void DatabaseSerialise(CPersistContext& context, CEmitterDatabaseSerialiseInfo* info);   // 0x00B21E90
    };
    struct CSplinePosParamData {                       // 0x18 bytes
        CParticleSystem*      PParentParticleSystem;   // +0x00
        CCardinalSpline*      Spline;                  // +0x04
        bool                  MaintainSplineShape;     // +0x08
        CArray<unsigned long> SplinePosParam;          // +0x0c
        __forceinline CSplinePosParamData(CCardinalSpline* spline)   // the landed 0x00B21A00 body
            : Spline(spline)
        {
            unsigned long count = Spline->ControlPoints.size();
            SplinePosParam.resize(count, 0);
        }
        void DatabaseSerialise(CPersistContext& context, CEmitterDatabaseSerialiseInfo* info);   // 0x00B21A50
    };
    float                                    SplineAnimSpeed;          // +0x0c
    CCardinalSpline                          Spline;                   // +0x10
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData>     SplinePosParamData;       // +0x2c
    void DatabaseSerialise(CPersistContext& context, CEmitterDatabaseSerialiseInfo* info);
};

void CPSCSplineBase::DatabaseSerialise(CPersistContext& context, CEmitterDatabaseSerialiseInfo* info)
{
    float tension = Spline.Tension;
    context.Transfer("SplineTension", tension, 0.0f);
    Spline.Tension = tension;
    bool connected = Spline.EndPointsConnected;
    context.Transfer("SplineConnectedEndPoints", connected, false);
    Spline.EndPointsConnected = connected;
    long count;
    if (context.IsSaving()) {
        count = Spline.ControlPoints.size();
        context.Transfer("SplineUserPointsArraySize_BugFixed", count, 0L);
        CArray<C3DVector> points(Spline.ControlPoints);
        long i = 0;
        if (i < count) {
            C3DVector* point = points._M_start;
            C3DVector zero(0.0f, 0.0f, 0.0f);
            do {
                context.Transfer((CCharString("SplineUserPointsArray_") + GFIntToCharString(i)).c_str(), *point, zero);
            } while (++i, ++point, i < count);
        }
    } else {
        CArray<C3DVector> points;
        context.Transfer("SplineUserPointsArraySize", count, 0L);
        if (count > 0) {
            if (count > 20)
                count = 20;
            points.resize(count);
            long i = 0;
            if (i < count) {
                C3DVector* point = points._M_start;
                C3DVector zero(0.0f, 0.0f, 0.0f);
                do {
                    context.Transfer("SplineUserPointsArray" + i, *point, zero);
                } while (++i, ++point, i < count);
            }
        } else {
            context.Transfer("SplineUserPointsArraySize_BugFixed", count, 0L);
            points.resize(count);
            long i = 0;
            if (i < count) {
                C3DVector* point = points._M_start;
                C3DVector zero(0.0f, 0.0f, 0.0f);
                do {
                    context.Transfer((CCharString("SplineUserPointsArray_") + GFIntToCharString(i)).c_str(), *point, zero);
                } while (++i, ++point, i < count);
            }
        }
        while (points.size() < 2)
            points.push_back(C3DVector(0.0f, 0.0f, 0.0f));
        Spline.ControlPoints = points;
    }
    bool hasOffset = SplineRandomOffsetData.Data != 0;
    context.Transfer("OffsetPos", hasOffset, false);
    if (hasOffset) {
        if (!context.IsSaving())
            SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&Spline));
        SplineRandomOffsetData->DatabaseSerialise(context, info);
    }
    if (!context.IsSaving())
        SplinePosParamData.Reset(new CSplinePosParamData(&Spline));
    SplinePosParamData->DatabaseSerialise(context, info);
    if (!(SplinePosParamData->SplinePosParam.size() > 0))
        SplinePosParamData.Clear();
}
}
