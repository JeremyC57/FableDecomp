// std::_Rb_tree<NCameraModes::EMode, pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >, ...>::_M_insert(x, y, v, w) -- STLport red-black tree node insert: allocate and copy-construct the node, link it left or right of y, rebalance, count it
// Leaf: ?_M_insert@?$_Rb_tree@W4EMode@NCameraModes@@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@U?$_Select1st@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@@4@U?$less@W4EMode@NCameraModes@@@4@V?$allocator@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@@4@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069D520, 211 bytes, `ret 14h` (hidden iterator result + 4 arguments).
// Callees: malloc 0x00BFEA0E (the 0x1c-byte node: 0x10 links + the 12-byte pair), _Rb_global<bool>::_Rebalance(node, root&)
//   0x0042971D (?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z, unchanged).
// Identity (NF56, round 7, queue q10409): CGameCameraManager::RegisteredModes = map<NCameraModes::EMode, CCountedPointer<
//   NCameraModes::CBaseInit> > (Ego_r.pdb); its insert_unique 0x0069D900 (NF41, landed) calls this address with this STLport spelling
//   (private _M_insert, the hidden _Rb_tree_iterator result). Was the stand-in CCameraModeMap::_M_insert over local SRbNode /
//   SCameraModeValue / pair models. Now the explicit instantiation of the shared stlport/fable_map.h _M_insert
//   (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY: STLport 4.6.2 _tree.c 345-383 with _M_create_node) over engine/CCountedPointer.h.
// Shape: STLport's `y == header || (w == 0 && (x != 0 || key(v) < key(y)))` (signed compare); _M_create_node duplicated in both
//   branches; the iterator returned by value through the hidden pointer.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
namespace NCameraModes { class CBaseInit; enum EMode; }

template std::_Rb_tree_iterator<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >,
    std::_Nonconst_traits<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > > >
std::_Rb_tree<NCameraModes::EMode, std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >,
    std::_Select1st<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > >, std::less<NCameraModes::EMode>,
    std::allocator<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > > >::_M_insert(
    std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >&,
    std::_Rb_tree_node_base*);
