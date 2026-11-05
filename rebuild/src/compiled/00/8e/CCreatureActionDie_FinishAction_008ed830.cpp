// CCreatureAction_Die::FinishAction  (retail 0x008ed830, __fastcall, void)
// Leaf: ?FinishAction@CCreatureAction_Die@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).

#include "engine/CThing.h"
struct RetObj
{
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct SubObj
{
};

struct Base
{
    virtual void FinishAction();  // call 0x694430 (base FinishAction on this)
};

class CCreatureAction_Die : public Base
{ public:
    int    filler;   // +4
    SubObj sub;      // +8

    virtual void FinishAction();
};

void CCreatureAction_Die::FinishAction()
{
    Base::FinishAction();
    ((CThing*)(((RetObj*)((CBaseIntelligentPointer*)&(this->sub))->GetPItem())))->Kill(true);
}