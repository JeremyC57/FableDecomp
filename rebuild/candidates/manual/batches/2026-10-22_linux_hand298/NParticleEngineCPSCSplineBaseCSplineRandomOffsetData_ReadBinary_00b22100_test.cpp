#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CSplineRandomOffsetData::ReadBinary reads flags, count and the per-point limit/speed vectors, then snapshots or clears the previous spline


#include <cstring>

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};

struct CDataInputStream {
    void* m_pVtable;                       // +0
    unsigned int m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { std::memcpy(buffer, m_pData, size); m_pData += size; m_position += size; }
    C3DVector Read3DVector() { C3DVector v; Read(&v.X, 4); Read(&v.Y, 4); Read(&v.Z, 4); return v; }
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
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
    __forceinline unsigned int ReadULong() { unsigned int value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
};

template <class T> struct CArray {              // STLport vector<T> (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    CArray& operator=(const CArray& other) { _M_finish = _M_start; for (T* p = other._M_start; p != other._M_finish; ++p) *_M_finish++ = *p; return *this; }
    void resize(unsigned int newSize, const C3DVector& value = C3DVector()) { while ((unsigned int)(_M_finish - _M_start) < newSize) *_M_finish++ = value; _M_finish = _M_start + newSize; }
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
        void ReadBinary(CDataInputStream& in);
    };
};

void CPSCSplineBase::CSplineRandomOffsetData::ReadBinary(CDataInputStream& in)
{
    OffsetOnlyAtStartup = in.ReadBool();
    ResetEachFrame = in.ReadBool();
    unsigned int count = in.ReadULong();
    SplinePosOffsetLimits.resize(count);
    SplinePosOffsetSpeed.resize(count);
    for (unsigned int i = 0; i < count; ++i) {
        SplinePosOffsetLimits[i] = in.Read3DVector();
        SplinePosOffsetSpeed[i] = in.Read3DVector();
    }
    if (OffsetOnlyAtStartup)
        PreviousSpline = ThisSpline;
    else
        PreviousSpline.ControlPoints.clear();
}
}

static C3DVector bufA[8], bufB[8], bufC[8], bufD[8];
static void Init(CArray<C3DVector>& a, C3DVector* b) { a._M_start = a._M_finish = b; a._M_end_of_storage = b + 8; }
int main()
{
    using namespace NParticleEngine;
    CPSCSplineBase::CSplineRandomOffsetData d;
    Init(d.ThisSpline.ControlPoints, bufA); Init(d.PreviousSpline.ControlPoints, bufB);
    Init(d.SplinePosOffsetLimits, bufC); Init(d.SplinePosOffsetSpeed, bufD);
    d.ThisSpline.ControlPoints._M_finish = bufA + 2; bufA[1].Y = 4.0f; d.ThisSpline.Tension = 0.75f; d.ThisSpline.EndPointsConnected = true;
    d.PreviousSpline.ControlPoints._M_finish = bufB + 3;
    unsigned char data[64]; float f[12];
    for (int i = 0; i < 12; ++i) f[i] = 1.0f + i;
    data[0] = 1; data[1] = 0; unsigned int n = 2; std::memcpy(data + 2, &n, 4); std::memcpy(data + 6, f, 48);
    CDataInputStream s; s.m_pVtable = 0; s.m_position = 0; s.StreamSize = 54; s.m_pData = data; s.SourceChunkStreamPos = 0; s.m_remaining = 10;
    d.ReadBinary(s);
    bool ok = d.OffsetOnlyAtStartup && !d.ResetEachFrame && d.SplinePosOffsetLimits._M_finish == bufC + 2
        && bufC[0].X == 1.0f && bufD[0].X == 4.0f && bufC[1].Z == 9.0f && bufD[1].Z == 12.0f
        && d.PreviousSpline.ControlPoints._M_finish == bufB + 2 && bufB[1].Y == 4.0f && d.PreviousSpline.Tension == 0.75f
        && d.PreviousSpline.EndPointsConnected;
    data[0] = 0; n = 0; std::memcpy(data + 2, &n, 4);
    s.m_position = 0; s.m_pData = data; s.m_remaining = 64;
    d.ReadBinary(s);
    ok = ok && !d.OffsetOnlyAtStartup && d.PreviousSpline.ControlPoints._M_finish == bufB && d.SplinePosOffsetLimits._M_finish == bufC;
    if (!ok) { std::printf("00b22100_TEST FAIL\n"); return 1; }
    std::printf("00b22100_TEST PASS\n");
    return 0;
}
