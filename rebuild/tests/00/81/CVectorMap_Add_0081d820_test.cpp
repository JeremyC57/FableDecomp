// Behaviour test: CVectorMap<EHeroMorphType, CParticleMorphs::CEntry>::Add -- append the (key, entry) pair and mark the map unsorted
#include <cstdio>

enum EHeroMorphType { MORPH_A = 1, MORPH_B = 2, MORPH_C = 3 };

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

struct CVectorMap {
    SPair storage[8];
    SPair* m_start;
    SPair* m_finish;
    SPair* m_end;
    bool  Dirty;
    int   overflowCalls;
    void Reset() { m_start = m_finish = storage; m_end = storage + 2; Dirty = false; overflowCalls = 0; }
    void InsertOverflow(const SPair& x)                 // stand-in for _M_insert_overflow 0x0081C990 (grows the capacity)
    {
        ++overflowCalls;
        m_end = storage + 8;
        *m_finish++ = x;
    }
    void push_back(const SPair& x)
    {
        if (m_finish != m_end)
            *m_finish++ = x;
        else
            InsertOverflow(x);
    }
    void Add(const EHeroMorphType& key, const CEntry& value)
    {
        SPair p;
        p.first = key;
        p.second = value;
        push_back(p);
        Dirty = true;
    }
};

int main()
{
    CVectorMap m;
    m.Reset();
    CEntry e;
    e.MorphType = MORPH_B; e.Blending = true; e.Location = 7; e.LocationName = 11; e.LocationIndex = 2;
    e.ParticleIndex = 5; e.SecondParticleIndex = 6; e.StartAtMorphStrength = 0.25f; e.EndAtMorphStrength = 0.75f;
    EHeroMorphType k1 = MORPH_C, k2 = MORPH_A, k3 = MORPH_B;
    m.Add(k1, e);
    m.Add(k2, e);
    if (m.overflowCalls != 0 || !m.Dirty || m.m_finish - m.m_start != 2)
        { std::printf("0081d820_TEST FAIL\n"); return 1; }
    m.Add(k3, e);                                        // capacity reached: the overflow insert
    if (m.overflowCalls != 1 || m.m_finish - m.m_start != 3 || m.m_start[0].first != MORPH_C || m.m_start[1].first != MORPH_A
        || m.m_start[2].first != MORPH_B || m.m_start[2].second.ParticleIndex != 5 || m.m_start[2].second.EndAtMorphStrength != 0.75f)
        { std::printf("0081d820_TEST FAIL\n"); return 1; }
    std::printf("0081d820_TEST PASS\n");
    return 0;
}