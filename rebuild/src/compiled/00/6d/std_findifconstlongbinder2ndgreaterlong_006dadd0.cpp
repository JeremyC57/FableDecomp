// std::__find_if<const long*, binder2nd<greater<long> > >(const long* first, const long* last, binder2nd<greater<long> > pred, const random_access_iterator_tag&) -- STLport four-way unrolled linear search for the first long greater than the bound value
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DADD0, length 102 bytes (+ int3 padding), `__fastcall` (first in ecx,
// last in edx; the 8-byte binder and the tag reference on the stack = `ret 0Ch`, a /Gr algorithm TU). Single caller
// 0x006D7469 in 0x006D7430 (CTCHeroExperience::GetNextExperienceFallbackMultiplier, this chunk), which pushes the tag's
// address, the multiplier as the binder's value and an uninitialised dword for its empty `greater<long>`. The manifest
// label "std_lower_bound_int" is wrong: the search is linear (STLport `find_if` on a random-access range), not binary.
// The predicate is `*first > value` (`cmp [eax],esi ; jg`), i.e. `bind2nd(greater<long>(), value)`.
// Shape: STLport 4.x `__find_if` verbatim: trip count `(last - first) >> 2` (folded to `sar ecx,4` on the byte
// difference), four `if (pred(*first)) return first; ++first;` per trip, then the `switch (last - first)` fall-through
// for 3 / 2 / 1 left (no step after the last test) and `return last`. Sibling of the landed reverse-iterator copy 0x006DABE0.
namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class Arg1, class Arg2, class Result> struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};

template <class T> struct greater : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x > y; }
};

template <class Operation> class binder2nd {
protected:
    Operation op;
    typename Operation::second_argument_type value;
public:
    binder2nd(const Operation& x, const typename Operation::second_argument_type& y) : op(x), value(y) { }
    typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const { return op(x, value); }
};

template <class RandomAccessIter, class Predicate>
RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred,
                                      const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;

    for ( ; trip_count > 0 ; --trip_count) {
        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;
    }

    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}

}

template const long* __fastcall std::__find_if<const long*, std::binder2nd<std::greater<long> > >(
    const long* first, const long* last, std::binder2nd<std::greater<long> > pred, const std::random_access_iterator_tag&);