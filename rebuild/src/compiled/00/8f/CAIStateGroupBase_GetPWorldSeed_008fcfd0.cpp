#include "engine/CAIStateGroupBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CAIBrain { public: char pad[0x14]; unsigned long base; };

unsigned long* __fastcall CAIStateGroupBase_GetPWorldSeed(const CAIStateGroupBase* stateGroup)
{
    unsigned long v = stateGroup->PBrain->base;
    return (unsigned long*)(v + 0x1613c);
}