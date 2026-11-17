// std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> >::_M_create_node(const value_type& x) -- allocate a 0x20-byte tree node and copy-construct the (augmentation type, sound set) pair into it
// Leaf: ?_M_create_node@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@std@@@2@ABU?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationSoundSet@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769980, 62 bytes (+int3 pad to 64), `ret 4` (thiscall; `this` is never read).
// Caller: 0x0076A110 _M_insert (its right-hand branch; the left-hand branch carries an inlined copy).
// Callees: malloc 0x00BFEA0E (STLport's malloc allocator, n = 1 folded into `push 20h`), CCharString copy
//   constructor 0x0099EC30.
// Identity: Ego_r.pdb CTCObjectAugmentations::InitSounds / LoopingSounds are map<EObjectAugmentationType,
//   CObjectAugmentationSoundSet>; the 0x10-byte value (key +0, Played +4, Sound +8, Handle +0xc) sits after the
//   0x10-byte node links. The catalog's "RBTree_Node_Allocate_And_Construct" is a descriptive auto label.
// Shape: STLport `_M_create_node`: allocate, then `_Construct(&tmp->_M_value_field, x)` (placement new, null-tested);
//   the pair's user copy constructor copies the key, then the sound set's implicit memberwise copy (byte, string
//   copy, dword). The node is returned in eax.
// agentNF57 (q10438): an explicit instantiation of the STLport tree member on stlport/fable_map.h (FABLE_RETAIL_STLP_MAP), the element the
//   shared engine/EObjectAugmentationType.h / engine/CObjectAugmentationSoundSet.h (Ego_r.pdb CTCObjectAugmentations::InitSounds /
//   LoopingSounds map<EObjectAugmentationType, CObjectAugmentationSoundSet>); descriptive-by-library (retail is STLport 4.6, both PDBs
//   are Dinkumware builds). The sound set is copied inline, memberwise (new overlay opt-in
//   FABLE_RETAIL_COBJECTAUGMENTATIONSOUNDSET_IMPLICIT_COPY drops the out-of-line copy-constructor declaration). Was the CSoundSetTree
//   stand-in class with local CCharString / CObjectAugmentationSoundSet models.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_COBJECTAUGMENTATIONSOUNDSET_IMPLICIT_COPY
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
#include "engine/CObjectAugmentationSoundSet.h"

typedef std::pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> SSoundSetValue;
typedef std::_Rb_tree<EObjectAugmentationType, SSoundSetValue, std::_Select1st<SSoundSetValue>, std::less<EObjectAugmentationType>,
                      std::allocator<SSoundSetValue> > CSoundSetTree;
template std::_Rb_tree_node<SSoundSetValue>* CSoundSetTree::_M_create_node(const SSoundSetValue&);
