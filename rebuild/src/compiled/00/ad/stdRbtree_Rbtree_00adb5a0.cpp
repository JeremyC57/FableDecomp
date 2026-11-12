// std::_Rb_tree<unsigned long, std::pair<unsigned long const, CVertexGroupBlockOptimiser::CGroupInfo>, std::_Select1st<std::pair<unsigned long const, CVertexGroupBlockOptimiser::CGroupInfo> >, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, CVertexGroupBlockOptimiser::CGroupInfo> > >::_Rb_tree(const std::less<unsigned long>&, const std::allocator<std::pair<unsigned long const, CVertexGroupBlockOptimiser::CGroupInfo> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@KU?$pair@$$CBKVCGroupInfo@CVertexGroupBlockOptimiser@@@std@@U?$_Select1st@U?$pair@$$CBKVCGroupInfo@CVertexGroupBlockOptimiser@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCGroupInfo@CVertexGroupBlockOptimiser@@@std@@@2@@std@@QAE@ABU?$less@K@1@ABV?$allocator@U?$pair@$$CBKVCGroupInfo@CVertexGroupBlockOptimiser@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADB5A0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo> (agentFAM07 proved this tree for 0x00ADB8D0 map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo>::~map, RENAMES 2026-11-11): CVertexGroupBlockOptimiser::FindBestStartingBlock 0x00AD94F0 builds a local map of group infos and calls this tree's _M_erase 0x00ADAE00, landed and bound as _Rb_tree<unsigned long, pair<const unsigned long, CVertexGroupBlockOptimiser::CGroupInfo>, ...>; ??0 / ??1 of map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo> are in the PDB name sets; node 0x1C = 0x10 + 4 + 8. Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here..
//   This _Rb_tree constructor 0x00ADB5A0 is the COMDAT right before the tree's ~_Rb_tree 0x00ADB5E0, which calls that _M_erase 0x00ADAE00.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00adae00,CUnprovenValue_00adae00,CUnprovenKeyOfValue_00adae00,std::less<CUnprovenKey_00adae00>,std::allocator<CUnprovenValue_00adae00> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00adae00@@UCUnprovenValue_00adae00@@UCUnprovenKeyOfValue_00adae00@@U?$less@UCUnprovenKey_00adae00@@@std@@V?$allocator@UCUnprovenValue_00adae00@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00adae00@@@1@ABV?$allocator@UCUnprovenValue_00adae00@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CVertexGroupBlockOptimiser {   // no shared header for CVertexGroupBlockOptimiser; Ego_r.pdb CGroupInfo members
public:
    class CGroupInfo {
    public:
        unsigned long Count;                      // +0x00
        long BlockIndex;                          // +0x04
    };
};
#include "stlport/fable_map.h"

template std::_Rb_tree<unsigned long, std::pair<unsigned long const, CVertexGroupBlockOptimiser::CGroupInfo>, std::_Select1st<std::pair<unsigned long const, CVertexGroupBlockOptimiser::CGroupInfo> >, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, CVertexGroupBlockOptimiser::CGroupInfo> > >::_Rb_tree(const std::less<unsigned long>&, const std::allocator<std::pair<unsigned long const, CVertexGroupBlockOptimiser::CGroupInfo> >&);
