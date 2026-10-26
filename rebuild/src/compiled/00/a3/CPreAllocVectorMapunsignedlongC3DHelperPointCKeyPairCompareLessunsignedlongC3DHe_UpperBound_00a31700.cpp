// CPreAllocVectorMap<unsigned long,C3DHelperPoint,CKeyPairCompareLess<unsigned long,C3DHelperPoint> >::UpperBound(const unsigned long&) const -- binary search for the first element whose key is greater than `key`
// Leaf: UpperBound
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A31700, 65 bytes, `__fastcall`, `ret 4`; no callees.
// Layout (Ego_r.pdb, 16 bytes): Compare (+0x0, empty CKeyPairCompareLess), Elements (+0x4, pair<unsigned long,C3DHelperPoint>*,
//   0x14-byte elements), Size (+0x8, unsigned long), Dirty (+0xC).
// Shape: the SGI lower/upper_bound len/half walk over the pre-allocated array: the count is taken as a signed
//   `long` (`cdq ; sub ; sar` halving, `jle`/`jg` loop tests), the key is read once before the rotated loop, the
//   compare is `cmp key,[mid] ; jb` (key < mid->first keeps the left half), and the right step is `len = len - half - 1` (`or reg,-1 ; sub ; add`).
//   Declaring `len` before `first` gives retail's registers (len in esi, first in edi); with `first` declared first VC7.1
//   swaps them for this 0x14-byte element (the 0x38-byte twins 0x00A31790/0x00A317E0 match either way round).
template <class K, class V> struct pair {
    K first;
    V second;
};

class C3DVector { public: float X, Y, Z; };
class C3DHelperPoint { public:                 // Ego_r.pdb, 16 bytes
    C3DVector Position;                 // +0x00
    long      BoneIndex;                // +0x0C
};

template <class K, class V> class CKeyPairCompareLess { public: };

template <class K, class V, class C> struct CPreAllocVectorMap {
    C              Compare;             // +0x00
    pair<K, V>*    Elements;            // +0x04
    unsigned long  Size;                // +0x08
    bool           Dirty;               // +0x0C
    pair<K, V>* UpperBound(const K& key) const;
};

template <class K, class V, class C> pair<K, V>* CPreAllocVectorMap<K, V, C>::UpperBound(const K& key) const
{
    long len = Size;
    pair<K, V>* first = Elements;
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

template pair<unsigned long, C3DHelperPoint>* CPreAllocVectorMap<unsigned long, C3DHelperPoint, CKeyPairCompareLess<unsigned long, C3DHelperPoint> >::UpperBound(const unsigned long& key) const;