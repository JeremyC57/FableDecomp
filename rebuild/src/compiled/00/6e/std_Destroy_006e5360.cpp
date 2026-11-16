// std::_Destroy(CLipSyncPhoneme* first, CLipSyncPhoneme* last) -- destroy [first, last) of lip-sync phonemes (12 bytes, led by the CCharString Symbol)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5360, length 33 bytes (padded to 48).
// Leaf: ??$_Destroy@PAVCLipSyncPhoneme@@@std@@YIXPAVCLipSyncPhoneme@@0@Z
// Name (round 4, NF40): STLport `_Destroy<CLipSyncPhoneme*>(first, last)` emitted out of line (first in ecx, last in edx, no
// tag -> plain `ret`). Codex pass 22 (landed_evidence/00a495f0, NAME_FIXES.tsv) proves the element from the typed call chain:
// the landed vector<CLipSyncPhoneme>::operator= 0x00A4A520 calls this address as ??$_Destroy@PAVCLipSyncPhoneme@@@std@@.., and
// Ego_r.pdb CLipSyncPhoneme is { CCharString Symbol; unsigned long ID; float Weight; } (12 bytes), whose implicit destructor
// reduces to ~CCharString @ 0x0099EAE0 on the leading member. Was the descriptive `_Destroy_StringRecordC` over a local
// SStringRecordC model; now on engine/CLipSyncPhoneme.h.
// Callees: CCharString::~CCharString @ 0x0099EAE0.
// Shape: guarded do-while, `last` in edi, the element address straight into ecx (`mov ecx,esi`).
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "engine/CLipSyncPhoneme.h"

namespace std {
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __fastcall _Destroy<CLipSyncPhoneme*>(CLipSyncPhoneme*, CLipSyncPhoneme*);
}
