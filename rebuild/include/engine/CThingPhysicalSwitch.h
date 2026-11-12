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
#ifdef FABLE_RETAIL_CTHINGPHYSICALSWITCH_TRIGGER
    // Nested opt-in (agentNF12, 2026-11-11, rows 0x0083DDC0 / 0x0083DF20): Ego_r.pdb / FableWin.pdb
    // ?TriggerMe@CThingPhysicalSwitch@@IAEXAAVCThingCreatureBase@@@Z and ?UntriggerMe@CThingPhysicalSwitch@@IAEXXZ (protected; Ego_r donor
    // 0x00812E20 / 0x00812F90 right after PeekDefinition, as retail 0x0083DDC0 / 0x0083DF20 follow 0x0083D900). Declarations only.
protected:
    void TriggerMe(class CThingCreatureBase& triggerer);                // 0x0083DDC0
    void UntriggerMe();                                                  // 0x0083DF20
public:
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICALSWITCH_LEVELSCRIPT
    // Nested opt-in (wf8, 2026-11-12, row 0x0083D990): FableWin.pdb ?ConstructFromLevelScript@CThingPhysicalSwitch@@UAE_NPBD@Z
    // (public virtual, vtable 0x01245644 slot 4). Declaration only; needs FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT.
public:
    virtual bool ConstructFromLevelScript(const char* script);          // 0x0083D990
public:
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICALSWITCH_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsEntitySerialisable() const;  // slot 22, 0x00529C90
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysicalSwitch)==0xd8);
#ifdef FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION_SAMETU
// Nested opt-in (agentNF12, 2026-11-11, rows 0x0083DDC0 TriggerMe / 0x0083DF20 UntriggerMe): the body of PeekDefinition (the landed
// 0x0083D900, same retail TU: Ego_r donor order PeekDefinition / TriggerMe / UntriggerMe) visible to its callers, as in retail, where
// VC7.1 expands the first call of each caller (`call 0x0083E0D0` then the release, then the stale `[edi+90h]` read, 0x0083DF51..0x0083DF71)
// and calls the second (`call 0x0083D900`, 0x0083DFB4). Needs engine/CThingPhysicalSwitchDef.h, engine/CGameDefinitionManager.h and
// FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION / FABLE_RETAIL_CTHING_MOREACCESS. The definition is the landed row's.
inline const CThingPhysicalSwitchDef& CThingPhysicalSwitch::PeekDefinition() const
{
    CDefPointer<const CThingPhysicalSwitchDef> def;
    PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
    return *def;
}
#endif
#endif
