// Behaviour test: CParticleMorphs::Add -- append the entry keyed by its morph type, mark the map dirty and re-sort it by key
#include <cstdio>

enum EHeroMorphType { MORPH_A = 1, MORPH_B = 2, MORPH_C = 3, MORPH_D = 4 };

struct CEntry {                                         // CParticleMorphs::CEntry (36 bytes)
    EHeroMorphType MorphType;
    bool  Blending;
    long  Location;
    long  LocationName;
    long  LocationIndex;
    long  ParticleIndex;
    long  SecondParticleIndex;
    float StartAtMorphStrength;
    float EndAtMorphStrength;
};
struct SPair {
    EHeroMorphType first;
    CEntry second;
};
struct CKeyPairCompareLess {
    bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; }
};

static void sort(SPair* first, SPair* last, CKeyPairCompareLess comp)   // stand-in for the STLport sort 0x0081E090
{
    for (SPair* i = first + 1; i < last; ++i) {
        SPair v = *i;
        SPair* j = i;
        while (j != first && comp(v, *(j - 1))) {
            *j = *(j - 1);
            --j;
        }
        *j = v;
    }
}

class CParticleMorphs { public:
    SPair storage[8];
    SPair* m_start;
    SPair* m_finish;
    CKeyPairCompareLess Compare;
    bool Dirty;
    void Add(const CEntry& entry)
    {
        SPair p;
        p.first = entry.MorphType;
        p.second = entry;
        *m_finish++ = p;                                 // push_back (capacity is ample here)
        Dirty = true;
        if (Dirty) {
            sort(m_start, m_finish, Compare);
            Dirty = false;
        }
    }
};

static CEntry Entry(EHeroMorphType type, long particle)
{
    CEntry e;
    e.MorphType = type; e.Blending = false; e.Location = 0; e.LocationName = 0; e.LocationIndex = 0;
    e.ParticleIndex = particle; e.SecondParticleIndex = -1; e.StartAtMorphStrength = 0.0f; e.EndAtMorphStrength = 1.0f;
    return e;
}

int main()
{
    CParticleMorphs m;
    m.m_start = m.m_finish = m.storage;
    m.Dirty = false;
    m.Add(Entry(MORPH_C, 30));
    m.Add(Entry(MORPH_A, 10));
    m.Add(Entry(MORPH_D, 40));
    m.Add(Entry(MORPH_B, 20));
    if (m.Dirty || m.m_finish - m.m_start != 4)
        { std::printf("0081b490_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (m.m_start[i].first != EHeroMorphType(i + 1) || m.m_start[i].second.MorphType != EHeroMorphType(i + 1)
            || m.m_start[i].second.ParticleIndex != (i + 1) * 10)
            { std::printf("0081b490_TEST FAIL\n"); return 1; }
    std::printf("0081b490_TEST PASS\n");
    return 0;
}