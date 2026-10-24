// Behaviour test for 0x00674D80 std::__copy<CBodyPartMesh*, CBodyPartMesh*, int>: last - first meshes are assigned
// forward (both longs, the map's entry vector through its operator=, the Dirty byte), the result end is returned, and
// an empty range returns result untouched.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

static int g_vectorAssigns = 0;

struct CTextureMorph { unsigned int NewTexture; unsigned int GroupID; };

namespace nav {
struct random_access_iterator_tag { };
template <class _T1, class _T2> class pair { public: _T1 first; _T2 second; };
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector<_Tp>& operator=(const vector<_Tp>& x)            // models 0x006741E0 (pointer copy here)
    {
        ++g_vectorAssigns;
        _M_start = x._M_start; _M_finish = x._M_finish; _M_end_of_storage = x._M_end_of_storage;
        return *this;
    }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    nav::vector<nav::pair<K, V> > Entries;
    C    Compare;
    bool Dirty;
};

class CBodyPartMesh {
public:
    int MeshIndex;
    int Priority;
    CVectorMap<unsigned int, CTextureMorph, CKeyPairCompareLess<unsigned int, CTextureMorph> > TextureMorphs;
};

int main()
{
    typedef nav::pair<unsigned int, CTextureMorph> Entry;
    Entry blocks[3];
    CBodyPartMesh src[3], dst[4];
    for (int i = 0; i < 3; ++i) {
        src[i].MeshIndex = 10 + i; src[i].Priority = 20 + i;
        src[i].TextureMorphs.Entries._M_start = blocks + i;
        src[i].TextureMorphs.Entries._M_finish = blocks + i;
        src[i].TextureMorphs.Entries._M_end_of_storage = blocks + i;
        src[i].TextureMorphs.Dirty = (i == 1);
    }
    for (int i = 0; i < 4; ++i) {
        dst[i].MeshIndex = -1; dst[i].Priority = -1; dst[i].TextureMorphs.Dirty = true;
        dst[i].TextureMorphs.Entries._M_start = dst[i].TextureMorphs.Entries._M_finish = 0;
        dst[i].TextureMorphs.Entries._M_end_of_storage = 0;
    }
    CBodyPartMesh* end = nav::__copy(src, src + 3, dst, nav::random_access_iterator_tag(), (int*)0);
    if (end != dst + 3 || g_vectorAssigns != 3 || dst[3].MeshIndex != -1) { std::printf("00674d80_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (dst[i].MeshIndex != 10 + i || dst[i].Priority != 20 + i || dst[i].TextureMorphs.Entries._M_start != blocks + i ||
            dst[i].TextureMorphs.Dirty != (i == 1)) {
            std::printf("00674d80_TEST FAIL\n"); return 1;
        }
    CBodyPartMesh* same = nav::__copy(src, src, dst + 1, nav::random_access_iterator_tag(), (int*)0);
    if (same != dst + 1 || g_vectorAssigns != 3) { std::printf("00674d80_TEST FAIL\n"); return 1; }
    std::printf("00674d80_TEST PASS\n");
    return 0;
}