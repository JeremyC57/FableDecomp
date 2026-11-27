#ifndef FABLE_ENGINE_CFIREEFFECTCREATIONSET_H
#define FABLE_ENGINE_CFIREEFFECTCREATIONSET_H
#include "engine/CInitBaseClass.h"
#include "engine/CDefString.h"
#include "stlport/fable_map.h"
template<class T> class CCompareDefString {
public:
    bool operator()(const CDefString& a, const CDefString& b) const { return a.TablePos < b.TablePos; }
};
class CFireEffectCreationSet : public CInitBaseClass {
public:
    typedef std::map<CDefString, long, CCompareDefString<long> > TContainmentMap;
    TContainmentMap ContainmentMap;
};
#endif
