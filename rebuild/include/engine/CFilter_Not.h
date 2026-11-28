#ifndef FABLE_ENGINE_CFILTER_NOT_H
#define FABLE_ENGINE_CFILTER_NOT_H

#include "fable_compiler.h"

#ifdef FABLE_RETAIL_CREATURE_NAV_FLEE_COVER_PASS30
// The sole reference F is at offset zero in the Ego_r template record.
// FableWin 0x024812A0 initializes the reference and 0x02482400 negates its predicate.
template<class Filter>
class CFilter_Not {
protected:
    const Filter& F;
public:
    explicit CFilter_Not(const Filter& filter) : F(filter) {}
    template<class T> bool operator()(const T& thing) const { return !F(thing); }
};

template<class Filter>
inline CFilter_Not<Filter> FABLE_FASTCALL GFFilter_NOT(const Filter& filter)
{
    return CFilter_Not<Filter>(filter);
}
#endif

#endif
