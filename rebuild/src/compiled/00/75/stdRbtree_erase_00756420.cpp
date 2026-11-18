// std::_Rb_tree<CCharString, pair<const CCharString, pair<long, long> >, ..>::erase(iterator first, iterator last) -- STLport 4.6 range erase of the crime map's tree (clear() when the range is the whole tree)
// Leaf: ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00756420, length 198 bytes, `ret 8` (thiscall: first, last).
// Identity: STLport 4.6 _tree.c erase(iterator, iterator) over map<CCharString, pair<long, long> >; its landed caller
//   erase(const CCharString&) 0x00756C60 (NF85, on stlport/fable_map.h) already binds this spelling, and its _M_erase callee 0x00755E80 is
//   the same tree's. The old row's _Rb_tree_CrimeMap / _Rb_tree_iterator_CrimeMap names were stand-ins (descriptive-by-library: no PDB
//   holds STLport instances; the spelling is what VC7.1 emits for STLport 4.6 over the element type its callers prove).
// Shape: STLport 4.6 _tree.c `erase(iterator, iterator)`: clear() and erase(iterator) inline, `erase(first++)` with the successor walk
//   inline (_Rb_tree_base_iterator::_M_increment), _Rebalance_for_erase 0x0042A34E, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// NF87: moved off the row-local CCharString / tree / iterator models onto stlport/fable_map.h with the 0x004B9C10 (NF85) opt-in set;
//   explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "stlport/fable_map.h"
#include "fable_string.h"
template void std::_Rb_tree<CCharString, std::pair<CCharString const, std::pair<long, long> >, std::_Select1st<std::pair<CCharString const, std::pair<long, long> > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, std::pair<long, long> > > >::erase(std::_Rb_tree_iterator<std::pair<CCharString const, std::pair<long, long> >, std::_Nonconst_traits<std::pair<CCharString const, std::pair<long, long> > > >, std::_Rb_tree_iterator<std::pair<CCharString const, std::pair<long, long> >, std::_Nonconst_traits<std::pair<CCharString const, std::pair<long, long> > > >);
