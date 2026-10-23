#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CSplineRandomOffsetData::DatabaseSerialise transfers flags, count, indexed limit/speed names and snapshots/clears the previous spline


struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CCharString {
    char buf[64];
    CCharString(const char* text, long length = -1) { (void)length; std::strcpy(buf, text); }
    ~CCharString() {}
    operator const char*() const { return buf; }
};
CCharString operator+(const CCharString& left, const CCharString& right) { CCharString r(left.buf); std::strcat(r.buf, right.buf); return r; }
CCharString GFIntToCharString(long value) { char t[16]; std::sprintf(t, "%ld", value); return CCharString(t); }

struct CPersistContext {
    char names[16][32]; int count; long loadCount;
    void Note(const char* n) { std::strcpy(names[count++], n); }
    void Transfer(const char* name, bool& value, const bool& defaultValue) { Note(name); if (!defaultValue) value = !value; }
    void Transfer(const char* name, long& value, const long& defaultValue) { Note(name); value = loadCount + defaultValue; }
    void Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue) { Note(name); value.X = (float)count + defaultValue.X; }
};

template <class T> struct CArray {              // STLport vector<T> (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
    CArray& operator=(const CArray& other) { _M_finish = _M_start; for (T* p = other._M_start; p != other._M_finish; ++p) *_M_finish++ = *p; return *this; }
    void resize(unsigned int newSize, const C3DVector& value = C3DVector()) { while (size() < newSize) *_M_finish++ = value; _M_finish = _M_start + newSize; }
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

static C3DVector bufA[8], bufB[8], bufC[8], bufD[8], bufI[8];
static void Init(CArray<C3DVector>& a, C3DVector* b) { a._M_start = a._M_finish = b; a._M_end_of_storage = b + 8; }
int main()
{
    using namespace NParticleEngine;
    CCardinalSpline initial; Init(initial.ControlPoints, bufI); initial.ControlPoints._M_finish = bufI + 3;
    CPSCSplineBase::CSplineRandomOffsetData d;
    d.InitialSpline = &initial;
    Init(d.ThisSpline.ControlPoints, bufA); Init(d.PreviousSpline.ControlPoints, bufB);
    Init(d.SplinePosOffsetLimits, bufC); Init(d.SplinePosOffsetSpeed, bufD);
    d.ThisSpline.ControlPoints._M_finish = bufA + 1; bufA[0].Z = 3.0f; d.ThisSpline.Tension = 0.5f;
    d.OffsetOnlyAtStartup = false; d.ResetEachFrame = true;
    CPersistContext c; c.count = 0; c.loadCount = 2;
    d.DatabaseSerialise(c, true);
    bool ok = c.count == 7 && d.OffsetOnlyAtStartup && !d.ResetEachFrame
        && std::strcmp(c.names[2], "SplinePosOffsetLimitsSize") == 0 && std::strcmp(c.names[3], "SplinePosOffsetLimit_0") == 0
        && std::strcmp(c.names[6], "SplinePosOffsetSpeed_1") == 0 && d.SplinePosOffsetLimits.size() == 3
        && d.SplinePosOffsetSpeed.size() == 3 && bufC[1].X == 6.0f && bufD[1].X == 7.0f
        && d.PreviousSpline.ControlPoints.size() == 1 && bufB[0].Z == 3.0f && d.PreviousSpline.Tension == 0.5f;
    c.count = 0; c.loadCount = 4;
    d.DatabaseSerialise(c, true);
    ok = ok && !d.OffsetOnlyAtStartup && d.SplinePosOffsetLimits.size() == 4 && d.PreviousSpline.ControlPoints.size() == 0 && c.count == 11;
    if (!ok) { std::printf("00b21e90_TEST FAIL\n"); return 1; }
    std::printf("00b21e90_TEST PASS\n");
    return 0;
}
