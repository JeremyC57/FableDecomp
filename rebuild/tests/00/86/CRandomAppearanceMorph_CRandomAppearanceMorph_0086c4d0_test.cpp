// Behaviour test for 0086c4d0: body copied from src/0086c4d0.cpp (CRandomAppearanceMorph ctor), the CArray resize/dtor callees stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_resize2 = 0, g_resize1 = 0, g_tempDtor = 0; static unsigned long g_n2 = 0, g_n1 = 0; static const void* g_resizeThis2 = 0; static const void* g_resizeThis1 = 0;
template <class T> struct CArrayAllocProxy { T* p; CArrayAllocProxy(T* v) : p(v) { } };
template <class T> struct CArray {
    T* m_first;
    T* m_last;
    CArrayAllocProxy<T> m_end;
    CArray() : m_first(0), m_last(0), m_end(0) {}
    ~CArray() { ++g_tempDtor; }
    void resize(unsigned long n, const T& value) { ++g_resize2; g_n2 = n; g_resizeThis2 = this; CHECK(value.m_first == 0 && value.m_last == 0 && value.m_end.p == 0); }
    void resize(unsigned long n) { ++g_resize1; g_n1 = n; g_resizeThis1 = this; }
};
template <class K, class V> struct CKeyPairCompareLess { CKeyPairCompareLess() {} };
template <class K, class V, class C> struct CVectorMap {
    CArray<V> Values;
    C         Compare;
    bool      Dirty;
    CVectorMap() : Dirty(false) {}
};
struct CRandomAppearanceMorph {
    struct CBodyPartMesh { long x; };
    struct CTextureMorph { long x; };
    struct CSkeletalMorph { long x; };
    typedef CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > CTextureMorphMap;
    CArray<CArray<CBodyPartMesh> >                                                      BodyParts;
    CArray<CTextureMorphMap>                                                            BodyPartTextureMorphs;
    CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >        SkeletalMorphs;
    CTextureMorphMap                                                                    GeneralTextureMorphs;
    unsigned long                                                                       MaxTextureGroupID;
    CRandomAppearanceMorph();
};
CRandomAppearanceMorph::CRandomAppearanceMorph()
    : MaxTextureGroupID(0)
{
    BodyParts.resize(3, CArray<CBodyPartMesh>());
    BodyPartTextureMorphs.resize(3);
}
int main()
{
    unsigned char raw[sizeof(CRandomAppearanceMorph)]; memset(raw, 0xAA, sizeof(raw));
    CRandomAppearanceMorph* m = new (raw) CRandomAppearanceMorph();
    CHECK(sizeof(CRandomAppearanceMorph) == 0x3c);
    CHECK(m->BodyParts.m_first == 0 && m->BodyParts.m_last == 0 && m->BodyParts.m_end.p == 0);
    CHECK(m->BodyPartTextureMorphs.m_first == 0 && m->BodyPartTextureMorphs.m_end.p == 0);
    CHECK(m->SkeletalMorphs.Values.m_first == 0 && m->SkeletalMorphs.Values.m_end.p == 0 && m->SkeletalMorphs.Dirty == false);
    CHECK(m->GeneralTextureMorphs.Values.m_last == 0 && m->GeneralTextureMorphs.Dirty == false);
    CHECK(m->MaxTextureGroupID == 0);
    CHECK(g_resize2 == 1 && g_n2 == 3 && g_resizeThis2 == &m->BodyParts);
    CHECK(g_resize1 == 1 && g_n1 == 3 && g_resizeThis1 == &m->BodyPartTextureMorphs);
    CHECK(g_tempDtor == 1);                                   // the by-value CArray temporary is destroyed after the resize
    if (fails == 0) printf("0086c4d0_TEST PASS\n"); else printf("0086c4d0_TEST FAIL\n");
    return fails;
}