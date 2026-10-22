// Behaviour test for 00f3b970: src/00f3b970.cpp copied verbatim, the growth and assignment helpers stubbed below.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int fails = 0;
static char g_msg[128];
#define CHECK(c) do { if (!(c)) { ++fails; _snprintf(g_msg, sizeof(g_msg), "FAIL %d: %s", __LINE__, #c); puts(g_msg); } } while (0)
// ---- source (verbatim) ----
// CTriangleStripifier2::StripUnusedVerts(CArray<unsigned short>& verts, const CArray<CArray<long> >& vertTriangles, const CArray<bool>& triangleUsed) -- keep only the vertices that belong to at least one used triangle
// Leaf: ?StripUnusedVerts@CTriangleStripifier2@@AAEXAAV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@_N@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3B970, length 244 bytes (+ int3 padding), `ret 0Ch` (`this` unused).
// Callees: the CArray<unsigned short> reallocating insert @ 0x005135D0 (catalog "Vector_InsertN_uint16"), CArray<unsigned
// short>::operator= @ 0x00F3B640 (catalog "std_vector_assign_with_realloc"), free @ 0x00BFEA14 (the kept list's storage).
// Layout: CArray = the retail STLport vector {first, last, end_of_storage} (12 bytes); CArray<bool> is a byte array.
// Shape: the kept list is a local vector (three zero registers); the vertex is an `unsigned long` local (movzx at the
// load), converted to the push_back's const unsigned short& temporary homed in the frame (the growth template gets a
// same-TU `auto_inline(off)` body that only reads its const reference, the landed push_back idiom); the inner loop
// re-indexes vertTriangles[vert] rather than naming it; every size() is recomputed per pass; `break` after the push.
#include <new>
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };
template<class T> struct CArray {                              // retail STLport vector: first, last, end_of_storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CArray() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    CArray& operator=(const CArray& other);                    // 0x00F3B640 (unsigned short instantiation)
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x005135D0
    void push_back(const T& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(value);
            ++_M_finish;
        }
        else
            InsertGrow(_M_finish, value, SScalarTag(), 1, true);
    }
};
void* GrowArrayStorage_005135d0(unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the CArray<unsigned short> growth template 0x005135D0 (retail's TU instantiated it): reads the value
// only through its const reference, never touches the empty tag, keeps the array to itself.
template<class T>
__declspec(noinline) void CArray<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    T* p = (T*)GrowArrayStorage_005135d0(count);
    if (p) {
        *p = value;
        _M_start = p;
        _M_finish = p + 1;
        _M_end_of_storage = p + count;
    }
}
void AssignArrayRange_00f3b640(void* array, const void* first, const void* last);
// Same-TU body of CArray<unsigned short>::operator= 0x00F3B640: reads the source only through its range (the kept list's
// address does not escape, so its `last` can stay in ebx through the loop, as retail's LTCG saw).
template<class T>
__declspec(noinline) CArray<T>& CArray<T>::operator=(const CArray<T>& other)
{
    if (&other != this)
        AssignArrayRange_00f3b640(this, other._M_start, other._M_finish);
    return *this;
}
#pragma auto_inline(on)

class CTriangleStripifier2 {
public:
    void StripUnusedVerts(CArray<unsigned short>& verts, const CArray<CArray<long> >& vertTriangles, const CArray<bool>& triangleUsed);
};

void CTriangleStripifier2::StripUnusedVerts(CArray<unsigned short>& verts, const CArray<CArray<long> >& vertTriangles, const CArray<bool>& triangleUsed)
{
    CArray<unsigned short> kept;
    for (unsigned long i = 0; i < verts.size(); ++i) {
        unsigned long vert = verts[i];
        for (unsigned long j = 0; j < vertTriangles[vert].size(); ++j) {
            if (triangleUsed[vertTriangles[vert][j]]) {
                kept.push_back(vert);
                break;
            }
        }
    }
    verts = kept;
}

// ---- stubs and driver ----
// The stand-in growth keeps one element per block, so the kept sequence is read back block by block.
static unsigned short* g_prev = 0;
static unsigned short g_seq[16];
static int g_seqCount = 0, g_grows = 0, g_assigns = 0;
static const void* g_assignArray = 0;
void* GrowArrayStorage_005135d0(unsigned long count)
{
    CHECK(count == 1);
    ++g_grows;
    if (g_prev && g_seqCount < 16) g_seq[g_seqCount++] = *g_prev;
    g_prev = (unsigned short*)malloc(sizeof(unsigned short) * 4);
    return g_prev;
}
void AssignArrayRange_00f3b640(void* array, const void* first, const void* last)
{
    ++g_assigns;
    g_assignArray = array;
    const unsigned short* f = (const unsigned short*)first;
    const unsigned short* l = (const unsigned short*)last;
    if (l - f == 1 && g_seqCount < 16) g_seq[g_seqCount++] = *f;
    CHECK(f == l || l - f == 1);
}
int main()
{
    static unsigned short vertData[4] = { 5, 2, 7, 0 };
    static long tri5[2] = { 1, 3 }, tri7[2] = { 0, 1 }, tri0[1] = { 2 };
    static bool used[4] = { false, false, true, true };
    CTriangleStripifier2 strip;
    {
        CArray<unsigned short> verts;
        verts._M_start = vertData; verts._M_finish = vertData + 4; verts._M_end_of_storage = vertData + 4;
        CArray<CArray<long> > vertTriangles;
        CArray<long> lists[8];
        lists[5]._M_start = tri5; lists[5]._M_finish = tri5 + 2;
        lists[7]._M_start = tri7; lists[7]._M_finish = tri7 + 2;
        lists[0]._M_start = tri0; lists[0]._M_finish = tri0 + 1;
        vertTriangles._M_start = lists; vertTriangles._M_finish = lists + 8;
        CArray<bool> triangleUsed;
        triangleUsed._M_start = used; triangleUsed._M_finish = used + 4;
        strip.StripUnusedVerts(verts, vertTriangles, triangleUsed);
        CHECK(g_grows == 2 && g_assigns == 1 && g_assignArray == &verts);
        CHECK(g_seqCount == 2 && g_seq[0] == 5 && g_seq[1] == 0);
        // 2) nothing used: no growth, an empty assignment
        used[2] = false; used[3] = false;
        g_grows = 0; g_assigns = 0; g_seqCount = 0; g_prev = 0;
        strip.StripUnusedVerts(verts, vertTriangles, triangleUsed);
        CHECK(g_grows == 0 && g_assigns == 1 && g_seqCount == 0);
        // the arrays here do not own their storage
        verts._M_start = 0; vertTriangles._M_start = 0; triangleUsed._M_start = 0;
        for (int k = 0; k < 8; ++k) lists[k]._M_start = 0;
    }
    if (fails == 0) printf("00f3b970_TEST PASS\n");
    return fails;
}