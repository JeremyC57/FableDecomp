// Behaviour test for 00709ce0 (_STL::__uninitialized_fill_n over pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet>): n copies of x are copy-constructed into raw storage,
// the returned cursor is first + n, n == 0 constructs nothing, and every copy is balanced by its destructor.
#include <cstdio>
#include <new>
static int g_live = 0;
struct CMeshInfo {
    long          Index;
    float         SizeX;
    unsigned char PrimaryMesh : 1;
};
struct CEngineTextureMorph { int v; };
struct CEngineTextureMorphArray {               // stands in for the STLport vector copy (0x005BBDF1)
    CEngineTextureMorph* _M_start;
    CEngineTextureMorph* _M_finish;
    CEngineTextureMorph* _M_end_of_storage;
    CEngineTextureMorphArray(CEngineTextureMorph* f, CEngineTextureMorph* l) : _M_start(f), _M_finish(l), _M_end_of_storage(l) { ++g_live; }
    CEngineTextureMorphArray(const CEngineTextureMorphArray& o) : _M_start(o._M_start), _M_finish(o._M_finish), _M_end_of_storage(o._M_end_of_storage) { ++g_live; }
    ~CEngineTextureMorphArray() { --g_live; }
};
struct CEngineTextureMorphSet {
    CEngineTextureMorphArray Morphs;
    CEngineTextureMorphSet(CEngineTextureMorph* f, CEngineTextureMorph* l) : Morphs(f, l) { }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
typedef pair<CMeshInfo, CEngineTextureMorphSet> T;
static bool Same(const T& a, const T& b)
{
    return a.first.Index == b.first.Index && a.first.SizeX == b.first.SizeX && a.first.PrimaryMesh == b.first.PrimaryMesh && a.second.Morphs._M_start == b.second.Morphs._M_start && a.second.Morphs._M_finish == b.second.Morphs._M_finish;
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
    static CEngineTextureMorph morphs[3];
    CMeshInfo info;
    info.Index = 6; info.SizeX = 2.5f; info.PrimaryMesh = 1;
    T x(info, CEngineTextureMorphSet(morphs, morphs + 3));
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    T* r = __uninitialized_fill_n(items, 0u, x, __false_type());
    if (r != items || g_live != base) { std::printf("00709ce0_TEST FAIL\n"); return 1; }
    r = __uninitialized_fill_n(items, 5u, x, __false_type());
    if (r != items + 5) { std::printf("00709ce0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (!Same(items[i], x)) { std::printf("00709ce0_TEST FAIL\n"); return 1; }
    if (g_live != base + 5 * 1) { std::printf("00709ce0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        items[i].~T();
    if (g_live != base) { std::printf("00709ce0_TEST FAIL\n"); return 1; }
    std::printf("00709ce0_TEST PASS\n");
    return 0;
}