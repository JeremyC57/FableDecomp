// std::pair<const EHeroTrainableStatType,SExpSpendUndo>::`scalar deleting destructor'(unsigned int flags) -- destroy the SExpSpendUndo value (+4), free the pair when bit 0 is set
// Leaf: ??_G?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005799DE, 31 bytes, thiscall `ret 4`; size-optimised (`pop ecx` after the delete).
// Identity: the map node value of CTCInventoryExperience::UndoableStatBuys (struct_layouts_egor:
//   map<EHeroTrainableStatType, SExpSpendUndo>). Its only direct caller is 0x00579D18, the UndoableStatBuys erase(iterator), reached from CTCInventoryExperience::UndoBuyStat 0x0057712C;
//   STLport erase runs `_Destroy(&node->_M_value_field)`, which VC7.1 under optimize("s") compiles as `push 0 ; call ??_G`
//   (a call with flag 0) instead of inlining ~pair. The twin for map<EHeroAbility, SExpSpendUndo> sits beside it
//   (0x005799DE / 0x005799FD, no identical-code folding). SExpSpendUndo::~SExpSpendUndo is 0x005797FA (Ego_r.pdb
//   ??1SExpSpendUndo@@QAE@XZ). The earlier "CLandscapeBackgroundPatch::`vector_deleting_destructor'" label was propagated.
// Shape: the implicit ~pair is emitted beside it; the explicit _Destroy instantiation below is what makes VC7.1 emit the
//   `??_G` (exactly STLport's `_Destroy(_Tp* p) { p->~_Tp(); }`).
#pragma optimize("s", on)
enum EHeroTrainableStatType { EHEROTRAINABLESTATTYPE_FIRST };

struct SExpSpendUndo {
    unsigned char _body[0x34];                  // four STLport list<long> + nine longs (see 0x005797FA)
    ~SExpSpendUndo();                           // 0x005797FA
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                  // +0x00
    _T2 second;                                 // +0x04
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template void _Destroy(pair<const EHeroTrainableStatType, SExpSpendUndo>* __pointer);
}
