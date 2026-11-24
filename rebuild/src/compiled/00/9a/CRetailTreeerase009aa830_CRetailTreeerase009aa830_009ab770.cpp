// std::_Rb_tree<CCharString, std::pair<const CCharString, CCharString>, ...>::~_Rb_tree() -- STLport tree destructor: clear() (the out-of-line recursive _M_erase from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V1@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V1@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AB770, 66 bytes, thiscall; no retail referrer (the linker kept the pick-any copy). Callees: the tree's
//   _M_erase 0x009AA830 (landed, STLport spelling), free 0x00BFEA14.
// Identity (NF391, the T3 nf389 derivation test applied to NF132's twin; advisor decision, owner delegated): 0x009AC310 is ~map (34 retail
//   call sites, both PDB name sets, landed). A full-image E8/E9 scan of _M_erase 0x009AA830 finds 22 sites and exactly two standalone 66-byte
//   destructor bodies, 0x009AB770 and 0x009AC310; both PDB name sets know this tree only under map traits ($0A@: no multimap, no set over
//   the pair); and the TU that emits ~map also emits the 66-byte ~_Rb_tree calling _M_erase directly. So the unreferenced twin is the
//   tree's own destructor.
// NF391: was the generated CRetailTree_erase009aa830 destructor (one of 108 bodies named by their erase callee) over a local tree model;
//   now the repo STLport (stlport/fable_map.h, FABLE_RETAIL_STLP_MAP) in the pick-any form.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
namespace {
// Fidelity: scaffolding, not retail: a TU-internal holder whose out-of-line destructor destroys the map member, so VC7.1 emits the
// implicit map / tree destructors (and _M_erase) as pick-any COMDATs, as every retail TU that destroys this map does (GOTCHAS nf86 / nf88,
// nf387). Internal to this TU; never called.
struct CharStringMapHolder {
    std::map<CCharString, CCharString> Map;
    ~CharStringMapHolder();
};
CharStringMapHolder::~CharStringMapHolder() {}
}
