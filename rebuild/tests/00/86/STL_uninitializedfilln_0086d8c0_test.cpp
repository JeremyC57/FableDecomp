// Behaviour test for 0086d8c0 (_STL::__uninitialized_fill_n over CRandomAppearanceMorph::CBodyPartMesh): n copies of x are copy-constructed into raw storage,
// the returned cursor is first + n, n == 0 constructs nothing, and every copy is balanced by its destructor.
#include <cstdio>
#include <new>
static int g_live = 0;
struct CTextureMorph { unsigned int NewTexture; unsigned int GroupID; };
template <class K, class V> struct pair { K first; V second; };
template <class T> struct vector {              // stands in for the STLport vector copy (0x00673E10)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector(T* f, T* l) : _M_start(f), _M_finish(l), _M_end_of_storage(l) { ++g_live; }
    vector(const vector& o) : _M_start(o._M_start), _M_finish(o._M_finish), _M_end_of_storage(o._M_end_of_storage) { ++g_live; }
    ~vector() { --g_live; }
};
struct CKeyPairCompareLess { };
struct CTextureMorphMap {
    vector<pair<unsigned int, CTextureMorph> > Vec;
    CKeyPairCompareLess Compare;
    bool Dirty;
    CTextureMorphMap(pair<unsigned int, CTextureMorph>* f, pair<unsigned int, CTextureMorph>* l, bool d) : Vec(f, l), Dirty(d) { }
};
struct CBodyPartMesh {
    long MeshIndex;
    long Priority;
    CTextureMorphMap TextureMorphs;
    CBodyPartMesh(long m, long p, pair<unsigned int, CTextureMorph>* f, pair<unsigned int, CTextureMorph>* l) : MeshIndex(m), Priority(p), TextureMorphs(f, l, true) { }
};
typedef CBodyPartMesh T;
static bool Same(const T& a, const T& b)
{
    return a.MeshIndex == b.MeshIndex && a.Priority == b.Priority && a.TextureMorphs.Vec._M_start == b.TextureMorphs.Vec._M_start && a.TextureMorphs.Vec._M_finish == b.TextureMorphs.Vec._M_finish && a.TextureMorphs.Dirty == b.TextureMorphs.Dirty;
}
struct __false_type { __false_type() { } };
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 6]; };
int main()
{
    static pair<unsigned int, CTextureMorph> morphs[2];
    T x(3, 8, morphs, morphs + 2);
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    T* r = __uninitialized_fill_n(items, 0u, x, __false_type());
    if (r != items || g_live != base) { std::printf("0086d8c0_TEST FAIL\n"); return 1; }
    r = __uninitialized_fill_n(items, 5u, x, __false_type());
    if (r != items + 5) { std::printf("0086d8c0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (!Same(items[i], x)) { std::printf("0086d8c0_TEST FAIL\n"); return 1; }
    if (g_live != base + 5 * 1) { std::printf("0086d8c0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        items[i].~T();
    if (g_live != base) { std::printf("0086d8c0_TEST FAIL\n"); return 1; }
    std::printf("0086d8c0_TEST PASS\n");
    return 0;
}