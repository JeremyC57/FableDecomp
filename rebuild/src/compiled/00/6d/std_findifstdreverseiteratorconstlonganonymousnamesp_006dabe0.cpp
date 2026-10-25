// std::__find_if<std::reverse_iterator<const long*>, `anonymous namespace'::CPredIsFallbackMultiplierForMultiplier>(
// first, last, pred, const random_access_iterator_tag&) -- STLport random-access find_if (4-way unrolled, switch tail)
// walking a vector<long> backwards for the first fallback multiplier the predicate accepts
// Leaf: ??$__find_if@V?$reverse_iterator@PBJ@std@@VCPredIsFallbackMultiplierForMultiplier@?A
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DABE0, length 488 bytes, `ret 0Ch`.
// Identity (FINDINGS chunk 387 agent table; was the descriptive "FindReverseIntegerThreshold (owner uncertain)" with a
//   private reverse-iterator struct): Ego_r.pdb / FableWin.pdb name find_if<reverse_iterator<vector<long>::
//   const_iterator>, (anon)::CPredIsFallbackMultiplierForMultiplier> (Dinkumware spelling; STLport's const_reverse_iterator
//   is reverse_iterator<const long*>) in the CTCHeroExperience TU, with ??0CPredIsFallbackMultiplierForMultiplier@?A..@@
//   QAE@M@Z and ??R...@@QBE_NJ@Z (operator()(long) const); struct_layouts: CPredIsFallbackMultiplierForMultiplier is
//   4 bytes {float Multiplier}. Callers: CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier 0x006D73D0
//   (0x006D7412; PDB ?GetExperienceFallbackMultiplierForMultiplier@CTCHeroExperience@@ABEJJ@Z) and 0x006DB310.
// Shape: /Gr TU (`__fastcall`: the hidden result in ecx, the empty tag reference in edx, the two 4-byte reverse
//   iterators and the 4-byte predicate on the stack). STLport 4.x unrolls by four (`(last - first) >> 2` = the byte
//   difference of the bases `sar 4`), the remainder is the fall-through `switch` (3, 2, 1 via `dec eax`). The
//   reverse_iterator dereference is `*--tmp` (base[-1]); the predicate is `fallback < 2.0f || fallback - Multiplier <
//   -epsilon` (2.0f @ 0x0122DCDC, epsilon 1e-4f @ 0x0129BA3C).
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

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
