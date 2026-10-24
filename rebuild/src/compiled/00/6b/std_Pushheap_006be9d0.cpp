// std::_Push_heap(std::pair<long,CMusicManager::CRegisteredMusicEntry>* first, int hole, int top, std::pair<long,CMusicManager::CRegisteredMusicEntry> val, CKeyPairCompareLess<long,CMusicManager::CRegisteredMusicEntry> pred) -- percolate a registered music entry up a max-heap ordered by its id, then destroy the by-value entry
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x006BE9D0, 208 bytes incl. int3 padding (/O2). Callees: CWideString::operator=
// 0x0099B7D0, CWideString::~CWideString 0x0099B510.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 24-byte element
// (`lea eax,[ebx+ebx*2]` * 8): long key +0, CMusicManager::CRegisteredMusicEntry +4 (Ego_r.pdb, 20
// bytes: CWideString SrcFileName, DstFileName, float Gain, Offset, bool CacheToHD), as labelled.
// The implicit pair/entry operator= is the key move, two out-of-line CWideString assignments, then
// Gain/Offset as dword moves and the bool as a byte move; the key test is SIGNED (`jge`). The
// by-value argument's strings are destroyed in reverse order at the end.
// /Gr: first=ecx, hole=edx; `ret 20h` = top + 24-byte pair + the empty comparator.
namespace N006be9d0 {
struct CWideString {
    void* Data;
    CWideString& operator=(const CWideString& other);     // 0x0099B7D0
    ~CWideString();                                       // 0x0099B510
};
namespace CMusicManager {
struct CRegisteredMusicEntry {
    CWideString SrcFileName;                              // +0x00
    CWideString DstFileName;                              // +0x04
    float Gain;                                           // +0x08
    float Offset;                                         // +0x0C
    bool CacheToHD;                                       // +0x10
};
}
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
typedef pair<long, CMusicManager::CRegisteredMusicEntry> Elem;
typedef CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}