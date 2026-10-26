// Behaviour test for 00b22b40: final.cpp copied verbatim (CRT names wrapped and 32-bit `long` under g++; build with
// `g++ -fpermissive`), every declared callee stubbed below so the stream reads, allocations and the rebuilt spline are
// observable. NParticleEngine::CPSCSplineBase::ReadBinary(CDataInputStream*)
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* CopyBytes(void* d, const void* s, unsigned int n) { return std::memcpy(d, s, n); }
#ifndef _MSC_VER
#include <stdint.h>
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 int64_t
extern "C" void* fable_memcpy(void* d, const void* s, unsigned int n) { return CopyBytes(d, s, n); }
#define memcpy fable_memcpy
#define long int
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NParticleEngine::CPSCSplineBase::ReadBinary(CDataInputStream* in) -- load a spline component: the control-point count and points into the cardinal spline, then the optional random-offset data and the optional position-parameter data (each re-created bound to the spline and read by itself, or left released)
// Leaf: ReadBinary
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
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* p);                 // 0x00BFE9BC
class C3DVector { public: float X, Y, Z; };
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
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    C3DVector Read3DVector();                                          // 0x009950F0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
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
    __forceinline unsigned char ReadUByte() { unsigned char value; Read(&value, 1); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, 4); return value; }
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
        void ReadBinary(CDataInputStream* in);                     // 0x00B22100
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
        void ReadBinary(CDataInputStream* in);         // 0x00B21C90
    };
    float                                    SplineAnimSpeed;          // +0x0c
    CCardinalSpline                          Spline;                   // +0x10
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;   // +0x24
    CCountedPointer<CSplinePosParamData>     SplinePosParamData;       // +0x2c
    void ReadBinary(CDataInputStream* in);
};

void CPSCSplineBase::ReadBinary(CDataInputStream* in)
{
    unsigned long count = in->ReadULong();
    CArray<C3DVector> points(count);
    if (count > 0) {
        C3DVector* point = points.m_begin;
        for (unsigned long i = count; i != 0; --i, ++point)
            *point = in->Read3DVector();
    }
    Spline.ControlPoints = points;

    SplineRandomOffsetData.Clear();
    unsigned char hasRandomOffset = in->ReadUByte();
    if (hasRandomOffset) {
        SplineRandomOffsetData.Reset(new CSplineRandomOffsetData(&Spline));
        SplineRandomOffsetData->ReadBinary(in);
    }

    SplinePosParamData.Clear();
    unsigned char hasPosParam = in->ReadUByte();
    if (hasPosParam) {
        SplinePosParamData.Reset(new CSplinePosParamData(&Spline));
        SplinePosParamData->ReadBinary(in);
    }
}
}

