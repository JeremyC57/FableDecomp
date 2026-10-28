#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// std::fill(first, last, value) -- Assign a value to each element in a range.
// Leaf: ??$fill@PAU?$pair@VCCharString@@K@std@@U12@@std@@YIXPAU?$pair@VCCharString@@K@0@0ABU10@@Z
// Flags: /Gr
namespace std {
template<class A, class B> struct pair { A first; B second; };
template<class Iterator, class T>
void __fastcall fill(Iterator first, Iterator last, const T& value)
{
    for (; first != last; ++first) *first = value;
}
}
class CCharString {
public:
    char* Data;
    CCharString& operator=(const CCharString&);
};

template void __fastcall std::fill<std::pair<CCharString, unsigned long> *, std::pair<CCharString, unsigned long>  >(std::pair<CCharString, unsigned long> *, std::pair<CCharString, unsigned long> *, const std::pair<CCharString, unsigned long> &);
static int assignments;
CCharString& CCharString::operator=(const CCharString& rhs) { ++assignments;Data=rhs.Data;return *this; }
int main() {
    typedef std::pair<CCharString, unsigned long>  Item;
    Item a[5], value;char oldText[]="old",newText[]="new";
    value.first.Data=newText;value.second=17;
    for(int i=0;i<5;++i){a[i].first.Data=oldText;a[i].second=3;}
    std::fill(a+1,a+4,value);
    int ok=assignments==3 && a[0].first.Data==oldText && a[4].second==3;
    for(int j=1;j<4;++j)ok=ok && a[j].first.Data==newText && a[j].second==17;
    std::fill(a+2,a+2,value);ok=ok && assignments==3;
    std::fill(a+1,a+4,a[2]);ok=ok && assignments==6 && a[3].second==17;
    puts(ok ? "00ae5820_TEST PASS" : "00ae5820_TEST FAIL");return ok ? 0 : 1;
}