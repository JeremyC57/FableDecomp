// CVectorMap::LowerBound(const CCharString& key) const -- (CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> >) binary search of the sorted 56-byte dummy-object entries for the first one whose name does not sort before the key
// Leaf: ?LowerBound@?$CVectorMap@VCCharString@@VC3DDummyObject@@V?$CCompareString@VC3DDummyObject@@@@@@QBEPBU?$pair@VCCharString@@VC3DDummyObject@@@std@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6F2F0, 93 bytes, thiscall `ret 4`, the entry pointer (STLport const_iterator) in eax.
// Identity: Ego_r.pdb `?LowerBound@?$CVectorMap@VCCharString@@VC3DDummyObject@@V?$CCompareString@VC3DDummyObject@@@@@@QBE...`
//   (Dinkumware const_iterator there; STLport's is a pointer); its one caller CPhysicsMesh::GetDummyObjectPositionStatic
//   (0x00A6CA00) calls it on DummyObjects (+0x20). Twin of the landed HelperPoints LowerBound 0x00A6F290 (20-byte entries).
// Callees: CCharString::FastCompare(const CCharString&) const @ 0x009E6530 (the CCompareString "less").
// Layout: entry = pair<CCharString, C3DDummyObject> (4 + 52 bytes: Transform CMatrix3x4 +0, BoneIndex +0x30), so the count
//   is (last - first) / 56 (`imul 92492493h ; add edx,ecx ; sar edx,5`) and the middle entry `imul esi,esi,38h`.
class CCharString {
public:
    void* PData;                                         // +0x00
    bool FastCompare(const CCharString& other) const;    // 0x009E6530
};
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject {                                   // 52 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                                // +0x00
    long       BoneIndex;                                // +0x30
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class T> class vector {
public:
    typedef const T* const_iterator;
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}
template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& entry, const CCharString& key) const { return entry.first.FastCompare(key); }
};
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    C    Compare;                                        // +0x0C
    bool Dirty;                                          // +0x0D
    const_iterator LowerBound(const K& key) const;
};
template <class K, class V, class C>
typename CVectorMap<K, V, C>::const_iterator CVectorMap<K, V, C>::LowerBound(const K& key) const
{
    int count = this->end() - this->begin();
    const_iterator first = this->begin();
    while (count > 0)
    {
        int half = count / 2;
        const_iterator middle = first + half;
        if (Compare(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
typedef CVectorMap<CCharString, C3DDummyObject, CCompareString<C3DDummyObject> > CDummyObjectMap;
template CDummyObjectMap::const_iterator CDummyObjectMap::LowerBound(const CCharString& key) const;   // emit just this member