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
#ifdef FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION
    // Retail opt-in (agentNF9, row 0x0083D900): Ego_r.pdb ?PeekDefinition@CThingPhysicalSwitch@@IBEABVCThingPhysicalSwitchDef@@XZ
    // (protected const, returns a reference), out of line in retail at 0x0083D900: its only callers are TriggerMe 0x0083DDC0 and
    // UntriggerMe 0x0083DF20, and it calls GetDef<CThingPhysicalSwitchDef> 0x0083E0D0 (FableWin.pdb spelling), which those two also call.
    // Declaration only (CThingPhysicalSwitchDef forward-declared).
protected:
    const class CThingPhysicalSwitchDef& PeekDefinition() const;         // 0x0083D900
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysicalSwitch)==0xd8);
#endif
