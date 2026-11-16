// std::__copy(CLipSyncFrameData* first, CLipSyncFrameData* last, CLipSyncFrameData* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCLipSyncFrameData@@PAV1@H@std@@YIPAVCLipSyncFrameData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4ADB0, 66 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a4b160 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::resize; binder ??$__copy@PAVCLipSyncFrameData@@PAV1@H@std@@YIPAVCLipSyncFrameData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLipSyncFrameData.
// NF42 (round 5): the local CLipSyncFrameData stand-in (an opaque 12-byte class with an out-of-line operator=) replaced by the shared
// engine/CLipSyncFrameData.h (FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: one CArray<CLipSyncPhoneme> Phonemes member); its implicit
// copy assignment reduces to STLport vector<CLipSyncPhoneme>::operator= (0x00A4A520, declaration-only FABLE_RETAIL_STLP_VECTOR_ASSIGN).
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#include "engine/CLipSyncFrameData.h"
template CLipSyncFrameData* std::__copy<CLipSyncFrameData*,CLipSyncFrameData*,int >(CLipSyncFrameData*, CLipSyncFrameData*, CLipSyncFrameData*, std::random_access_iterator_tag const&, int*);
