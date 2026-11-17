// std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- link a new (augmentation type, sound set) node left or right of y, keep leftmost/rightmost, rebalance, count it
// Leaf: ?_M_insert@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076A110, 199 bytes (+int3 pad to 208), `ret 14h` (hidden iterator result + 4 arguments).
// Callers: nine sites in 0x0076A1E0..0x0076A891 (insert_unique / hint insert of the same tree).
// Callees: malloc 0x00BFEA0E and CCharString copy constructor 0x0099EC30 (the inlined left-hand node construction),
//   _M_create_node 0x00769980 (right-hand branch), _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (__cdecl).
// Identity: Ego_r.pdb CTCObjectAugmentations::InitSounds / LoopingSounds = map<EObjectAugmentationType,
//   CObjectAugmentationSoundSet>; the value is the 0x10-byte pair at node +0x10 (key compared signed: `jl`).
// Shape: the landed STLport _M_insert 0x0069D520 with `y == header || (w == 0 && (x != 0 || key(v) < key(y)))`; this
//   instantiation inlines _M_create_node in the left-hand branch only and
//   calls the out-of-line member in the right-hand branch. The iterator returns through the hidden pointer.
//   The landed callers insert_unique 0x0076A1E0 / 0x0076A700 bind exactly this spelling (probe of 0x0076A1E0).
// agentNF57 (q10438): an explicit instantiation of the STLport tree member on stlport/fable_map.h (FABLE_RETAIL_STLP_MAP), the element the
//   shared engine/EObjectAugmentationType.h / engine/CObjectAugmentationSoundSet.h (Ego_r.pdb CTCObjectAugmentations::InitSounds /
//   LoopingSounds map<EObjectAugmentationType, CObjectAugmentationSoundSet>); descriptive-by-library (retail is STLport 4.6, both PDBs
//   are Dinkumware builds). The sound set is copied inline, memberwise (new overlay opt-in
//   FABLE_RETAIL_COBJECTAUGMENTATIONSOUNDSET_IMPLICIT_COPY drops the out-of-line copy-constructor declaration). Was the CSoundSetTree
//   stand-in class with local CCharString / CObjectAugmentationSoundSet models.
//   _M_insert is STLport 4.6.2 _tree.c's own body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY); VC7.1 inlines _M_create_node in one branch
//   and calls 0x00769980 in the other, as retail does.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_COBJECTAUGMENTATIONSOUNDSET_IMPLICIT_COPY
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
#include "engine/CObjectAugmentationSoundSet.h"

typedef std::pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> SSoundSetValue;
typedef std::_Rb_tree<EObjectAugmentationType, SSoundSetValue, std::_Select1st<SSoundSetValue>, std::less<EObjectAugmentationType>,
                      std::allocator<SSoundSetValue> > CSoundSetTree;
template std::_Rb_tree_iterator<SSoundSetValue, std::_Nonconst_traits<SSoundSetValue> >
    CSoundSetTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const SSoundSetValue&, std::_Rb_tree_node_base*);
