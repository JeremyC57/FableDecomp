// CTCHeroExperience::GetNextExperienceFallbackMultiplier() const -- the first fallback multiplier above the current pickup multiplier (the last fallback when none is)
// Leaf: ?GetNextExperienceFallbackMultiplier@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D7430, length 84 bytes (+ int3 padding), thiscall plain `ret`.
// Ego_r.pdb `?GetNextExperienceFallbackMultiplier@CTCHeroExperience@@ABEJXZ`; caller 0x006D86D6 (GetOnHitSoundIntensity,
// this chunk). The manifest label "Combat_GetNextFlourishLevelAsFloat" is descriptive and wrong (it returns a long).
// Callees: __ftol2 @ 0x00BFEA70, std::__find_if<const long*, binder2nd<greater<long> > > @ 0x006DADD0 (this chunk;
// `__fastcall` first/last in ecx/edx, the binder and the tag reference pushed). Constant: 0.5 (double) @ 0x01237F00.
// Layout: CTCHeroExperience retail (ExperienceMultiplierParam +0x30, PDef +0x54); CHeroExperienceDef retail:
// FallbackMultipliers vector<long> +0x40 (12-byte STLport vector), ExperienceMultiplierParamPerUnitComboMultiplier +0x64.
// Shape: `find_if(begin, end, bind2nd(greater<long>(), GetExperiencePickUpMultiplier()))` (STLport `find_if` inline
// forwarding to `__find_if` with a `random_access_iterator_tag` temporary); `if (it != end()) return *it; return back();`. The
// GetExperiencePickUpMultiplier inline is the landed 0x006D7F20's. __find_if's body sits above under
// `#pragma auto_inline(off)` (retail calls it).
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

#pragma auto_inline(off)
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
#pragma auto_inline(on)

template <class T> inline random_access_iterator_tag __ITERATOR_CATEGORY(const T*) { return random_access_iterator_tag(); }

template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, __ITERATOR_CATEGORY(first));
}

template <class Operation, class T>
inline binder2nd<Operation> bind2nd(const Operation& op, const T& x)
{
    return binder2nd<Operation>(op, typename Operation::second_argument_type(x));
}

template <class T> class vector {
public:
    typedef const T* const_iterator;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    const T& back() const { return *(end() - 1); }
};

}


class CHeroExperienceDef {
public:
    unsigned char _base[0x28];                              // CDefClassBase
    std::vector<long> ExperienceLevelThresholds;            // +0x28
    std::vector<long> HeroLevelTitles;                      // +0x34
    std::vector<long> FallbackMultipliers;                  // +0x40
    unsigned char SoundIntensityThresholds[0xC];            // +0x4C
    unsigned char ComboMultiplierMap[0xC];                  // +0x58
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
};

class CTCHeroExperience {
public:
    virtual void v00();
    void* ParentThing;                                      // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0C
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    long* StatExperiencePoints[3];                          // +0x18
    long* TrainableStatLevels[3];                           // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3C
    void* Pickups;                                          // +0x40
    long* ExperienceSpentOnStats[3];                        // +0x44
    void* AttackHistory;                                    // +0x50
    CHeroExperienceDef* PDef;                               // +0x54

    long GetExperiencePickupMultiplierForParam(float param) const
    {
        long multiplier = (long)(param / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5);
        if (multiplier < 1)
            multiplier = 1;
        return multiplier;
    }
    long GetExperiencePickUpMultiplier() const { return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam); }
    long GetNextExperienceFallbackMultiplier() const;
};

long CTCHeroExperience::GetNextExperienceFallbackMultiplier() const
{
    std::vector<long>::const_iterator it = std::find_if(PDef->FallbackMultipliers.begin(), PDef->FallbackMultipliers.end(),
                                                        std::bind2nd(std::greater<long>(), GetExperiencePickUpMultiplier()));
    if (it != PDef->FallbackMultipliers.end())
        return *it;
    return PDef->FallbackMultipliers.back();
}