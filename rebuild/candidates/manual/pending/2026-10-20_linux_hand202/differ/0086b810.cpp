// CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long&, unsigned long, const CVectorMap<unsigned long, CTextureMorph>&, CArray<CMorphSet::CMorphSetTextureMorph>&) const -- for each original texture in the morph map, pick one of its morphs that is ungrouped or in the requested group at random and append (texture, new texture) to the result
// Grade: DIFFER(238v230) -- the push_back block only: retail takes the result vector into esi, loads the entry key before its new texture and copies both into the free slot from the same registers, the grow call's tag sits in the live morphs argument slot; ours keeps the vector in edx, loads the texture first, reloads the second word from the frame and homes the tag in the groupID slot (both loops match); tried named local / by-value and by-reference record ctors / placement copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086B810, length 230 bytes, `ret 10h` (`this` unused).
// Leaf: GetRandomTextureMorphs
// Called by the landed GetRandomMorphSet (0x0086BF70) for the general and per-body-part texture morphs.
// Callees: CVectorMap<unsigned long, CTextureMorph>::UpperBound 0x0086A8D0 (the end of one key's run), GFROR13
// 0x00497890 (the engine LCG step: seed = seed * 0x24A1 + 0x24DF, then rotated), the result vector's grow path
// 0x0086CA20 (STLport _M_insert_overflow(pos, value, __true_type, 1, true)).
// Layout: map entries are 12 bytes {original texture, CTextureMorph {NewTexture, GroupID}}; the result entries 8 bytes
// {OriginalTexture, NewTexture}; retail vectors are {first, last, end}.
// Shape: the run [it, UpperBound(it->key)) is counted, a random index below the count chosen (`div`), and the run walked
// again to that entry; the record is a named local pushed through the inline push_back (placement copy when there is
// room, else the grow call with an empty tag temporary); the outer iterator jumps to the run's end.
#include <new>
struct SScalarTag { SScalarTag() {} };
template<class T> struct CArray {
    T* m_first; T* m_last; T* m_end;
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x0086CA20 (T = CMorphSetTextureMorph)
    void push_back(const T& value)
    {
        if (m_last != m_end) {
            new (m_last) T(value);
            ++m_last;
        }
        else
            InsertGrow(m_last, value, SScalarTag(), 1, true);
    }
};
struct CKeyPairCompareLess {};
template<class K, class V> struct CVectorMap {
    struct SEntry { K Key; V Value; };
    CArray<SEntry> m_vector; CKeyPairCompareLess m_compare; bool m_sorted;
    SEntry* begin() const { return m_vector.m_first; }
    SEntry* end() const { return m_vector.m_last; }
    SEntry* UpperBound(const K& key) const;              // 0x0086A8D0 (K = unsigned long)
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890

struct CMorphSet {
    struct CMorphSetTextureMorph {
        unsigned long OriginalTexture;                   // +0x00
        unsigned long NewTexture;                        // +0x04
        CMorphSetTextureMorph(unsigned long original, unsigned long newTexture) : OriginalTexture(original), NewTexture(newTexture) {}
        CMorphSetTextureMorph(const CMorphSetTextureMorph& o) : OriginalTexture(o.OriginalTexture), NewTexture(o.NewTexture) {}
    };
};

struct CRandomAppearanceMorph {
    struct CTextureMorph {
        unsigned long NewTexture;                        // +0x00
        unsigned long GroupID;                           // +0x04
    };
    typedef CVectorMap<unsigned long, CTextureMorph> CTextureMorphMap;
    void GetRandomTextureMorphs(unsigned long& seed, unsigned long groupID, const CTextureMorphMap& morphs,
                                CArray<CMorphSet::CMorphSetTextureMorph>& result) const;
};

void CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long& seed, unsigned long groupID, const CTextureMorphMap& morphs,
                                                    CArray<CMorphSet::CMorphSetTextureMorph>& result) const
{
    CTextureMorphMap::SEntry* it = morphs.begin();
    while (it != morphs.end()) {
        CTextureMorphMap::SEntry* runEnd = morphs.UpperBound(it->Key);
        unsigned long count = 0;
        CTextureMorphMap::SEntry* e;
        for (e = it; e != runEnd; ++e)
            if (e->Value.GroupID == 0 || e->Value.GroupID == groupID)
                ++count;
        if (count) {
            seed = seed * 0x24A1 + 0x24DF;
            seed = GFROR13(seed);
            unsigned long pick = seed % count;
            unsigned long index = 0;
            for (e = it; e != runEnd; ++e) {
                if (e->Value.GroupID == 0 || e->Value.GroupID == groupID) {
                    if (index == pick) {
                        result.push_back(CMorphSet::CMorphSetTextureMorph(e->Key, e->Value.NewTexture));
                        break;
                    }
                    ++index;
                }
            }
        }
        it = runEnd;
    }
}
