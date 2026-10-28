#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// std::__copy_backward(first, last, result, tag, distance) -- Copy a range from its end.
// Leaf: ??$__copy_backward@PAU?$pair@VCCharString@@K@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@K@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
class CCharString {
public:
    char* Data;
    CCharString& operator=(const CCharString&);
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
template std::pair<CCharString, unsigned long> * __fastcall std::__copy_backward<std::pair<CCharString, unsigned long> *, std::pair<CCharString, unsigned long> *, int>(std::pair<CCharString, unsigned long> *, std::pair<CCharString, unsigned long> *, std::pair<CCharString, unsigned long> *, const std::random_access_iterator_tag&, int*);

static int assignments;
CCharString& CCharString::operator=(const CCharString& rhs) { ++assignments; Data=rhs.Data; return *this; }
int main() {
    typedef std::pair<CCharString, unsigned long>  Item;
    Item a[6]; char values[6];
    for(int i=0;i<6;++i){ a[i].first.Data=values+i; a[i].second=i+10; }
    std::random_access_iterator_tag tag;
    Item* result=std::__copy_backward(a,a+4,a+6,tag,(int*)0);
    int ok=result==a+2 && assignments==4 && a[0].first.Data==values && a[1].first.Data==values+1;
    for(int j=0;j<4;++j)ok=ok && a[j+2].first.Data==values+j && a[j+2].second==j+10;
    result=std::__copy_backward(a,a,a+3,tag,(int*)0);ok=ok && result==a+3 && assignments==4;
    result=std::__copy_backward(a+2,a+6,a+6,tag,(int*)0);ok=ok && result==a+2 && assignments==8;
    puts(ok ? "009b1e20_TEST PASS" : "009b1e20_TEST FAIL");return ok ? 0 : 1;
}