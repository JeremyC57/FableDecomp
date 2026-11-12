// std::_Rb_tree<ESurfaceMultisampleType, ESurfaceMultisampleType, std::_Identity<ESurfaceMultisampleType>, std::less<ESurfaceMultisampleType>, std::allocator<ESurfaceMultisampleType> >::insert_unique(ESurfaceMultisampleType const&) -- STLport 4.6 insert_unique (stlport/fable_map.h)
// Leaf: ?insert_unique@?$_Rb_tree@W4ESurfaceMultisampleType@@W41@U?$_Identity@W4ESurfaceMultisampleType@@@std@@U?$less@W4ESurfaceMultisampleType@@@3@V?$allocator@W4ESurfaceMultisampleType@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@W4ESurfaceMultisampleType@@U?$_Nonconst_traits@W4ESurfaceMultisampleType@@@std@@@std@@_N@2@ABW4ESurfaceMultisampleType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C29B0, oracle length 180 bytes.
// Identity: (1) the landed caller 0x009C05C0 (CDisplayManager::EnumerateAntiAliasingModes) binds this set's insert_unique (its iterator spelled without the traits argument: a binder spelling to fix, see NOTES) (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the set<W4ESurfaceMultisampleType@@> instantiation (_Tset_traits@W4ESurfaceMultisampleType@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
enum ESurfaceMultisampleType { };
typedef std::_Rb_tree<ESurfaceMultisampleType, ESurfaceMultisampleType, std::_Identity<ESurfaceMultisampleType>, std::less<ESurfaceMultisampleType>, std::allocator<ESurfaceMultisampleType> > _Tree_ESurfaceMultisampleType;
template std::pair<_Tree_ESurfaceMultisampleType::iterator, bool> _Tree_ESurfaceMultisampleType::insert_unique(const ESurfaceMultisampleType&);
