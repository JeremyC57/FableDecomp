#ifndef FABLE_ENGINE_CTHINGPHYSICALSWITCH_H
#define FABLE_ENGINE_CTHINGPHYSICALSWITCH_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// Triggered +d0, TriggerableObjectScriptName +d4; native allocation52ad50.
class CThingPhysicalSwitch:public CThingGameObject {
protected:
    bool Triggered; // +d0, original field immediately after full base
    unsigned char _pad_0xd1[3];
    CCharString TriggerableObjectScriptName; // +d4, ctor52ad86
public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysicalSwitch)==0xd8);
#endif
