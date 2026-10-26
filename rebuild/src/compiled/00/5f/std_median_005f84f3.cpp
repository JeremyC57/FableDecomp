// std::__median(const pair<long,long>& a, const pair<long,long>& b, const pair<long,long>& c, CKeyPairCompareLess<long,long> comp) -- STLport median of three keyed pairs (the introsort pivot); returns a reference to the median element
// Leaf: ??$__median@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F84F3, 45 bytes, `ret 8` (/Gr TU: a in ecx, b in edx; c and the empty
//   comparator on the stack). Caller: __introsort_loop 0x005FA2F6 (the ExpressionButtons CVectorMap<long,long> sort).
// Label correction: the catalog `std_median_three` is behavioural; the body is STLport `__median(a, b, c, comp)`
//   (byte twin 0x005F84C6, the pair<long,EHeroAbility> instantiation).
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

template const pair<long, long>& __fastcall __median<pair<long, long>, CKeyPairCompareLess<long, long> >(
    const pair<long, long>&, const pair<long, long>&, const pair<long, long>&,
    CKeyPairCompareLess<long, long>);
}