// ---- stubs ----
using namespace NParticleEngine;
static int g_news, g_deletes, g_overflowReads, g_randomCtor, g_randomRead, g_posRead, g_destroys;
static CCardinalSpline* g_randomSpline; static CDataInputStream* g_randomReadStream; static CDataInputStream* g_posReadStream;
void* __cdecl operator new(unsigned int size) { ++g_news; return std::malloc(size); }
void __cdecl operator delete(void* p) { ++g_deletes; std::free(p); }
void CDataInputStream::v0() { }
struct CTestStream : CDataInputStream { const unsigned char* Source; };
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++g_overflowReads; const unsigned char* src = ((CTestStream*)this)->Source + StreamPos;
    CopyBytes(buffer, src, size); StreamPos += size; CurrentSourcePtr = src + size; SourceChunkBytesRemaining = StreamSize - (long)StreamPos;
}
C3DVector CDataInputStream::Read3DVector() { C3DVector v; Read(&v, 12); return v; }
template <class T> CArray<T>::CArray(unsigned long n) { m_begin = n ? (T*)std::malloc(n * sizeof(T)) : 0; m_end = m_begin + n; m_capacity = m_end; }
template <class T> CArray<T>& CArray<T>::operator=(const CArray<T>& other)
{
    unsigned long n = (unsigned long)other.Size(); if (m_begin) std::free(m_begin);
    m_begin = n ? (T*)std::malloc(n * sizeof(T)) : 0; for (unsigned long i = 0; i < n; ++i) m_begin[i] = other.m_begin[i]; m_end = m_begin + n; m_capacity = m_end; return *this;
}
template <class T> void CArray<T>::resize(unsigned long newSize, const T& value)
{
    T* block = newSize ? (T*)std::malloc(newSize * sizeof(T)) : 0; for (unsigned long i = 0; i < newSize; ++i) block[i] = value;
    if (m_begin) std::free(m_begin); m_begin = block; m_end = block + newSize; m_capacity = m_end;
}
template <class T> static void __fastcall DestroyCounted(void* p) { ++g_destroys; delete (T*)p; }
template <class T> void CCountedPointer<T>::Reset(T* object)
{
    Clear(); Info = (SCountBlock*)operator new(sizeof(SCountBlock)); Info->m_references = 1; Info->m_pfnDestroy = &DestroyCounted<T>; Info->m_pObject = object; Data = object;
}
CPSCSplineBase::CSplineRandomOffsetData::CSplineRandomOffsetData(CCardinalSpline* initialSpline) { ++g_randomCtor; g_randomSpline = initialSpline; std::memset(_body, 0, sizeof(_body)); }
void CPSCSplineBase::CSplineRandomOffsetData::ReadBinary(CDataInputStream* in) { ++g_randomRead; g_randomReadStream = in; unsigned long v; in->Read(&v, 4); _body[0] = (unsigned char)v; }
void CPSCSplineBase::CSplinePosParamData::ReadBinary(CDataInputStream* in) { ++g_posRead; g_posReadStream = in; unsigned long v; in->Read(&v, 4); if (SplinePosParam.Size() > 0) SplinePosParam[0] = v; }
void CPSCBase::v00() { }
static void PutLong(unsigned char* p, unsigned long v) { CopyBytes(p, &v, 4); }
static void PutFloat(unsigned char* p, float v) { CopyBytes(p, &v, 4); }
struct Fixture {
    CPSCSplineBase spline; CTestStream stream; unsigned char data[128]; long length;
    Fixture()
    {
        std::memset(&spline, 0, sizeof(spline)); std::memset(&stream, 0, sizeof(stream)); std::memset(data, 0, sizeof(data)); length = 0;
        g_news = g_deletes = g_overflowReads = g_randomCtor = g_randomRead = g_posRead = g_destroys = 0; g_randomSpline = 0; g_randomReadStream = g_posReadStream = 0;
    }
    void Begin(long chunkBytes) { stream.Source = data; stream.StreamPos = 0; stream.StreamSize = length; stream.CurrentSourcePtr = data; stream.SourceChunkStreamPos = 0; stream.SourceChunkBytesRemaining = chunkBytes < 0 ? length : chunkBytes; }
    void Add(unsigned long v) { PutLong(data + length, v); length += 4; }
    void AddByte(unsigned char v) { data[length++] = v; }
    void AddPoint(float x, float y, float z) { PutFloat(data + length, x); PutFloat(data + length + 4, y); PutFloat(data + length + 8, z); length += 12; }
    ~Fixture() { spline.SplineRandomOffsetData.Clear(); spline.SplinePosParamData.Clear(); if (spline.Spline.ControlPoints.m_begin) std::free(spline.Spline.ControlPoints.m_begin); spline.Spline.ControlPoints.m_begin = 0; }
};
int main()
{
    {   // two points, both optional blocks present, whole stream in one chunk
        Fixture f; f.Add(2); f.AddPoint(1.0f, 2.0f, 3.0f); f.AddPoint(-4.0f, 5.5f, 6.25f); f.AddByte(1); f.Add(0x11223344); f.AddByte(1); f.Add(0x55667788); f.Begin(-1);
        f.spline.ReadBinary(&f.stream);
        CHECK(f.spline.Spline.ControlPoints.Size() == 2 && f.spline.Spline.ControlPoints[0].X == 1.0f && f.spline.Spline.ControlPoints[0].Z == 3.0f && f.spline.Spline.ControlPoints[1].X == -4.0f && f.spline.Spline.ControlPoints[1].Y == 5.5f);
        CHECK(g_randomCtor == 1 && g_randomSpline == &f.spline.Spline && g_randomRead == 1 && g_randomReadStream == &f.stream);
        CHECK(f.spline.SplineRandomOffsetData.Data != 0 && f.spline.SplineRandomOffsetData.Info != 0 && f.spline.SplineRandomOffsetData.Info->m_references == 1 && f.spline.SplineRandomOffsetData.Data->_body[0] == 0x44);
        CHECK(g_posRead == 1 && g_posReadStream == &f.stream && f.spline.SplinePosParamData.Data != 0);
        CPSCSplineBase::CSplinePosParamData* pos = f.spline.SplinePosParamData.Data;
        CHECK(pos->Spline == &f.spline.Spline && pos->SplinePosParam.Size() == 2 && pos->SplinePosParam[0] == 0x55667788 && pos->SplinePosParam[1] == 0);
        CHECK(f.stream.StreamPos == (unsigned long)f.length && g_overflowReads == 0 && g_destroys == 0);
    }
    {   // existing data is released first; no points and no optional blocks leave everything empty
        Fixture f; f.Add(0); f.AddByte(0); f.AddByte(0); f.Begin(-1);
        f.spline.SplineRandomOffsetData.Reset(new CPSCSplineBase::CSplineRandomOffsetData(&f.spline.Spline));
        f.spline.SplinePosParamData.Reset(new CPSCSplineBase::CSplinePosParamData(&f.spline.Spline));
        g_destroys = 0; g_deletes = 0; g_randomCtor = 0;
        f.spline.ReadBinary(&f.stream);
        CHECK(f.spline.Spline.ControlPoints.Size() == 0 && g_destroys == 2 && g_deletes >= 2);
        CHECK(f.spline.SplineRandomOffsetData.Data == 0 && f.spline.SplineRandomOffsetData.Info == 0 && f.spline.SplinePosParamData.Data == 0 && f.spline.SplinePosParamData.Info == 0);
        CHECK(g_randomCtor == 0 && g_randomRead == 0 && g_posRead == 0 && f.stream.StreamPos == 6);
    }
    {   // one point, only the position-parameter block, chunked so the inlined reads take the overflow path
        Fixture f; f.Add(1); f.AddPoint(7.0f, 8.0f, 9.0f); f.AddByte(0); f.AddByte(1); f.Add(3); f.Begin(2);
        f.spline.ReadBinary(&f.stream);
        CHECK(f.spline.Spline.ControlPoints.Size() == 1 && f.spline.Spline.ControlPoints[0].Y == 8.0f);
        CHECK(g_overflowReads >= 1 && g_randomCtor == 0 && f.spline.SplineRandomOffsetData.Data == 0);
        CHECK(g_posRead == 1 && f.spline.SplinePosParamData.Data != 0 && f.spline.SplinePosParamData.Data->SplinePosParam.Size() == 1 && f.spline.SplinePosParamData.Data->SplinePosParam[0] == 3);
    }
    if (fails) { printf("00b22b40_TEST FAIL (%d)\n", fails); return 1; }
    printf("00b22b40_TEST PASS\n");
    return 0;
}