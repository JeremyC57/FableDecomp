// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "engine/CCreatureActionBase.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CreaturePointerStorage;
struct TimeConverter { float Convert(int ticks); };

struct CCreatureActionBaseVirtuals {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6();
    virtual int GetTotalTimeTicks() const;
};

float __fastcall CCreatureActionBase_GetTotalTimeSeconds(const CCreatureActionBase* action) {
    TimeConverter* converter = ((TimeConverter*)(((CBaseIntelligentPointer*)(reinterpret_cast<CreaturePointerStorage*>(const_cast<unsigned char*>(action->PCreature))))->GetPItem()));
    const CCreatureActionBaseVirtuals* virtuals =
        reinterpret_cast<const CCreatureActionBaseVirtuals*>(action);
    return converter->Convert(virtuals->GetTotalTimeTicks());
}
