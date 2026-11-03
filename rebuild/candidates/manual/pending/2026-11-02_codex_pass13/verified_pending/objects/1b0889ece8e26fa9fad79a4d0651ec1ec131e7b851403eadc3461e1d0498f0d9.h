#ifndef FABLE_ENGINE_CINDEXEDARRAY_H
#define FABLE_ENGINE_CINDEXEDARRAY_H
#include "engine/CArray.h"
#include "stlport/fable_map.h"
// Original Ego_r CIndexedArray<CCharString> TPI0x40eb7: Array then IndexMap, both private.
// Native0098db20: vector at0, map at12; STLport makes full sizeof24.
template<class T> class CIndexedArray {
public:
    void push_back(const T&);
private:
    CArray<T> Array;
    std::map<T,long,std::less<T>,std::allocator<std::pair<const T,long> > > IndexMap;
};
#endif
