// std::__uninitialized_copy(CRGBColour const* first, CRGBColour const* last, CRGBColour* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVCRGBColour@@PAV1@@std@@YIPAVCRGBColour@@PBV1@0PAV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082C690, 74 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0082c8a0 std::vector<CRGBColour, std::allocator<CRGBColour>>::vector; binder ??$__uninitialized_copy@PBVCRGBColour@@PAV1@@std@@YIPAVCRGBColour@@PBV1@0PAV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CRGBColour.
#define FABLE_RETAIL_CRGBCOLOUR
#include "stlport/fable_containers.h"
#include "engine/CRGBColour.h"
template CRGBColour* std::__uninitialized_copy<CRGBColour const*,CRGBColour* >(CRGBColour const*, CRGBColour const*, CRGBColour*, std::__false_type const&);
