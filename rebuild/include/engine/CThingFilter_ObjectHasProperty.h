#ifndef FABLE_ENGINE_CTHINGFILTER_OBJECTHASPROPERTY_H
#define FABLE_ENGINE_CTHINGFILTER_OBJECTHASPROPERTY_H
#ifdef FABLE_RETAIL_GOTOWELL_PASS29
#include "stlport/fable_containers.h"
#include "rebuild_abi.h"
class CThingObject;
// Ego_d enum record and FableWin GoToWell select this property.
enum EObjectProperty { THING_OBJECT_PROPERTY_WELL = 0x800 };
// Ego_r layout and FableWin constructor store a single EObjectProperty.
class CThingFilter_ObjectHasProperty : public std::unary_function<CThingObject,bool> {
    EObjectProperty Property;
public:
    CThingFilter_ObjectHasProperty(EObjectProperty property) : Property(property) {}
    bool operator()(const CThingObject&) const;
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_ObjectHasProperty)==4);
#endif
#endif
