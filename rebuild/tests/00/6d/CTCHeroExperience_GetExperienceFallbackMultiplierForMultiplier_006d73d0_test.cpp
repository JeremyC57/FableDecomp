// Behaviour test for 0x006D73D0 CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier(multiplier): scanning
// the def's fallback multipliers from the back, the first one below the multiplier (plus 0.5 during a hit run) or below 2.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
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
const float gTolerance_0129ba3c = 0.001f;
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

static long g_hits = 0;
void CTCHeroExperience::v00() { }
long CTCHeroExperience::GetRunningNumHits() const { return g_hits; }

long CTCHeroExperience::GetExperienceFallbackMultiplierForMultiplier(long multiplier) const
{
    float m = (float)multiplier;
    if (GetRunningNumHits() > 0)
        m += 0.5f;
    return *std::find_if(PDef->FallbackMultipliers.rbegin(), PDef->FallbackMultipliers.rend(),
                         CPredIsFallbackMultiplierForMultiplier(m));
}


int main()
{
    long fallbacks[5] = { 1, 2, 4, 8, 16 };
    CHeroExperienceDef* def = (CHeroExperienceDef*)malloc(sizeof(CHeroExperienceDef));
    def->FallbackMultipliers._M_start = fallbacks; def->FallbackMultipliers._M_finish = fallbacks + 5;
    def->FallbackMultipliers._M_end_of_storage = fallbacks + 5;
    CTCHeroExperience* xp = (CTCHeroExperience*)malloc(sizeof(CTCHeroExperience));
    memset(xp, 0, sizeof(*xp)); xp->PDef = def;
    int ok = 1;
    g_hits = 0;
    if (xp->GetExperienceFallbackMultiplierForMultiplier(5) != 4) ok = 0;
    if (xp->GetExperienceFallbackMultiplierForMultiplier(4) != 2) ok = 0;    // 4 is not below 4
    if (xp->GetExperienceFallbackMultiplierForMultiplier(100) != 16) ok = 0;
    if (xp->GetExperienceFallbackMultiplierForMultiplier(0) != 1) ok = 0;    // below 2 always qualifies
    g_hits = 3;
    if (xp->GetExperienceFallbackMultiplierForMultiplier(4) != 4) ok = 0;    // 4.5 during a hit run
    if (xp->GetExperienceFallbackMultiplierForMultiplier(16) != 16) ok = 0;
    printf(ok ? "006d73d0_TEST PASS\n" : "006d73d0_TEST FAIL\n");
    return ok ? 0 : 1;
}