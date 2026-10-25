// Test for 0x006DABE0 std::__find_if<reverse_iterator<const long*>, (anon)::CPredIsFallbackMultiplierForMultiplier>.
// The predicate has internal linkage (anonymous namespace), so the instantiation cannot be linked from another TU: this
// test compiles the same STLport model text (reverse_iterator, the predicate, the unrolled __find_if) and checks that
// the backward walk returns the first value (from the back) that is < 2 or below Multiplier - epsilon, else `last`,
// for every range length 0..9 (all unrolled and switch-tail paths).
#include <stdio.h>
extern const float g_epsilon_0129ba3c = 1e-4f;
namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _Iterator> struct iterator_traits { };
template <class _Tp> struct iterator_traits<const _Tp*> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp                        value_type;
    typedef int                        difference_type;
    typedef const _Tp*                 pointer;
    typedef const _Tp&                 reference;
};

template <class _Iterator>
class reverse_iterator {
protected:
    _Iterator current;
    typedef reverse_iterator<_Iterator> _Self;
public:
    typedef typename iterator_traits<_Iterator>::difference_type difference_type;
    typedef typename iterator_traits<_Iterator>::reference reference;
    reverse_iterator() { }
    explicit reverse_iterator(_Iterator __x) : current(__x) { }
    reverse_iterator(const _Self& __x) : current(__x.current) { }
    _Iterator base() const { return current; }
    reference operator*() const { _Iterator __tmp = current; return *--__tmp; }
    _Self& operator++() { --current; return *this; }
};

template <class _Iterator>
inline typename reverse_iterator<_Iterator>::difference_type
operator-(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y)
{
    return __y.base() - __x.base();
}
}

namespace {
class CPredIsFallbackMultiplierForMultiplier {           // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    CPredIsFallbackMultiplierForMultiplier(float multiplier) : Multiplier(multiplier) { }
    bool operator()(long fallback_multiplier) const
    {
        return (float)fallback_multiplier < 2.0f || (float)fallback_multiplier - Multiplier < -g_epsilon_0129ba3c;
    }
    float Multiplier;                                    // +0x00
};
}

namespace std {
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    typename iterator_traits<const long*>::difference_type __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}

template reverse_iterator<const long*> __fastcall __find_if(reverse_iterator<const long*>, reverse_iterator<const long*>,
    CPredIsFallbackMultiplierForMultiplier, const random_access_iterator_tag&);
}

int main()
{
    static const long values[9] = { 7, 9, 1, 8, 6, 5, 3, 4, 10 };
    bool ok = true;
    for (int n = 0; n <= 9; ++n) {
        for (int m = 2; m <= 11; ++m) {
            std::reverse_iterator<const long*> first(values + n), last(values);
            std::random_access_iterator_tag tag;
            std::reverse_iterator<const long*> r = std::__find_if(first, last, CPredIsFallbackMultiplierForMultiplier((float)m), tag);
            int expect = 0;                                  // base of the hit (index + 1), 0 = last
            for (int i = n - 1; i >= 0; --i)
                if (values[i] < 2 || (float)values[i] - (float)m < -1e-4f) { expect = i + 1; break; }
            if (r.base() != values + expect) ok = false;
        }
    }
    if (!ok) { puts("006dabe0_TEST FAIL"); return 1; }
    puts("006dabe0_TEST PASS");
    return 0;
}
