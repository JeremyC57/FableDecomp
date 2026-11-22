#ifndef FABLE_ENGINE_CKEYPAIRCOMPAREGREATER_H
#define FABLE_ENGINE_CKEYPAIRCOMPAREGREATER_H
#ifdef FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26
#include "stlport/fable_containers.h"
// Both original PDBs name this comparator for CAIBrain::StateGroupList.
template<class K,class V> class CKeyPairCompareGreater {
public:
    typedef std::pair<K,V> TData;
    bool operator()(const TData& first,const TData& second) const { return first.first>second.first; }
};
#endif
#endif
