// std::_Push_heap(std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>* first, int hole, int top, std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> val, CKeyPairCompareLess<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> pred) -- percolate a scripted-entity persist record up a max-heap ordered by its 64-bit key, then destroy the by-value record
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00CB93E0, 178 bytes (/O2). Called from _Adjust_heap @ 0x00CB9FA0. Callees:
// CCharString::operator= 0x0099EFB0, CCharString::~CCharString 0x0099EAE0.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 24-byte element
// (`lea eax,[edi+edi*2]` * 8): UNSIGNED 64-bit key +0 (high word first: `ja done ; jb move`, then
// `jae done` on the low word), NScript::CRuntimeScriptedEntityPersist::CPersistInfo +8 (Ego_r.pdb:
// ulong64 UID +0, CCharString PersistScript +8; 16 bytes with the 8-alignment padding), as labelled.
// The implicit assignment copies the key and UID as four dwords, then PersistScript through its
// out-of-line operator=; the word at +0x14 is alignment padding and is never copied. The by-value
// record's string is destroyed at the end. /Gr: first=ecx, hole=edx; `ret 20h` = top + 24-byte pair
// + the empty comparator.
namespace N00cb93e0 {
} // (hoisted to global scope: naming wave 9)
class CCharString { public:
    void* Data;
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
    ~CCharString();                                       // 0x0099EAE0
};
namespace N00cb93e0 {
namespace NScript { namespace CRuntimeScriptedEntityPersist {
struct CPersistInfo {
    unsigned __int64 UID;                                 // +0x0
    CCharString PersistScript;                            // +0x8
};
} }
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
typedef pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> Elem;
typedef CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}