// CRandomAppearanceMorph::GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& set) -- pick a random skeletal morph entry per morph group
// Grade: DIFFER(ours v retail) -- 389v389: register allocation -- retail this in ebp, the map in ebx, `end` spilled to the frame
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
template<class T> struct CArray {
    T* m_first; T* m_last; T* m_end;
    CArray() : m_first(0), m_last(0), m_end(0) {}
    ~CArray() { if (m_first) free(m_first); }
};
struct CKeyPairCompareLess {};
struct CMorphSet;
struct CRandomAppearanceMorph {
    struct CSkeletalMorph {
        unsigned char Data[8];                     // +0x00 (entry +0x04)
        bool          AlwaysApply;                 // +0x08 (entry +0x0c)
    };
    struct CSkeletalMorphMap {                     // CVectorMap<long, CSkeletalMorph>
        struct SEntry { long Key; CSkeletalMorph Value; };
        CArray<SEntry> m_vector;
        CKeyPairCompareLess m_compare;
        bool m_sorted;
        void GetKeyList(CArray<long>* keys);       // 0x0086D240
        SEntry* UpperBound(const long& key) const; // 0x0086A980
        SEntry* LowerBound(const long& key) const; // 0x0086A930
    };
    unsigned char _pad00[0x18];
    CSkeletalMorphMap SkeletalMorphs;              // +0x18
    void CreateRandomMorphEntry(unsigned long& seed, const CSkeletalMorph& morph, CMorphSet& set);   // 0x0086B900
    void GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& set);
};

void CRandomAppearanceMorph::GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& set)
{
    CArray<long> keys;
    SkeletalMorphs.GetKeyList(&keys);
    for (long* k = keys.m_first; k != keys.m_last; ++k) {
        long key = *k;
        if (key == 0) {
            CSkeletalMorphMap::SEntry* end = SkeletalMorphs.UpperBound(key);
            for (CSkeletalMorphMap::SEntry* e = SkeletalMorphs.LowerBound(key); e != end; ++e) {
                if (!e->Value.AlwaysApply) {
                    seed = seed * 0x24A1 + 0x24DF;
                    seed = GFROR13(seed);
                    if (seed % 100 < 50)
                        continue;
                }
                CreateRandomMorphEntry(seed, e->Value, set);
            }
        } else {
            seed = seed * 0x24A1 + 0x24DF;
            seed = GFROR13(seed);
            if (seed % 100 > 50) {
                unsigned long count = SkeletalMorphs.UpperBound(key) - SkeletalMorphs.LowerBound(key);
                seed = seed * 0x24A1 + 0x24DF;
                seed = GFROR13(seed);
                unsigned long index = count ? seed % count : 0;
                CreateRandomMorphEntry(seed, SkeletalMorphs.LowerBound(key)[index].Value, set);
            }
        }
    }
}
