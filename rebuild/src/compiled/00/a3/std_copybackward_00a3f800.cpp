// std::__copy_backward(first, last, result, tag, distance) -- Copy a range from its end.
// Leaf: ??$__copy_backward@PAU?$pair@VCCharString@@PAVCSoundBankLH@@@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@PAVCSoundBankLH@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
class CCharString {
public:
    char* Data;
    const CCharString& operator=(const CCharString&);
};
class CSoundBankLH;
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template<class A, class B> struct pair { A first; B second; };
template<class RandomAccessIterator, class BidirectionalIterator, class Distance>
BidirectionalIterator __fastcall __copy_backward(RandomAccessIterator first, RandomAccessIterator last,
    BidirectionalIterator result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
}
template std::pair<CCharString, CSoundBankLH*> * __fastcall std::__copy_backward<std::pair<CCharString, CSoundBankLH*> *, std::pair<CCharString, CSoundBankLH*> *, int>(std::pair<CCharString, CSoundBankLH*> *, std::pair<CCharString, CSoundBankLH*> *, std::pair<CCharString, CSoundBankLH*> *, const std::random_access_iterator_tag&, int*);