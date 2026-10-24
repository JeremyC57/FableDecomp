// _STL::__copy(CLipSyncPhoneme* first, CLipSyncPhoneme* last, CLipSyncPhoneme* result, const random_access_iterator_tag&, int*) -- SGI counted forward copy of lip-sync phonemes (string operator=, ID, Weight), returns the advanced result
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A48870, length 78 bytes, `ret 0Ch`.
// Callers: 0x00A49BAC and 0x00A4A339 (vector<CLipSyncPhoneme> helpers). Callee: CCharString::operator= 0x0099EFB0.
// Identity: the catalog's `_Move<CLipSyncPhoneme*,CLipSyncPhoneme*>` names the element right (Ego_r.pdb CLipSyncPhoneme,
//   12 bytes: Symbol CCharString +0, ID unsigned long +4, Weight float +8) but the body is SGI/STLport `__copy` with the
//   random-access tag and the Distance* (`ret 0Ch`). Byte twin of 0x00A484B0 (a second instantiation in the same lip-sync
//   module; same source).
// Shape: /Gr TU (`__fastcall` template: first in ecx -> esi, last in edx; result, the tag reference and the null Distance*
//   on the stack); `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }` with the signed count from
//   the 0x2AAAAAAB reciprocal multiply (/ 12); the phoneme's implicit operator= is the string's out-of-line operator=
//   then two dword moves (a float copied by assignment is an integer move).
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CLipSyncPhoneme {                              // 12 bytes
public:
    CCharString   Symbol;                            // +0x00
    unsigned long ID;                                // +0x04
    float         Weight;                            // +0x08
};

namespace _STL {
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

template CLipSyncPhoneme* __fastcall __copy(CLipSyncPhoneme*, CLipSyncPhoneme*, CLipSyncPhoneme*, const random_access_iterator_tag&, int*);
}