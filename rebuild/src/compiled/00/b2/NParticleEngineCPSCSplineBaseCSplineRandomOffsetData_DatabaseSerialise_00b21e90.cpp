// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::DatabaseSerialise(CPersistContext& context, bool) -- persist the two flags, the offset count (the arrays sized to at least the initial spline's point count), each offset limit/speed by indexed name, then snapshot or clear the previous spline
// Leaf: DatabaseSerialise@CSplineRandomOffsetData
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B21E90, 624 bytes, `ret 8`.
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer(name, long&, const long&) 0x00410BE0,
//   Transfer(name, C3DVector&, const C3DVector&) 0x004859AD, CArray<C3DVector>::resize 0x0075C040,
//   CArray<C3DVector>::operator= 0x0075B8D0, GFIntToCharString 0x0099F830 (__fastcall), CCharString(const char*, long)
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
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

struct CPersistContext {
    void Transfer(const char* name, bool& value, const bool& defaultValue);             // 0x004045C0
    void Transfer(const char* name, long& value, const long& defaultValue);             // 0x00410BE0
    void Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue);   // 0x004859AD
};

template <class T> struct CArray {              // STLport vector<T> (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
    CArray& operator=(const CArray& other);                                  // 0x0075B8D0
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());  // 0x0075C040
    static __forceinline T* copy(T* first, T* last, T* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    __forceinline T* erase(T* first, T* last)
    {
        T* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    __forceinline void clear() { erase(begin(), end()); }
};

struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;    // +0x00
    float             Tension;          // +0x0C
    bool              EndPointsConnected; // +0x10
};

template <class T>
__forceinline void SaveValue(CPersistContext& context, const char* name, T& value, const T& defaultValue)
{
    context.Transfer(name, value, defaultValue);
}

namespace NParticleEngine {
struct CPSCSplineBase {
    struct CSplineRandomOffsetData {
        CCardinalSpline*  InitialSpline;          // +0x00
        CCardinalSpline   ThisSpline;             // +0x04
        CCardinalSpline   PreviousSpline;         // +0x18
        bool              OffsetOnlyAtStartup;    // +0x2C
        bool              ResetEachFrame;         // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;  // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;   // +0x3C
        void DatabaseSerialise(CPersistContext& context, bool transfer);
    };
};

void CPSCSplineBase::CSplineRandomOffsetData::DatabaseSerialise(CPersistContext& context, bool)
{
    context.Transfer("OffsetOnlyAtStartup", OffsetOnlyAtStartup, false);
    context.Transfer("ResetEachFrame", ResetEachFrame, false);
    long count = SplinePosOffsetLimits.size();
    context.Transfer("SplinePosOffsetLimitsSize", count, 0L);
    SplinePosOffsetLimits.resize(count > (long)InitialSpline->ControlPoints.size()
        ? count : InitialSpline->ControlPoints.size());
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
        PreviousSpline.ControlPoints.clear();
}
}