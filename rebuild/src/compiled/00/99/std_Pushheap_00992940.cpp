// std::_Push_heap(std::pair<long,CSoundBankRecord>* first, int hole, int top, std::pair<long,CSoundBankRecord> val, CKeyPairCompareLess<long,CSoundBankRecord> pred) -- percolate a keyed sound-bank record up a max-heap ordered by the signed key, then destroy the by-value record
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00992940, 128 bytes (/O2). Called from the landed _Adjust_heap @ 0x00992B80
// (same instantiation). Callees: the record's operator= 0x00992630 and destructor 0x00992510.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 36-byte element
// (`lea eax,[esi+esi*8]` * 4): long key +0 and the 0x20-byte sound-bank record +4 (two counted
// pointers, four CCharStrings; see the landed 0x00992510). The implicit pair operator= is the key
// move plus the record's out-of-line operator= (result dropped); the key test is SIGNED (`jge`);
// the by-value argument's record is destroyed in the callee after the final store (`lea
// ecx,[esp+1Ch] ; call ~CSoundBankRecord`). /Gr: first=ecx, hole=edx; `ret 2Ch` = top + 36-byte pair
// + the empty comparator. Label correction: the propagated pair<unsigned long, CDefinitionManager::
// CDefClassInfo> compares unsigned; this element is the CASoundManager record heap.
namespace N00992940 {
struct CSoundBankRecord {                                 // 0x20 bytes (see the landed 0x00992510)
    unsigned char m_body[0x20];
    CSoundBankRecord(const CSoundBankRecord& other);              // 0x009922F0
    CSoundBankRecord& operator=(const CSoundBankRecord& other);   // 0x00992630
    ~CSoundBankRecord();                                          // 0x00992510
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<long, CSoundBankRecord> Elem;
typedef CKeyPairCompareLess<long, CSoundBankRecord> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}