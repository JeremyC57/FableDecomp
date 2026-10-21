// Behaviour test for 0x00F3D830 (CTriangleStripifier2::CalcSectionScore): the scorer over host arrays with a scripted
// polygon mask (test doubles for CalcSectionPolyMask / StripUnusedVerts), checking the 0 / 1e30 / ratio outcomes.
#include <cstdio>
#include <cstdlib>
#include <cstring>
template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CArray(const CArray& o) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned long n = o.size();
        if (n) { _M_start = (T*)std::malloc(n * sizeof(T)); std::memcpy(_M_start, o._M_start, n * sizeof(T)); }
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    ~CArray() { if (_M_start) std::free(_M_start); }
    void assign(const T* p, unsigned long n)
    {
        _M_start = (T*)std::malloc(n * sizeof(T) + 1); std::memcpy(_M_start, p, n * sizeof(T));
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
static const bool* g_maskScript; static unsigned long g_maskCount; static long g_marked; static int g_stripCalls;
struct Stripifier {
    long CalcSectionPolyMask(const CArray<unsigned short>&, const CArray<CArray<long> >&, const CArray<unsigned short>&, CArray<bool>& mask)
    {
        if (g_maskCount) mask.assign(g_maskScript, g_maskCount);
        return g_marked;
    }
    void StripUnusedVerts(CArray<unsigned short>& verts, const CArray<CArray<long> >&, const CArray<bool>&)
    {
        ++g_stripCalls;
        if (verts.size()) --verts._M_finish;                      // drop the last vertex
    }
    float CalcSectionScore(const CArray<unsigned short>& section, const CArray<unsigned short>& triangles, const CArray<CArray<long> >& vertexPolys)
    {
        CArray<bool> polyMask;
        long count = CalcSectionPolyMask(triangles, vertexPolys, section, polyMask);
        if (count == 0)
            return 0.0f;
        CArray<unsigned short> verts(section);
        StripUnusedVerts(verts, vertexPolys, polyMask);
        long misses = 0;
        for (unsigned long i = 0; i < verts.size(); ++i) {
            for (unsigned long j = 0; j < vertexPolys[verts[i]].size(); ++j) {
                if (!polyMask[vertexPolys[verts[i]][j]]) { ++misses; break; }
            }
        }
        if (misses == 0)
            return 1e30f;
        return (float)count / misses;
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
int main()
{
    // 4 vertices; vertex polys: v0 {0,1}, v1 {1}, v2 {2,3}, v3 {3}
    CArray<CArray<long> > vp; vp._M_start = new CArray<long>[4]; vp._M_finish = vp._M_end_of_storage = vp._M_start + 4;
    long p0[] = { 0, 1 }, p1[] = { 1 }, p2[] = { 2, 3 }, p3[] = { 3 };
    vp[0].assign(p0, 2); vp[1].assign(p1, 1); vp[2].assign(p2, 2); vp[3].assign(p3, 1);
    unsigned short sec[] = { 0, 1, 2, 3 };
    CArray<unsigned short> section; section.assign(sec, 4);
    CArray<unsigned short> tris;
    Stripifier s;
    bool none[] = { false, false, false, false };
    bool some[] = { true, true, false, true };                  // polygon 2 unmarked
    bool all[] = { true, true, true, true };
    g_maskScript = none; g_maskCount = 4; g_marked = 0; g_stripCalls = 0;
    CHECK(s.CalcSectionScore(section, tris, vp) == 0.0f && g_stripCalls == 0);
    g_maskScript = all; g_marked = 4;
    CHECK(s.CalcSectionScore(section, tris, vp) == 1e30f && g_stripCalls == 1);
    // v2 touches polygon 2 (unmarked) -> 1 miss; v3 is stripped by the double: 3 / 1
    g_maskScript = some; g_marked = 3;
    CHECK(s.CalcSectionScore(section, tris, vp) == 3.0f);
    // polygon 0 and 3 unmarked: v0 misses (poly 0), v2 misses (poly 3, after 2 marked) -> 2 / 2
    bool two[] = { false, true, true, false }; g_maskScript = two; g_marked = 2;
    CHECK(s.CalcSectionScore(section, tris, vp) == 1.0f);
    CHECK(section.size() == 4);                                  // the section itself is untouched
    delete[] vp._M_start; vp._M_start = 0;
    if (g_fail) { std::printf("00f3d830_TEST FAIL\n"); return 1; }
    std::printf("00f3d830_TEST PASS\n");
    return 0;
}