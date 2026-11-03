#ifndef FABLE_ENGINE_CTCTHINGOWNER_H
#define FABLE_ENGINE_CTCTHINGOWNER_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
// Ego_r TPI CT CThingOwner derives CTCBase, ObjectList +12; retail loop
// 00666f90..00667055 dereferences this exact circular STLport list.
class CTCThingOwner : public CTCBase {
private:
    std::list<CIntelligentPointer<CThing> > ObjectList;
    bool CallOnDie;
public:
    const std::list<CIntelligentPointer<CThing> >& GetObjectList() const { return ObjectList; }
    std::list<CIntelligentPointer<CThing> >& GetObjectList() { return ObjectList; }
};
FABLE_STATIC_ASSERT(sizeof(CTCThingOwner)==0x14);
#endif
