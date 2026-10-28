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
    const CCharString& operator=(const CCharString&);
};

template void __fastcall std::fill<std::pair<CCharString, unsigned long> *, std::pair<CCharString, unsigned long>  >(std::pair<CCharString, unsigned long> *, std::pair<CCharString, unsigned long> *, const std::pair<CCharString, unsigned long> &);