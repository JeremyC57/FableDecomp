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
#ifdef FABLE_RETAIL_CTHINGSWITCH_PEEKDEFINITION
    // Retail opt-in (agentNF8, row 0x0083C1E0): Ego_r.pdb ?PeekDefinition@CThingSwitch@@IBE?AV?$CDefPointer@$$CBVCThingSwitchDef@@@@XZ
    // (protected const, the def by value through the hidden pointer; out of line in retail at 0x0083C1E0, called by WorldUpdate
    // 0x0083CD70). Declaration only (needs FABLE_RETAIL_DEF_CLASS_BASE and engine/CDefPointer.h).
protected:
    CDefPointer<const class CThingSwitchDef> PeekDefinition() const;     // 0x0083C1E0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGSWITCH_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsEntitySerialisable() const;  // slot 22, 0x00529B50
public:
    virtual bool IsUsingTriggerRadius() const;  // slot 61, 0x00529B70
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingSwitch)==0xe8);
#endif
