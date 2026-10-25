// Behaviour test for 0x0086D980 _STL::fill(CVectorMap<unsigned long, CTextureMorph, ...>*, ...): every texture-morph vector-map of the range becomes a copy of the value (the data vector through its operator=, then the dirty flag)
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
};
typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > CTextureMorphMap;
typedef CKeyValuePair<unsigned long, CRandomAppearanceMorph::CTextureMorph> CMorphEntry;
static CMorphEntry gMorphs[3];
static CTextureMorphMap gArr[4];
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
    CTextureMorphMap v;
    v.Data._M_start = gMorphs; v.Data._M_finish = gMorphs + 3; v.Data._M_end_of_storage = gMorphs + 3; v.Dirty = true;
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].Data._M_start = 0; gArr[i].Data._M_finish = 0; gArr[i].Dirty = false; }
    bool ok = true;
    _STL::fill(gArr, gArr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && gArr[i].Data._M_start == gMorphs && gArr[i].Data._M_finish == gMorphs + 3 && gArr[i].Dirty;
    ok = ok && gArr[3].Data._M_start == 0 && !gArr[3].Dirty && gAssigns == 3;
    _STL::fill(gArr + 1, gArr + 1, v);
    ok = ok && gAssigns == 3;
    if (ok) { printf("0086d980_TEST PASS\n"); return 0; }
    printf("0086d980_TEST FAIL\n");
    return 1;
}