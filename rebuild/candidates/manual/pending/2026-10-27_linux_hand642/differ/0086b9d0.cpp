// CRandomAppearanceMorph::GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& morphSet) const -- for every skeletal morph group key: group 0 applies each always-apply entry and each other entry with a 50% roll; any other group, on a roll over 50, applies one entry chosen at random from the group
// Grade: DIFFER(389v389) -- register allocation: retail this in ebp (spilled at entry, restored at the loop bottom), the map in ebx, the inner `end` spilled to [esp+18h]; ours this in ebx (restored at a rotated loop top), the map in ebp, end in ebx. This wave: PDB respell (const protected GetRandomSkeletalMorph, std::vector<long>& GetKeyList, const UpperBound/LowerBound, AddSkeletalMorphToMorphSet const 0x0086B900, GFROR13 returning long) (=); the Ego_r GFRandom(range, &seed) inline for the three rolls (=, byte-identical, kept); a POD const_iterator object for end/e (worse 438). Earlier: named map reference local (=), split inner-loop init (=).
// Leaf: ?GetRandomSkeletalMorph@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@@Z
// Callees: CVectorMap<long,CSkeletalMorph>::GetKeyList 0x0086D240, UpperBound 0x0086A980, LowerBound 0x0086A930 (Ego_r spellings return
//   _Vector_const_iterator; retail STLport returns the element pointer in eax), CRandomAppearanceMorph::AddSkeletalMorphToMorphSet 0x0086B900,
//   GFROR13 0x00497890, free 0x00BFEA14 (vector dtor). Retail: 0x0086B9D0, 389 bytes, thiscall ret 8.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
long __fastcall GFROR13(unsigned long value);  // 0x00497890 (Ego_r ?GFROR13@@YIJK@Z)
// GFRandom (Ego_r ?GFRandom@@YIKKPAK@Z, out of line in Ego_r, expanded in retail): one step of the seeded generator, then
// the result modulo range (0 for an empty range).
inline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    struct value_type { K first; V second; };
    typedef const value_type* const_iterator;
    value_type* m_first; value_type* m_last; value_type* m_end;
    C    m_compare;
    bool m_sorted;
    void GetKeyList(std::vector<K>& keys) const;    // 0x0086D240
    const_iterator UpperBound(const K& key) const;  // 0x0086A980
    const_iterator LowerBound(const K& key) const;  // 0x0086A930
};
class CRandomAppearanceMorph {
public:
    class CMorphSet;
    class CSkeletalMorph { public:
        unsigned char Data[8];                     // +0x00 (entry +0x04)
        bool          AlwaysApply;                 // +0x08 (entry +0x0c)
    };
    unsigned char _pad00[0x18];
    CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> > SkeletalMorphs;   // +0x18
protected:
    void AddSkeletalMorphToMorphSet(unsigned long& seed, const CSkeletalMorph& morph, CMorphSet& morphSet) const;   // 0x0086B900
    void GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& morphSet) const;
};

void CRandomAppearanceMorph::GetRandomSkeletalMorph(unsigned long& seed, CMorphSet& morphSet) const
{
    std::vector<long> keys;
    SkeletalMorphs.GetKeyList(keys);
    for (long* k = keys.begin(); k != keys.end(); ++k) {
        long key = *k;
        if (key == 0) {
            const CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >::value_type* end = SkeletalMorphs.UpperBound(key);
            for (const CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >::value_type* e = SkeletalMorphs.LowerBound(key); e != end; ++e) {
                if (!e->second.AlwaysApply && GFRandom(100, &seed) < 50)
                    continue;
                AddSkeletalMorphToMorphSet(seed, e->second, morphSet);
            }
        } else {
            if (GFRandom(100, &seed) > 50) {
                unsigned long count = SkeletalMorphs.UpperBound(key) - SkeletalMorphs.LowerBound(key);
                unsigned long index = GFRandom(count, &seed);
                AddSkeletalMorphToMorphSet(seed, SkeletalMorphs.LowerBound(key)[index].second, morphSet);
            }
        }
    }
}
