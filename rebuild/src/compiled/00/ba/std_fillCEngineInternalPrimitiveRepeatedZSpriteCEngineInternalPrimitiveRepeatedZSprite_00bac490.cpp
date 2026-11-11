// std::fill(CEngineInternalPrimitiveRepeatedZSprite** first, CEngineInternalPrimitiveRepeatedZSprite** last, CEngineInternalPrimitiveRepeatedZSprite* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCEngineInternalPrimitiveRepeatedZSprite@@PAV1@@std@@YIXPAPAVCEngineInternalPrimitiveRepeatedZSprite@@0ABQAV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BAC490, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00bac800 std::vector<CEngineInternalPrimitiveRepeatedZSprite *, std::allocator<CEngineInternalPrimitiveRepeatedZSprite *>>::_M_fill_insert; caller 00bac800 std::vector<CEngineInternalPrimitiveRepeatedZSprite *, std::allocator<CEngineInternalPrimitiveRepeatedZSprite *>>::_M_fill_insert; binder ??$fill@PAPAVCEngineInternalPrimitiveRepeatedZSprite@@PAV1@@std@@YIXPAPAVCEngineInternalPrimitiveRepeatedZSprite@@0ABQAV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineInternalPrimitiveRepeatedZSprite*.
#include "stlport/fable_containers.h"
class CEngineInternalPrimitiveRepeatedZSprite;
template void std::fill<CEngineInternalPrimitiveRepeatedZSprite**,CEngineInternalPrimitiveRepeatedZSprite* >(CEngineInternalPrimitiveRepeatedZSprite**, CEngineInternalPrimitiveRepeatedZSprite**, CEngineInternalPrimitiveRepeatedZSprite* const&);
