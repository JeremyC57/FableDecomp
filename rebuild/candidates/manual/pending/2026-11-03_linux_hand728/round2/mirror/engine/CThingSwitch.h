#ifndef FABLE_ENGINE_CTHINGSWITCH_H
#define FABLE_ENGINE_CTHINGSWITCH_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// TriggerRadius, EnvironmentDef, TimeToChangeEnvironmentDef, Triggered, TriggerableObjectScriptName, TimesTriggered; native allocation52ace0.
class CThingSwitch:public CThingGameObject {
protected:
    float TriggerRadius; // +d0 getter529b80/setter529b90
    CCharString EnvironmentDef; // +d4 ctor529b24
    float TimeToChangeEnvironmentDef; // +d8
    bool Triggered; // +dc
    unsigned char _pad_0xdd[3];
    CCharString TriggerableObjectScriptName; // +e0 ctor529b35
    long TimesTriggered; // +e4; complete nativee8
public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingSwitch)==0xe8);
#endif
