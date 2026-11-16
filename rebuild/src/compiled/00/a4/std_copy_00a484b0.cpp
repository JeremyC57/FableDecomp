// std::__copy(const CLipSyncPhoneme* first, const CLipSyncPhoneme* last, CLipSyncPhoneme* result, const random_access_iterator_tag&, int*) -- SGI counted forward copy of lip-sync phonemes (string operator=, ID, Weight), returns the advanced result
// Leaf: ??$__copy@PBVCLipSyncPhoneme@@PAV1@H@std@@YIPAVCLipSyncPhoneme@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A484B0, length 78 bytes, `ret 0Ch`.
// Callers: 0x00A489AC, 0x00A4A5E1 and 0x00A4A60A (the vector<CLipSyncPhoneme> erase / insert / assign helpers of the lip-sync TU). Callee: CCharString::operator= 0x0099EFB0.
// Identity: the catalog's `_Move<CLipSyncPhoneme*,CLipSyncPhoneme*>` names the element right (Ego_r.pdb CLipSyncPhoneme,
//   12 bytes: Symbol CCharString +0, ID unsigned long +4, Weight float +8) but the body is SGI/STLport `__copy` with the
//   random-access tag and the Distance* (`ret 0Ch`). Byte twin of 0x00A48870 (a second instantiation in the same lip-sync
//   module; same source).
// Name (round 4, NF40): the const-source instance __copy<const CLipSyncPhoneme*, CLipSyncPhoneme*, int> (Codex pass 22
//   NAME_FIXES.tsv, landed_evidence/00a495f0; the landed vector<CLipSyncPhoneme>::operator= 0x00A4A520 binds this spelling here);
//   the element now comes from engine/CLipSyncPhoneme.h instead of a local model.
// Shape: /Gr TU (`__fastcall` template: first in ecx -> esi, last in edx; result, the tag reference and the null Distance*
//   on the stack); `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }` with the signed count from
//   the 0x2AAAAAAB reciprocal multiply (/ 12); the phoneme's implicit operator= is the string's out-of-line operator=
//   then two dword moves (a float copied by assignment is an integer move).
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "engine/CLipSyncPhoneme.h"

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CLipSyncPhoneme* __fastcall __copy(const CLipSyncPhoneme*, const CLipSyncPhoneme*, CLipSyncPhoneme*, const random_access_iterator_tag&, int*);
}