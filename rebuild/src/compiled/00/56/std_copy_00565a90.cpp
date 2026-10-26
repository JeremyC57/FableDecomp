// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (std::pair<float, CWideString>*, std::pair<float, CWideString>*, int) STLport random-access forward copy: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`, returns the advanced result
// Leaf: ??$__copy@PAU?$pair@MVCWideString@@@std@@PAU12@H@std@@YIPAU?$pair@MVCWideString@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00565A90, 61 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element std::pair<float, CWideString> (8 bytes); evidence: its callers are 0x00566630 and 0x00568D40, the vector operator= that CTCHeroOnlineScoreboard::DownloadScoreboardForActivity 0x00563480 applies to a DownloadedClanScoreboard value -- Ego_r.pdb's CTCHeroOnlineScoreboard::DownloadedClanScoreboard maps to vector<pair<float, CWideString> > (H080's 0x0056A370 is that map's hinted insert), the TU's only vector of {4-byte, CWideString} elements
// Label: the manifest's "_Copy_impl<CTCSkeletalMorph::CSkeletalMorph*,CTCSkeletalMorph::CSkeletalMorph*>" is propagated (family H081: six forward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar edx,3`) and 16-byte ones (`sar edx,4`).
// Callee: CWideString::operator= 0x0099B7D0 on `second`.
// Shape: the element's implicit operator= inline -- the first dword through eax, then the member's operator= on +4.
class CWideString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    CWideString& operator=(const CWideString& other);  // 0x0099B7D0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                        // +0x00
    _T2 second;                                       // +0x04
};
}

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template std::pair<float, CWideString>* __fastcall __copy<std::pair<float, CWideString>*, std::pair<float, CWideString>*, int>(std::pair<float, CWideString>*, std::pair<float, CWideString>*, std::pair<float, CWideString>*,
                                                                const random_access_iterator_tag&, int*);
}   // namespace std