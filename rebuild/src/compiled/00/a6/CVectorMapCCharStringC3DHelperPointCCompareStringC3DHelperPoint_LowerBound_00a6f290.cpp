// CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> >::LowerBound(const CCharString& key) const --
// binary search of CPhysicsMesh::HelperPoints (sorted 20-byte pair<CCharString,C3DHelperPoint> entries) for the first
// entry whose name does not sort before the key
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6F290, 96 bytes, thiscall `ret 4`, the entry pointer (STLport const_iterator) in eax.
// Callers (all on CPhysicsMesh +0x10 = HelperPoints, PDB +16 behind ReferenceCount +8 / NavigationLineListInfo +0xC):
//   0x00A6C9C0 (`add ecx,10h`, CPhysicsMesh::GetHelperPointPositionStatic per chunk 421),
//   CPhysicsMesh::CalculateNumberOfNavigationLineLevels 0x00A73DB0 (`lea ebp,[ecx+10h]`), and 0x00A748B0.
// Name: Ego_r.pdb 0x00B4F550 / FableWin.pdb 0x030F9690 ?LowerBound@?$CVectorMap@VCCharString@@VC3DHelperPoint@@V?$CCompare
//   String@VC3DHelperPoint@@@@@@QBE...@ABVCCharString@@@Z (Dinkumware const_iterator there; STLport's is a pointer). The
//   catalog's CNavLineMapValue is in no PDB name set or layout: CPhysicsMesh's 20-byte CCharString maps are HelperPoints
//   (C3DHelperPoint: Position +0, BoneIndex +0xC) and DummyObjects (+0x20, not a caller here).
// Callees: CCharString::FastCompare(const CCharString&) const @ 0x009E6530 (the CCompareString "less", called on the
//   middle entry's key with the caller's key reference re-pushed every iteration).
// Body: Dinkumware-style lower bound -- count = (last - first) / sizeof(entry); halve, compare the middle entry,
//   step past it (`count -= half + 1`, emitted `or -1 ; sub ; add`) or keep the lower half.
// Order: the count is computed before `first` is copied from begin() (the other order loads it straight into ebp
//   and drops retail's `mov ebp,esi`).
// Layout: CVectorMap derives from the STLport vector {_M_start, _M_finish, _M_end_of_storage}; Compare +0xC, Dirty +0xD.
// Leaf: ?LowerBound@?$CVectorMap@VCCharString@@VC3DHelperPoint@@V?$CCompareString@VC3DHelperPoint@@@@@@QBEPBU?$pair@VCCharString@@VC3DHelperPoint@@@std@@ABVCCharString@@@Z
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};
class CCharString {
public:
    CBasicString<char>* PData;
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};
class C3DVector {
public:
    float X, Y, Z;
};
class C3DHelperPoint {                                   // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                  // +0x00
    long      BoneIndex;                                 // +0x0C
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
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
typedef CVectorMap<CCharString, C3DHelperPoint, CCompareString<C3DHelperPoint> > CHelperPointMap;
template CHelperPointMap::const_iterator CHelperPointMap::LowerBound(const CCharString& key) const;   // emit just this member
