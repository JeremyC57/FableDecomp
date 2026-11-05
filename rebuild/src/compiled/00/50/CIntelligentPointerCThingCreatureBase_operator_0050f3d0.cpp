// CIntelligentPointer<CThingCreatureBase>::operator= (retail 0x0050f3d0)
// Leaf: ??4CIntelligentPointer_CThingCreatureBase@@AAEPAV0@PAV0@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CThingCreatureBase;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CIntelligentPointer_CThingCreatureBase
{

    CIntelligentPointer_CThingCreatureBase* operator=(
        CIntelligentPointer_CThingCreatureBase* rhs);
};

CIntelligentPointer_CThingCreatureBase*
CIntelligentPointer_CThingCreatureBase::operator=(
    CIntelligentPointer_CThingCreatureBase* rhs)
{
    ((CBaseIntelligentPointer*)this)->SetPItem((CBaseObject*)(((CThingCreatureBase*)((CBaseIntelligentPointer*)rhs)->GetPItem())));
    return this;
}