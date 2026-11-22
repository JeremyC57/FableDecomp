#ifndef FABLE_ENGINE_CADAPTTHINGCOMPARETOSTLCOMPARITOR_H
#define FABLE_ENGINE_CADAPTTHINGCOMPARETOSTLCOMPARITOR_H
#include "rebuild_abi.h"
// 0096DF50: Ego_r and FableWin template captures its thing comparator by reference.
template<class Comparator> class CAdaptThingCompareToSTLComparitor {
public:
    explicit CAdaptThingCompareToSTLComparitor(const Comparator& comparator) : Functor(&comparator) {}
    template<class T> bool operator()(T* first,T* second) const {
        return (*Functor)(first,second)==first;
    }
private:
    const Comparator* Functor;
};
template<class Comparator>
inline CAdaptThingCompareToSTLComparitor<Comparator> FABLE_FASTCALL AdaptThingCompareToSTLComparitor(const Comparator& comparator) {
    return CAdaptThingCompareToSTLComparitor<Comparator>(comparator);
}
#endif
