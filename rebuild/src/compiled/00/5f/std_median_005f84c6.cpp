// std::__median(const pair<long,EHeroAbility>& a, const pair<long,EHeroAbility>& b, const pair<long,EHeroAbility>& c, CKeyPairCompareLess<long,EHeroAbility> comp) -- STLport median of three keyed pairs (the introsort pivot); returns a reference to the median element
// Leaf: ??$__median@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F84C6, 45 bytes, `ret 8` (/Gr TU: a in ecx, b in edx; c and the empty
//   comparator on the stack). Caller: __introsort_loop 0x005FA26B (the AbilityButtons CVectorMap<long,EHeroAbility> sort).
// Label correction: the catalog `std_MinOfThree` is behavioural; the body is STLport `__median(a, b, c, comp)`
//   (byte twin 0x005F84F3, the pair<long,long> instantiation).
// Shape: the three keys are loaded first (c -> ebx, a -> esi, b -> edi), then the five signed compares in STLport's
//   order; eax starts as &c and is replaced by &a / &b on the other exits; one shared epilogue (`jmp`) = size TU,
//   `#pragma optimize("s", on)`.
#pragma optimize("s", on)
enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class T, class Compare>
const T& __fastcall __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

template const pair<long, EHeroAbility>& __fastcall __median<pair<long, EHeroAbility>, CKeyPairCompareLess<long, EHeroAbility> >(
    const pair<long, EHeroAbility>&, const pair<long, EHeroAbility>&, const pair<long, EHeroAbility>&,
    CKeyPairCompareLess<long, EHeroAbility>);
}