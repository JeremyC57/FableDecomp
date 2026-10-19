// Grade: DIFFER(189v181) -- the blank CEntry: retail zero-fills all nine dwords from one `xor edx,edx` (then -1 into LocationName) before pushing; `= { 0 }` gets the zero-fill but VC spreads it over eax/edx/imm and interleaves it with the pushes; CEntry() / CEntry() copy-init give no zero-fill at all
// CParticleMorphs::GetAllMorphsOfType(long, vector<CEntry>&) -- copy every particle-morph entry of the given hero morph type into the output vector; false when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081B160, length 192 bytes (`ret 8`; callers 0x0071C7FB / 0x0071CB28).
// Callees: the morph map's lower_bound @ 0x00819E60 and upper_bound @ 0x00819EC0 (landed as
//          CVectorMap_00819ec0::UpperBound_CVectorMap_00819ec0; both take the key by const reference),
//          vector<CParticleMorphs::CEntry>::resize(n, const T&) @ 0x0081DB00 (label propagated).
// Layout (PDB): CParticleMorphs is the CVectorMap<EHeroMorphType, CEntry> Morphs (a 12-byte retail vector of 40-byte
// pairs); CEntry (36 bytes): MorphType, Blending, Location, LocationName (CDefString, default -1), LocationIndex,
// ParticleIndex, SecondParticleIndex, StartAt/EndAtMorphStrength.
// Shape: the matching range is [lower_bound, upper_bound) (count = byte difference / 40); the output is resized
// with a value-initialised CEntry() temporary -- VC7.1 zero-fills all nine dwords and then runs the implicit
// constructor (the CDefString's -1) -- and each entry is copied as nine dwords (rep movsd).
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

struct SEntry {                                    // pair<EHeroMorphType, CEntry>, 40 bytes
    long                 m_key;                    // +0x00
    CParticleMorphsEntry second;                   // +0x04
};

struct CVectorMap_00819ec0 {                       // CVectorMap<EHeroMorphType, CParticleMorphs::CEntry>
    SEntry* m_begin;
    SEntry* m_end;
    SEntry* m_capacity;
    SEntry* LowerBound_CVectorMap_00819e60(const long& key);   // 0x00819E60
    SEntry* UpperBound_CVectorMap_00819ec0(const long& key);   // 0x00819EC0
};

struct CParticleMorphsEntryVector {                // vector<CParticleMorphs::CEntry>
    CParticleMorphsEntry* m_first;
    CParticleMorphsEntry* m_last;
    CParticleMorphsEntry* m_end;
    void resize(unsigned long n, const CParticleMorphsEntry& value);   // 0x0081DB00
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
        CParticleMorphsEntry blank = { 0 };
        out.resize(count, blank);
        for (long i = 0; i < count; ++i)
            out[i] = first[i].second;
        return true;
    }
    return false;
}
