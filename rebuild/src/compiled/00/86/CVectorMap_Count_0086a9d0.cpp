// CVectorMap::Count(const long&) const -- number of elements with the key in the skeletal-morph vector map: UpperBound(key) - LowerBound(key)
// Leaf: ?Count@?$CVectorMap@JVCSkeletalMorph@CRandomAppearanceMorph@@
// Retail 0x0086A9D0, 0x26 bytes, thiscall `ret 4`. Manifest label "CVectorMap_FindEqualRange" is a descriptive auto label;
// Ego_r/FableWin name ?Count@?$CVectorMap@JVCSkeletalMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@JVCSkeletalMorph@CRandomAppearanceMorph@@@@@@QBEJABJ@Z
// and the two callees are this map's UpperBound @ 0x0086A980 and LowerBound @ 0x0086A930 (manifest labels; 16-byte
// pair<long, CRandomAppearanceMorph::CSkeletalMorph> elements, `sar 4`). No direct caller (out-of-line instantiation).
// Shape: UpperBound first (left operand), held in ebx across LowerBound; the pointer difference divides by 16.
// Layout (retail CVectorMap): STLport vector {start, finish, end_of_storage} + empty Compare +0x0C + bool Dirty +0x0D.
class CRandomAppearanceMorph
{
public:
    class CSkeletalMorph
    {
    public:
        long          GroupID;                       // +0x00
        unsigned char _rest[8];                      // +0x04
    };
};

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V> struct SVectorMapPair    // std::pair<K, V>
{
    K first;
    V second;
};

template <class K, class V, class C> class CVectorMap
{
public:
    typedef SVectorMapPair<K, V> value_type;
    value_type* _M_start;                            // +0x00
    value_type* _M_finish;                           // +0x04
    value_type* _M_end_of_storage;                   // +0x08
    C           Compare;                             // +0x0C
    bool        Dirty;                               // +0x0D

    const value_type* LowerBound(const K& key) const;    // 0x0086A930
    const value_type* UpperBound(const K& key) const;    // 0x0086A980
    long Count(const K& key) const;
};

template <class K, class V, class C> long CVectorMap<K, V, C>::Count(const K& key) const
{
    return UpperBound(key) - LowerBound(key);
}

template long CVectorMap<long, CRandomAppearanceMorph::CSkeletalMorph,
    CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> >::Count(const long& key) const;