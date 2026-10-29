// std::_Push_heap(std::pair<unsigned long,CDefinitionManager::CDefClassInfo>* first, int hole, int top, std::pair<unsigned long,CDefinitionManager::CDefClassInfo> val, CKeyPairCompareLess<unsigned long,CDefinitionManager::CDefClassInfo> pred) -- percolate a def-class info entry up a max-heap ordered by its CRC key, then destroy the by-value entry
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x009B77B0, 192 bytes incl. int3 padding (/O2). Callees: CCharString::operator=
// 0x0099EFB0, CArray<unsigned long>::operator= 0x0045BC09 (STLport vector assignment, out of line),
// free 0x00BFEA14, CCharString::~CCharString 0x0099EAE0.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 24-byte element
// (`lea eax,[edi+edi*2]` * 8): unsigned long key +0, CDefinitionManager::CDefClassInfo +4 (Ego_r.pdb:
// CCharString Name +0, AllocFunc +4, CArray<unsigned long> GlobalIndices +8 -- the retail STLport
// vector is 12 bytes, not the PDB's 16). The implicit pair/info operator= is key move, Name's
// out-of-line operator=, the AllocFunc dword, then the CArray's out-of-line operator=; the key test
// is UNSIGNED (`jae`). The by-value argument is destroyed in the callee: the inline STLport
// `~_Vector_base` (`if (_M_start) free(_M_start)`) and then ~CCharString on Name.
// /Gr: first=ecx, hole=edx; `ret 20h` = top + 24-byte pair + the empty comparator.
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
namespace N009b77b0 {
} // (hoisted to global scope: naming wave 9)
class CCharString { public:
    void* Data;
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
    ~CCharString();                                       // 0x0099EAE0
};
namespace N009b77b0 {
template<class T> struct CArray {                         // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray& operator=(const CArray& other);               // 0x0045BC09
    ~CArray() { if (_M_start) free(_M_start); }
};
struct CDefClassBase;
namespace CDefinitionManager {
struct CDefClassInfo {
    CCharString Name;                                     // +0x0
    CDefClassBase* (__cdecl* AllocFunc)();                // +0x4
    CArray<unsigned long> GlobalIndices;                  // +0x8
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
typedef pair<unsigned long, CDefinitionManager::CDefClassInfo> Elem;
typedef CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}