// CPreAllocVectorMap<unsigned long,C3DDummyObject,CKeyPairCompareLess<unsigned long,C3DDummyObject> >::UpperBound(const unsigned long&) const -- binary search for the first element whose key is greater than `key`
// Leaf: UpperBound
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A317E0, 66 bytes, `__fastcall`, `ret 4`; no callees.
// Layout (Ego_r.pdb, 16 bytes): Compare (+0x0, empty CKeyPairCompareLess), Elements (+0x4, pair<unsigned long,C3DDummyObject>*,
//   0x38-byte elements), Size (+0x8, unsigned long), Dirty (+0xC).
// Shape: the SGI lower/upper_bound len/half walk over the pre-allocated array: the count is taken as a signed
//   `long` (`cdq ; sub ; sar` halving, `jle`/`jg` loop tests), the key is read once before the rotated loop, the
//   compare is `cmp key,[mid] ; jb` (key < mid->first keeps the left half), and the right step is `len = len - half - 1` (`or reg,-1 ; sub ; add`).
template <class K, class V> struct pair {
    K first;
    V second;
};

struct CMatrix3x4 { float M[12]; };
struct C3DDummyObject {                 // Ego_r.pdb, 52 bytes
    CMatrix3x4 Transform;               // +0x00
    long       BoneIndex;               // +0x30
};

template <class K, class V> struct CKeyPairCompareLess { };

template <class K, class V, class C> struct CPreAllocVectorMap {
    C              Compare;             // +0x00
    pair<K, V>*    Elements;            // +0x04
    unsigned long  Size;                // +0x08
    bool           Dirty;               // +0x0C
    pair<K, V>* UpperBound(const K& key) const;
};

template <class K, class V, class C> pair<K, V>* CPreAllocVectorMap<K, V, C>::UpperBound(const K& key) const
{
    pair<K, V>* first = Elements;
    long len = Size;
    while (len > 0) {
        long half = len / 2;
        pair<K, V>* mid = first + half;
        if (!(key < mid->first)) {
            first = mid + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}

template pair<unsigned long, C3DDummyObject>* CPreAllocVectorMap<unsigned long, C3DDummyObject, CKeyPairCompareLess<unsigned long, C3DDummyObject> >::UpperBound(const unsigned long& key) const;