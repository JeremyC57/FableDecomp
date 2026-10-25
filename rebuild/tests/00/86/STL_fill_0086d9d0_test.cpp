// Behaviour test for 0x0086D9D0 _STL::fill(CRandomAppearanceMorph::CBodyPartMesh*, ...): every body-part mesh of the range becomes a copy of the value (index and priority copied, the texture-morph vector through its operator=)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
template <class T> class CVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CVector& operator=(const CVector& other)          // stand-in
    {
        _M_start = other._M_start; _M_finish = other._M_finish; _M_end_of_storage = other._M_end_of_storage;
        ++gAssigns;
        return *this;
    }
};
template <class K, class V> struct CKeyValuePair { K first; V second; };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    CVector<CKeyValuePair<K, V> > Data;
    C                             Compare;
    bool                          Dirty;
};
class CRandomAppearanceMorph {
public:
    class CTextureMorph {
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
    };
    class CBodyPartMesh {
    public:
        long MeshIndex;
        long Priority;
        CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > TextureMorphs;
    };
};
typedef CRandomAppearanceMorph::CBodyPartMesh CMesh;
typedef CKeyValuePair<unsigned long, CRandomAppearanceMorph::CTextureMorph> CMorphEntry;
static CMorphEntry gMorphs[2];
static CMesh gArr[4];
namespace _STL {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
int main()
{
    CMesh v; v.MeshIndex = 7; v.Priority = 2;
    v.TextureMorphs.Data._M_start = gMorphs; v.TextureMorphs.Data._M_finish = gMorphs + 2; v.TextureMorphs.Data._M_end_of_storage = gMorphs + 2;
    v.TextureMorphs.Dirty = true;
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].MeshIndex = -1; gArr[i].Priority = -1; gArr[i].TextureMorphs.Data._M_start = 0; gArr[i].TextureMorphs.Dirty = false; }
    bool ok = true;
    _STL::fill(gArr, gArr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && gArr[i].MeshIndex == 7 && gArr[i].Priority == 2 && gArr[i].TextureMorphs.Data._M_start == gMorphs &&
             gArr[i].TextureMorphs.Data._M_finish == gMorphs + 2 && gArr[i].TextureMorphs.Dirty;
    ok = ok && gArr[3].MeshIndex == -1 && gAssigns == 3;
    _STL::fill(gArr, gArr, v);
    ok = ok && gAssigns == 3;
    if (ok) { printf("0086d9d0_TEST PASS\n"); return 0; }
    printf("0086d9d0_TEST FAIL\n");
    return 1;
}