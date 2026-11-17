// std::_Rb_tree<EGameAction, pair<EGameAction const, std::pair<C3DVector, long> >, _Select1st<pair<EGameAction const, std::pair<C3DVector, long> > >, less<EGameAction >, allocator<pair<EGameAction const, std::pair<C3DVector, long> > > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with the instance's iterator operator-- (0x006ADFB0, out of line) unless at begin(), then _M_insert (0x006AE900) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EGameAction@@U?$pair@$$CBW4EGameAction@@U?$pair@VC3DVector@@J@std@@@std@@U?$_Select1st@U?$pair@$$CBW4EGameAction@@U?$pair@VC3DVector@@J@std@@@std@@@3@U?$less@W4EGameAction@@@3@V?$allocator@U?$pair@$$CBW4EGameAction@@U?$pair@VC3DVector@@J@std@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EGameAction@@U?$pair@VC3DVector@@J@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EGameAction@@U?$pair@VC3DVector@@J@std@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EGameAction@@U?$pair@VC3DVector@@J@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AE9F0, 180 bytes, thiscall `ret 8`; /O2 TU.
// Identity (NF58, round 7): the same tree as its callee _M_insert 0x006AE900 (see that row); its landed caller is CThingPlayerCreature::ApplyRelativeMovementAcceleration 0x006AD3C0 (Ego r.pdb CThingPlayerCreature::RelativeMovementAccelerationComponents).
// Was the stand-in `_unattributed::InsertUnique_CRetailTree_006ae9f0` (local model); now the explicit instantiation of stlport/fable_map.h's insert_unique (FABLE_RETAIL_STLP_MAP
//   body) with the iterator's operator-- kept out of line (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE, as retail calls the per-instance
//   COMDAT 0x006ADFB0). STLport 4.6.2 source, descriptive-by-library.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EGameAction.h"
#include "engine/C3DVector.h"
namespace std {
template pair<_Rb_tree<EGameAction, pair<EGameAction const, std::pair<C3DVector, long> >, _Select1st<pair<EGameAction const, std::pair<C3DVector, long> > >, less<EGameAction >, allocator<pair<EGameAction const, std::pair<C3DVector, long> > > >::iterator, bool> _Rb_tree<EGameAction, pair<EGameAction const, std::pair<C3DVector, long> >, _Select1st<pair<EGameAction const, std::pair<C3DVector, long> > >, less<EGameAction >, allocator<pair<EGameAction const, std::pair<C3DVector, long> > > >::insert_unique(pair<EGameAction const, std::pair<C3DVector, long> > const&);
}
