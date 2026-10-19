#include <stdio.h>
#include <string.h>
// Behaviour test for CParticleMorphs::GetAllMorphsOfType (0x0081B160).
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) {}
};

struct CParticleMorphsEntry {                      // CParticleMorphs::CEntry, 36 bytes
    long       MorphType;                          // +0x00 EHeroMorphType
    long       Blending;                           // +0x04 bool (dword-filled by the zero-initialisation)
    long       Location;                           // +0x08 EMeshLocationPoint
    CDefString LocationName;                       // +0x0c
    long       LocationIndex;                      // +0x10
    long       ParticleIndex;                      // +0x14
    long       SecondParticleIndex;                // +0x18
    float      StartAtMorphStrength;               // +0x1c
    float      EndAtMorphStrength;                 // +0x20
};

struct CBlankEntry {
    CParticleMorphsEntry Entry;
    CBlankEntry() : Entry() { }
};

struct SEntry {                                    // pair<EHeroMorphType, CEntry>, 40 bytes
    long                 m_key;                    // +0x00
    CParticleMorphsEntry second;                   // +0x04
};

struct CVectorMap_00819ec0 {                       // CVectorMap<EHeroMorphType, CParticleMorphs::CEntry>
    SEntry* m_begin;
    SEntry* m_end;
    SEntry* m_capacity;
    SEntry* LowerBound_CVectorMap_00819e60(const long& key) { SEntry* p = m_begin; while (p != m_end && p->m_key < key) ++p; return p; }
    SEntry* UpperBound_CVectorMap_00819ec0(const long& key) { SEntry* p = m_begin; while (p != m_end && !(key < p->m_key)) ++p; return p; }
};

struct CParticleMorphsEntryVector {                // vector<CParticleMorphs::CEntry>
    CParticleMorphsEntry* m_first;
    CParticleMorphsEntry* m_last;
    CParticleMorphsEntry* m_end;
    CParticleMorphsEntry store[8]; CParticleMorphsEntry lastValue;
    void resize(unsigned long n, const CParticleMorphsEntry& value) { m_first = store; m_last = store + n; for (unsigned long i = 0; i < n; ++i) store[i] = value; lastValue = value; }
    CParticleMorphsEntry& operator[](long i) { return m_first[i]; }
};

struct CParticleMorphs {
    CVectorMap_00819ec0 Morphs;                    // +0x00
    bool GetAllMorphsOfType(long type, CParticleMorphsEntryVector& out);
};

bool CParticleMorphs::GetAllMorphsOfType(long type, CParticleMorphsEntryVector& out)
{
    SEntry* first = Morphs.LowerBound_CVectorMap_00819e60(type);
    SEntry* last = Morphs.UpperBound_CVectorMap_00819ec0(type);
    if (first != last) {
        long count = last - first;
        out.resize(count, CBlankEntry().Entry);
        for (long i = 0; i < count; ++i)
            out[i] = first[i].second;
        return true;
    }
    return false;
}

int main()
{
    int ok = 1;
    SEntry e[4];
    memset(e, 0x55, sizeof(e));
    e[0].m_key = 1; e[1].m_key = 3; e[2].m_key = 3; e[3].m_key = 7;
    e[1].second.ParticleIndex = 11; e[2].second.ParticleIndex = 22;
    CParticleMorphs pm; pm.Morphs.m_begin = e; pm.Morphs.m_end = e + 4;
    CParticleMorphsEntryVector out; memset(&out, 0, sizeof(out));
    if (pm.GetAllMorphsOfType(5, out) || out.m_first != 0) ok = 0;
    if (!pm.GetAllMorphsOfType(3, out) || out.m_last - out.m_first != 2) ok = 0;
    if (out.m_first[0].ParticleIndex != 11 || out.m_first[1].ParticleIndex != 22 || out.m_first[1].MorphType != 0x55555555) ok = 0;
    if (out.lastValue.MorphType != 0 || out.lastValue.LocationName.TablePos != -1 || out.lastValue.ParticleIndex != 0 || out.lastValue.EndAtMorphStrength != 0.0f) ok = 0;
    if (ok) { printf("0081b160_TEST PASS\n"); return 0; }
    printf("0081b160_TEST FAIL\n"); return 1;
}