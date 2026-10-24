// CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier(long multiplier) const -- the highest fallback multiplier the given combo multiplier (plus a half while a hit run is going) has reached
// Leaf: ?GetExperienceFallbackMultiplierForMultiplier@CTCHeroExperience@@QBEJJ@Z
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the __find_if body (landed 0x006DABE0 shape, its predicate inline) noinline in the TU with the tag unnamed overlays the result iterator and tag on the dead multiplier slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D73D0, length 82 bytes (+ int3 padding), thiscall `ret 4`. Ego_r.pdb
// `?GetExperienceFallbackMultiplierForMultiplier@CTCHeroExperience@@ABEJJ@Z`; callers include the landed
// UpdateComboMultiplier 0x006D7F20 and GetOnHitSoundIntensity 0x006D86D0 (this chunk). The manifest label
// "CEngine_GetExperienceMultiplier" is propagated and wrong.
// Callees: GetRunningNumHits() const @ 0x006D63A0, and std::__find_if<reverse_iterator<const long*>,
// (anonymous namespace)::CPredIsFallbackMultiplierForMultiplier> @ 0x006DABE0 (landed as "FindReverseIntegerThreshold";
// Ego_r.pdb has `find_if` / `_Find_if` over `reverse_iterator<vector<long>::const_iterator>` with that predicate):
// `__fastcall` with the hidden result in ecx, the tag reference in edx, then first, last and the 4-byte predicate pushed.
// Constant: 0.5f @ 0x0122F59C.
// Layout: CTCHeroExperience retail (PDef +0x54); CHeroExperienceDef retail FallbackMultipliers vector<long> +0x40.
// Shape: `float m = multiplier; if (GetRunningNumHits() > 0) m += 0.5f; return *find_if(rbegin(), rend(), CPred(m));`
// -- the float is homed in the dead argument slot, which then also takes the result iterator and the
// random_access_iterator_tag temporary; the STLport reverse_iterator copies (user copy constructor) are built in place in
// the pushed argument words; `*it` is `*--tmp` (`[base-4]`).
namespace std {

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class It> struct iterator_traits {
    typedef typename It::iterator_category iterator_category;
};

template <class It> class reverse_iterator {
public:
    typedef random_access_iterator_tag iterator_category;
protected:
    It current;
public:
    explicit reverse_iterator(It x) : current(x) { }
    reverse_iterator(const reverse_iterator& x) : current(x.current) { }
    It base() const { return current; }
    const long& operator*() const { It tmp = current; return *--tmp; }
    reverse_iterator& operator++() { --current; return *this; }
    friend int operator-(const reverse_iterator& x, const reverse_iterator& y) { return y.current - x.current; }
};

// __find_if @ 0x006DABE0 (landed body shape): defined in the TU, never inlined, the tag parameter unnamed (GOTCHAS
// empty-tag rule) -- VC7.1 then overlays the result iterator and the tag on the dead `multiplier` slot as retail.
#pragma auto_inline(off)
template <class RandomAccessIter, class Predicate>
__declspec(noinline) RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred,
                                                           const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
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
#pragma auto_inline(on)

template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, typename iterator_traits<InputIter>::iterator_category());
}

template <class T> class vector {
public:
    typedef const T* const_iterator;
    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
};

}

namespace {

class CPredIsFallbackMultiplierForMultiplier {
public:
    float Multiplier;
    CPredIsFallbackMultiplierForMultiplier(float multiplier) : Multiplier(multiplier) { }
    bool operator()(long fallback) const;                   // inlined into 0x006DABE0
};

extern const float gTolerance_0129ba3c;
inline bool CPredIsFallbackMultiplierForMultiplier::operator()(long fallback) const
{
    return (float)fallback < 2.0f || (float)fallback - Multiplier < -gTolerance_0129ba3c;
}

}

class CHeroExperienceDef {
public:
    unsigned char _base[0x28];                              // CDefClassBase
    std::vector<long> ExperienceLevelThresholds;            // +0x28
    std::vector<long> HeroLevelTitles;                      // +0x34
    std::vector<long> FallbackMultipliers;                  // +0x40
};

class CTCHeroExperience {
public:
    virtual void v00();
    unsigned char _pad04[0x50];                             // ParentThing .. AttackHistory
    CHeroExperienceDef* PDef;                               // +0x54

    long GetRunningNumHits() const;                         // 0x006D63A0
    long GetExperienceFallbackMultiplierForMultiplier(long multiplier) const;
};

long CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier(long multiplier) const
{
    float m = (float)multiplier;
    if (GetRunningNumHits() > 0)
        m += 0.5f;
    return *std::find_if(PDef->FallbackMultipliers.rbegin(), PDef->FallbackMultipliers.rend(),
                         CPredIsFallbackMultiplierForMultiplier(m));
}