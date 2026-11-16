// std::_Rb_tree<NCameraModes::EMode, std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >, ...>::insert_unique(const value_type& value) -- STLport 4.6 _tree.c insert_unique of CGameCameraManager::RegisteredModes: descend from the root, step back once unless at begin, insert a key not yet present through _M_insert; returns {iterator, inserted}
// Leaf: ?insert_unique@?$_Rb_tree@W4EMode@NCameraModes@@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@U?$_Select1st@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@@4@U?$less@W4EMode@NCameraModes@@@4@V?$allocator@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@@4@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069D900, oracle length 180 bytes, `ret 8` (hidden result pointer + value).
// Identity: the map type is CGameCameraManager::RegisteredModes (Ego_r.pdb member type map<NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >);
//   the constructor 0x0069AE80 calls this insert_unique six times through map::insert (one per registered camera mode).
// Callees: the out-of-line _Rb_tree_iterator decrement 0x0069CA00 and _M_insert 0x0069D520 (hidden-pointer iterator result).
// Body: stlport/fable_map.h FABLE_RETAIL_STLP_MAP (as 0x005685E0, the map<long, CCountedPointer<CTexture> > instance).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/NCameraModes_CBaseInit.h"
#include "engine/NCameraModes_CBase.h"

template std::pair<std::_Rb_tree_iterator<std::pair<NCameraModes::EMode const, CCountedPointer<NCameraModes::CBaseInit> >,
    std::_Nonconst_traits<std::pair<NCameraModes::EMode const, CCountedPointer<NCameraModes::CBaseInit> > > >, bool>
    std::_Rb_tree<NCameraModes::EMode, std::pair<NCameraModes::EMode const, CCountedPointer<NCameraModes::CBaseInit> >,
        std::_Select1st<std::pair<NCameraModes::EMode const, CCountedPointer<NCameraModes::CBaseInit> > >, std::less<NCameraModes::EMode>,
        std::allocator<std::pair<NCameraModes::EMode const, CCountedPointer<NCameraModes::CBaseInit> > > >::insert_unique(
    std::pair<NCameraModes::EMode const, CCountedPointer<NCameraModes::CBaseInit> > const&);
