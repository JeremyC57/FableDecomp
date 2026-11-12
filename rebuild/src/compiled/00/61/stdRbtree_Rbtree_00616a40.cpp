// std::_Rb_tree<long, std::pair<long const, std::pair<long, long> >, std::_Select1st<std::pair<long const, std::pair<long, long> > >, std::less<long>, std::allocator<std::pair<long const, std::pair<long, long> > > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<long const, std::pair<long, long> > >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@JU?$pair@$$CBJU?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBJU?$pair@JJ@std@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJU?$pair@JJ@std@@@std@@@2@@std@@QAE@ABU?$less@J@1@ABV?$allocator@U?$pair@$$CBJU?$pair@JJ@std@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00616A40, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<long, pair<long, long> > (node 0x1C = 0x10 + 4 + 8, signed key). Sources: (1) retail: CTCInventoryMap::CreateMapMarkers 0x005FF190 calls
//   this tree's _M_erase 0x00613CE0 (its local per-region counter map), and the same TU's landed _M_insert 0x00616F90 is this map's
//   (map<long, pair<long,long> >::operator[] 0x00618DE0 -> insert_unique 0x00617D50); the ~_Rb_tree 0x00616A80 / ~map 0x006172B0 call 0x00613CE0;
//   (2) FableWin.pdb ??0 / ??1 / ??A of map<long, pair<long,long> > and Ego_r.pdb ??A (donor 0x00668CB0), both in the CTCInventoryMap TU.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00613ce0,CUnprovenValue_00613ce0,CUnprovenKeyOfValue_00613ce0,std::less<CUnprovenKey_00613ce0>,std::allocator<CUnprovenValue_00613ce0> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00613ce0@@UCUnprovenValue_00613ce0@@UCUnprovenKeyOfValue_00613ce0@@U?$less@UCUnprovenKey_00613ce0@@@std@@V?$allocator@UCUnprovenValue_00613ce0@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00613ce0@@@1@ABV?$allocator@UCUnprovenValue_00613ce0@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

template std::_Rb_tree<long, std::pair<long const, std::pair<long, long> >, std::_Select1st<std::pair<long const, std::pair<long, long> > >, std::less<long>, std::allocator<std::pair<long const, std::pair<long, long> > > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<long const, std::pair<long, long> > >&);
