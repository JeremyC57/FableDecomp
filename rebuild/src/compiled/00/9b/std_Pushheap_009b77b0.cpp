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
// Binder (NF387 / NF395, T3 nf383 0x009B7870 and nf393 0x00728B10; rulings R1 / R10 / R11): GlobalIndices is the shared engine/CArray.h
//   CArray<unsigned long> (CArray<T> : STLport vector<T>); its generated assignment calls
//   ??4?$vector@KV?$allocator@K@std@@@std@@QAEAAV01@ABV01@@Z 0x0045BC09 (declared out of line, FABLE_RETAIL_STLP_VECTOR_ASSIGN) and its
//   destructor is the inline STLport ~vector (`if (_M_start) free(_M_start)`); was a local CArray in this namespace whose operator= the
//   table bound at 0x0045BC09 under the stand-in spelling ??4?$CArray@K@N009b77b0@@.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "engine/CArray.h"
namespace N009b77b0 {
struct CDefClassBase;
namespace CDefinitionManager {
struct CDefClassInfo {
    CCharString Name;                                     // +0x0
    CDefClassBase* (__cdecl* AllocFunc)();                // +0x4
    ::CArray<unsigned long> GlobalIndices;                // +0x8
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