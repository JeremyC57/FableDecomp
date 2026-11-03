#ifndef FABLE_ENGINE_CTHINGVILLAGE_H
#define FABLE_ENGINE_CTHINGVILLAGE_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// CEngineGraphic CurrentGraphic +d0; native allocation52ac70.
class CThingVillage:public CThingGameObject {
protected:
    unsigned char CurrentGraphic[14]; // +d0 CEngineGraphic, size0e; ctor5299ec..529a04
    unsigned char _pad_0xde[2];
public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingVillage)==0xe0);
#endif
