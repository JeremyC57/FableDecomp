// std::__copy_backward(CSubtitledMessage* first, CSubtitledMessage* last, CSubtitledMessage* result, const random_access_iterator_tag&, int*) -- assign [first, last) onto the range ending at result, back to front
// Leaf: ??$__copy_backward@PAVCSubtitledMessage@@PAV1@H@std@@YIPAVCSubtitledMessage@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00719F50, 67 bytes, `__fastcall` (first ecx, last edx; result, the tag and the distance
//   pointer on the stack), `ret 0Ch`.
// Identity (proven): the per-element call is the landed CSubtitledMessage::operator=(const CSubtitledMessage&) 0x00719E80 and the stride is
//   sizeof(CSubtitledMessage) = 68 (the reciprocal divide and the `sub 0x44` steps); callers 0x00719FA0 / 0x0071A2F0 (vector<CSubtitledMessage> insertion). The manifest's
//   "_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*...>" is a propagated (later-STL) label.
// Callees: CSubtitledMessage::operator=(const CSubtitledMessage&) 0x00719E80.
// Shape (family M108, representative 0x00513D40): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;`.
class CSubtitledMessage {                                             // 68 bytes in retail
public:
    CSubtitledMessage& operator=(const CSubtitledMessage& other);                 // 0x00719E80
    unsigned long Members[17];
};
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template ::CSubtitledMessage* __fastcall __copy_backward<::CSubtitledMessage*, ::CSubtitledMessage*, int>(::CSubtitledMessage*, ::CSubtitledMessage*, ::CSubtitledMessage*, const random_access_iterator_tag&, int*);
}