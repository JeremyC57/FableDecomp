// behaviour test for CHeroMorphDef::operator= (0x004E6F0B): every member copied, base assignment called, *this returned
#include <cstdio>
static int g_base = 0, g_ops = 0;
struct CDefClassBase { unsigned char _body[0x25]; CDefClassBase& operator=(const CDefClassBase& o) { ++g_base; for (int i = 0; i < 0x25; ++i) _body[i] = o._body[i]; return *this; } };

struct CKeyPairCompareLess {};
struct CTextureMorphEntryVector { void* m_first; void* m_last; void* m_end; CTextureMorphEntryVector& operator=(const CTextureMorphEntryVector& o) { ++g_ops; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; } };
struct CSkeletalMorphEntryVector { void* m_first; void* m_last; void* m_end; CSkeletalMorphEntryVector& operator=(const CSkeletalMorphEntryVector& o) { ++g_ops; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; } };
struct CParticleMorphEntryVector { void* m_first; void* m_last; void* m_end; CParticleMorphEntryVector& operator=(const CParticleMorphEntryVector& o) { ++g_ops; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; } };
template<class V> struct CVectorMap { V m_vector; CKeyPairCompareLess m_compare; bool m_sorted; };
struct CTextureMorphs { CVectorMap<CTextureMorphEntryVector> Morphs; };
struct CSkeletalMorphs { CVectorMap<CSkeletalMorphEntryVector> Morphs; };
struct CParticleMorphs { CVectorMap<CParticleMorphEntryVector> Morphs; };
struct CHeroMorphDef : CDefClassBase {
    CTextureMorphs TextureMorphs;
    CSkeletalMorphs SkeletalMorphs;
    CParticleMorphs ParticleMorphs;
    CParticleMorphs IdleParticleMorphs;
    CHeroMorphDef& operator=(const CHeroMorphDef& other);
};
CHeroMorphDef& CHeroMorphDef::operator=(const CHeroMorphDef& other)
{
    CDefClassBase::operator=(other);
    TextureMorphs = other.TextureMorphs;
    SkeletalMorphs = other.SkeletalMorphs;
    ParticleMorphs = other.ParticleMorphs;
    IdleParticleMorphs = other.IdleParticleMorphs;
    return *this;
}
int main()
{
    CHeroMorphDef a, b;
    unsigned char* pa = (unsigned char*)&a; unsigned char* pb = (unsigned char*)&b;
    for (unsigned int i = 0; i < sizeof(a); ++i) { pa[i] = (unsigned char)(i * 7 + 3); pb[i] = 0; }
    a.TextureMorphs.Morphs.m_sorted = true; a.SkeletalMorphs.Morphs.m_sorted = false; a.ParticleMorphs.Morphs.m_sorted = true; a.IdleParticleMorphs.Morphs.m_sorted = true;
    CHeroMorphDef* r = &(b = a);
    int bad = (r != &b) || g_base != 1 || g_ops != 4;
    if (a.TextureMorphs.Morphs.m_vector.m_first != b.TextureMorphs.Morphs.m_vector.m_first || a.TextureMorphs.Morphs.m_vector.m_end != b.TextureMorphs.Morphs.m_vector.m_end || a.TextureMorphs.Morphs.m_sorted != b.TextureMorphs.Morphs.m_sorted) bad = 1;
    if (a.SkeletalMorphs.Morphs.m_vector.m_first != b.SkeletalMorphs.Morphs.m_vector.m_first || a.SkeletalMorphs.Morphs.m_vector.m_end != b.SkeletalMorphs.Morphs.m_vector.m_end || a.SkeletalMorphs.Morphs.m_sorted != b.SkeletalMorphs.Morphs.m_sorted) bad = 1;
    if (a.ParticleMorphs.Morphs.m_vector.m_first != b.ParticleMorphs.Morphs.m_vector.m_first || a.ParticleMorphs.Morphs.m_vector.m_end != b.ParticleMorphs.Morphs.m_vector.m_end || a.ParticleMorphs.Morphs.m_sorted != b.ParticleMorphs.Morphs.m_sorted) bad = 1;
    if (a.IdleParticleMorphs.Morphs.m_vector.m_first != b.IdleParticleMorphs.Morphs.m_vector.m_first || a.IdleParticleMorphs.Morphs.m_vector.m_end != b.IdleParticleMorphs.Morphs.m_vector.m_end || a.IdleParticleMorphs.Morphs.m_sorted != b.IdleParticleMorphs.Morphs.m_sorted) bad = 1;
    for (int i = 0; i < 0x25; ++i) if (a._body[i] != b._body[i]) bad = 1;
    if (bad) { std::printf("004e6f0b_TEST FAIL\n"); return 1; }
    std::printf("004e6f0b_TEST PASS\n");
    return 0;
}