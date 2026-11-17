// CSymbolMap::RemoveSymbolsOfGroupType(const CCharString&) -- drop every symbol of one group: remove_if over SLONGMap with CPred_HasGroupName, forget each removed entry's name in NamesUsed, then erase the tail of the vector
// Leaf: ?RemoveSymbolsOfGroupType@CSymbolMap@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E5360, 187 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?RemoveSymbolsOfGroupType@CSymbolMap@@QAEXABVCCharString@@@Z; this is the only caller of
//   the landed std::remove_if<pair<CCharString, CSymbolMap::CSymbolInfo>*, CPred_HasGroupName> 0x009E7380, and it is
//   called twice from the landed CDefinitionManager::LoadDef 0x009AD6E0 (0x009AD918 / 0x009AD93D).
// Layout (retail): CSymbolMap SLONGMap +0x04 (CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> >: the
//   12-byte STLport vector, Compare +0x10, Dirty +0x11), NamesUsed +0x14 (set<CCharString>, 12 bytes); Ego_r has the
//   same order (Dinkumware sizes push NamesUsed to +0x18 there).
// Callees: CCharString copy ctor 0x0099EC30 (the predicate built in its argument slot), remove_if 0x009E7380 (/Gr),
//   NamesUsed's _M_upper_bound 0x004B9D50 / _M_lower_bound 0x004B9D00 / erase(first, last) 0x004B9C10 (the tree helpers
//   are this set<CCharString> tree's private STLport members, NF80 / NF82),
//   __copy 0x009E6600 (vector::erase's __copy_ptrs), ~CCharString 0x0099EAE0 (the erased tail).
// Shape: `NamesUsed.erase(NamesUsed.lower_bound(name), NamesUsed.upper_bound(name))` (equal_range's arguments are
//   evaluated right to left: upper first); SLONGMap.erase(new_end, end()) is STLport 4.6 vector::erase inlined
//   (__copy_ptrs -> __copy(last, finish, first, random_access_iterator_tag(), (int*)0), _Destroy, _M_finish = i).
// NF82 (q10993 / q10994): moved off the row-local CCharString / CSymbolMap / CVectorMap / vector / set / tree models onto
//   engine/CSymbolMap.h (FABLE_RETAIL_CSYMBOLMAP + _SLONGMAP and the new overlay opt-in FABLE_RETAIL_CSYMBOLMAP_NAMESUSED: NamesUsed
//   typed std::set<CCharString>, the method declared), stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: set
//   lower_bound / upper_bound / erase(first, last) over the out-of-line private tree members), stlport/position_algorithms.h
//   (out-of-line /Gr remove_if) and fable_containers.h (FABLE_RETAIL_STLP_COPY_NOINLINE: the __copy COMDAT called from the inline
//   vector::erase). The NamesUsed bounds now bind the private ABE spellings of 0x004B9D00 / 0x004B9D50 (were the public QBE ones).
//   CPred_HasGroupName (Ego_r.pdb: one CCharString Name) has no shared header; it is the TU's own functor.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_SLONGMAP
#define FABLE_RETAIL_CSYMBOLMAP_NAMESUSED
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "stlport/position_algorithms.h"
#include "engine/CSymbolMap.h"

class CPred_HasGroupName {
public:
    CPred_HasGroupName(const CCharString& name) : Name(name) { }
    bool operator()(const std::pair<CCharString, CSymbolMap::CSymbolInfo>& entry);
    CCharString Name;                               // +0x00
};

void CSymbolMap::RemoveSymbolsOfGroupType(const CCharString& group_name)
{
    std::pair<CCharString, CSymbolInfo>* new_end = std::remove_if(SLONGMap.begin(), SLONGMap.end(), CPred_HasGroupName(group_name));
    for (std::pair<CCharString, CSymbolInfo>* it = new_end; it != SLONGMap.end(); ++it)
        NamesUsed.erase(NamesUsed.lower_bound(it->second.EntryName), NamesUsed.upper_bound(it->second.EntryName));
    SLONGMap.erase(new_end, SLONGMap.end());
}
