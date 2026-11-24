#ifndef FABLE_ENGINE_CTHINGFILTER_HASINSTANTIATIONNAME_H
#define FABLE_ENGINE_CTHINGFILTER_HASINSTANTIATIONNAME_H
#ifdef FABLE_RETAIL_CTCBATTLECHARGE_CREATEHIT_PASS27
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CThing;
// Ego_r original filter: one reference. FableWin 0x026C8FA0 invokes this ctor.
class CThingFilter_HasInstantiationName : public std::unary_function<CThing,bool> {
public:
    CThingFilter_HasInstantiationName(const CCharString& name) : InstantiationName(name) {}
    bool operator()(const CThing&) const;
private:
    const CCharString& InstantiationName;
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_HasInstantiationName)==4);
#endif
#endif
