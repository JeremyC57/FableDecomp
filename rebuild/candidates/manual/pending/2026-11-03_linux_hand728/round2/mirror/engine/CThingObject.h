#ifndef FABLE_ENGINE_CTHINGOBJECT_H
#define FABLE_ENGINE_CTHINGOBJECT_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// CDefPointer<const CThingObjectDef> PDef +d0, DieOffTimer +d4, DiesOff +d8; native allocation527320.
class CThingObject:public CThingGameObject {
protected:
    unsigned char PDef[4]; // +d0 CDefPointer<const CThingObjectDef>, ctor7031ec
    long DieOffTimer; // +d4, ctor7031f2
    bool DiesOff; // +d8, ctor7031f8
    unsigned char _pad_0xd9[7];
public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingObject)==0xe0);
#endif
