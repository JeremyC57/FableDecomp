// std::uninitialized_fill_n(CMultiStaticMeshEntryDef* first, unsigned int n, CMultiStaticMeshEntryDef const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCMultiStaticMeshEntryDef@@IV1@@std@@YIPAVCMultiStaticMeshEntryDef@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EE219, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 004edfdc std::vector<CMultiStaticMeshEntryDef>::_M_fill_insert; callee 004eb864 CMultiStaticMeshEntryDef::CMultiStaticMeshEntryDef; binder ??$uninitialized_fill_n@PAVCMultiStaticMeshEntryDef@@IV1@@std@@YIPAVCMultiStaticMeshEntryDef@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMultiStaticMeshEntryDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMULTISTATICMESHENTRYDEF
#include "stlport/fable_containers.h"
#include "engine/CMultiStaticMeshEntryDef.h"
template CMultiStaticMeshEntryDef* std::uninitialized_fill_n<CMultiStaticMeshEntryDef*,unsigned int,CMultiStaticMeshEntryDef >(CMultiStaticMeshEntryDef*, unsigned int, CMultiStaticMeshEntryDef const&);
