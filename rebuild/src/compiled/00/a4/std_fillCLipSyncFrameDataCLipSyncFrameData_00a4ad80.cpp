// std::fill(CLipSyncFrameData* first, CLipSyncFrameData* last, CLipSyncFrameData const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCLipSyncFrameData@@V1@@std@@YIXPAVCLipSyncFrameData@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4AD80, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a4b020 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::_M_fill_insert; binder ??$fill@PAVCLipSyncFrameData@@V1@@std@@YIXPAVCLipSyncFrameData@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLipSyncFrameData.
// NF42 (round 5): the local CLipSyncFrameData stand-in (an opaque 12-byte class with an out-of-line operator=) replaced by the shared
// engine/CLipSyncFrameData.h (FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: one CArray<CLipSyncPhoneme> Phonemes member); its implicit
// copy assignment reduces to STLport vector<CLipSyncPhoneme>::operator= (0x00A4A520, declaration-only FABLE_RETAIL_STLP_VECTOR_ASSIGN).
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#include "engine/CLipSyncFrameData.h"
template void std::fill<CLipSyncFrameData*,CLipSyncFrameData >(CLipSyncFrameData*, CLipSyncFrameData*, CLipSyncFrameData const&);
