// std::__pop_heap(pair<long,CSoundBankRecord>* first, last, result, value, CKeyPairCompareLess<long,CSoundBankRecord>, int*) -- move the heap top to result, then sift value down from the root
// Leaf: ??$__pop_heap@PAU?$pair@JVCSoundBankRecord@@@std@@U12@V?$CKeyPairCompareLess@JVCSoundBankRecord@@@@H@std@@YIXPAU?$pair@JVCSoundBankRecord@@@0@00U10@V?$CKeyPairCompareLess@JVCSoundBankRecord@@@@PAH@Z
// Flags: /Gr
// Retail: Fable.exe @ 0x00992DB0, 98 bytes, __fastcall (first ecx, last edx; result, the 36-byte value, the empty comparator
// and the distance-type pointer on the stack), `ret 30h`.
// Identity (proven by shape and callees): STLport 4.6 __pop_heap is `*result = *first; __adjust_heap(first, 0, last - first,
// value, comp);` -- this body exactly (`/ 36` through the 0x38E38E39 multiply, hole 0 in edx), over the 0x24-byte
// pair<long, record> elements of the landed __adjust_heap 0x00992B80 / __make_heap 0x00992E20 (the CASoundManager bank
// vector map's sort). Callers 0x00992F28 / 0x00992FD8 are the sort-heap / partial-sort loops of the same sort.
// Catalog "_gapscan::sub_00992db0".
// Callees: the record's operator= 0x00992630 (implicit pair assignment), its copy constructor 0x009922F0 (rebuilding the
// by-value argument), __adjust_heap 0x00992B80 (__fastcall), the record's destructor 0x00992510 (the callee-destroyed value).
// Layout: the record is the landed 0x20-byte CSoundBankRecord (descriptive: the sound module is absent from the Ego PDB);
// only its out-of-line members are needed here.
// Shape: key store then the record's operator= for `*result = *first`; the outgoing value is built in place (key, then the
// out-of-line record copy); finally the by-value parameter's record is destroyed.
class CSoundBankRecord {
public:
    CSoundBankRecord(const CSoundBankRecord& other);                // 0x009922F0
    CSoundBankRecord& operator=(const CSoundBankRecord& other);     // 0x00992630
    ~CSoundBankRecord();                                            // 0x00992510
    unsigned char _data[0x20];
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<long, CSoundBankRecord> TSoundBankEntry;
typedef CKeyPairCompareLess<long, CSoundBankRecord> TSoundBankLess;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);

template <> void __fastcall __adjust_heap(TSoundBankEntry* __first, int __holeIndex, int __len, TSoundBankEntry __val, TSoundBankLess __comp);   // 0x00992B80

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                  _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap(TSoundBankEntry*, TSoundBankEntry*, TSoundBankEntry*, TSoundBankEntry, TSoundBankLess, int*);
}