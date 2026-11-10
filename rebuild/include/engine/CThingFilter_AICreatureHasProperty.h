#ifndef FABLE_ENGINE_CTHINGFILTER_AICREATUREHASPROPERTY_H
#define FABLE_ENGINE_CTHINGFILTER_AICREATUREHASPROPERTY_H
// CThingFilter_AICreatureHasProperty (chunk 1011, row 0x005FB4B0): Ego_r.pdb layout (4 bytes, `unsigned long Property` +0),
// FableWin.pdb ??0CThingFilter_AICreatureHasProperty@@QAE@K@Z (inline in retail: the caller stores the property into the
// temporary, 0x005FB4EC `mov dword ptr [esp+10h],72h`) and Ego_r.pdb ??RCThingFilter_AICreatureHasProperty@@QBE_NABVCThingAICreature@@@Z
// (out of line in retail at 0x006A75D0, landed). The unary_function base follows the shared CThingFilter_HasTC / _InMap models.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CThingAICreature;
class CThingFilter_AICreatureHasProperty : public std::unary_function<CThingAICreature,bool> {
private:
    unsigned long Property;                                              // +0x00
public:
    CThingFilter_AICreatureHasProperty(unsigned long property) : Property(property) {}
    bool operator()(const CThingAICreature& creature) const;            // 0x006A75D0
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_AICreatureHasProperty)==4);
#endif
