// std::map<unsigned long, CCharString>::operator[](const unsigned long& k) -- find the entry for k (inline lower_bound walk); when absent insert (k, CCharString()) at the hint; return a reference to the mapped string
// Leaf: ??A?$map@KVCCharString@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@3@@std@@QAEAAVCCharString@@ABK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4A670, length 147 bytes (+13 int3 pad), `ret 4`.
// Callers: 0x00A48C6E (CLipSyncSampleData::WriteToFile 0x00A48BA0), 0x00A496F7 / 0x00A499F7 (CLipSyncSampleData::ReadFromFile
//   0x00A495F0): the phoneme ID -> symbol map of the lip-sync sample files.
// Callees: CCharString default ctor 0x0099E4B0 / copy ctor 0x0099EC30 / dtor 0x0099EAE0, 0x00A49F60
//   _Rb_tree::insert_unique(iterator, const value_type&) (landed; key compared unsigned, string at node +0x14).
// Identity: catalogued as `BST_InsertEntry_EHeroMorphType` (propagated); the key compares are unsigned (`jb` / `jae`), as in
//   the landed insert_unique, so the key is an unsigned long (the phoneme ID).
// Shape: STLport 4.x `map::operator[]` verbatim: `lower_bound` inlined (header in ecx, root at header+4, y kept in esi),
//   `__i == end() || key_comp()(k, __i->first)`, then `insert(__i, value_type(k, __default_constructed((_Tp*)0)))`:
//   the by-value CCharString() result is homed in the dead `k` argument slot, the pair is built in the frame (key word,
//   string copy ctor), the 4-byte iterator (user copy ctor) is built in the argument area and the result iterator comes
//   back through the hidden pointer; both string temporaries are destroyed after the call.
// Name (round 4, NF40): Codex pass 22 (landed_evidence/00a495f0 NAME_FIXES.tsv) and the landed CLipSyncSampleData::ReadFromFile
//   0x00A495F0, which binds this spelling here, prove std::map<unsigned long, CCharString>::operator[] (the STLport less / pair are
//   structs: U?$less / U?$pair). The row's own local std model spelled them as classes; now the explicit instantiation of
//   stlport/fable_map.h's operator[] (FABLE_RETAIL_STLP_MAP, __default_constructed under FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ)
//   over fable_string.h, with the 0x00A495F0 TU's opt-ins.
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"

template CCharString& std::map<unsigned long, CCharString, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CCharString> > >::operator[](const unsigned long&);
