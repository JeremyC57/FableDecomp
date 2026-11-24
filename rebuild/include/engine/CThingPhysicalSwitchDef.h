#ifndef FABLE_ENGINE_CTHINGPHYSICALSWITCHDEF_H
#define FABLE_ENGINE_CTHINGPHYSICALSWITCHDEF_H
// CThingPhysicalSwitchDef (new shared header, agentNF9, rows 0x0083D900 / 0x0083DDC0 / 0x0083DF20): Ego_r.pdb CThingPhysicalSwitchDef
// (struct_layouts_egor.tsv, 0x98 bytes; class key V; RTTI .?AVCThingPhysicalSwitchDef@@ : CThingBaseDef) on the retail CThingBaseDef
// (0x70, Ego_r 0x74): TriggerType +0x70, UntriggeredGraphic +0x74, TriggeredGraphic +0x82 (two byte-packed 14-byte CEngineGraphic),
// TriggerSound +0x90 (Ego_r +0x74 / +0x78 / +0x86 / +0x94). The enum's enumerators are in no name set on disk.
#include "engine/CThingBaseDef.h"
#include "engine/CEngineGraphic.h"
enum EPSwitchTriggerType { };                                            // Ego_r.pdb enum (4 bytes); enumerators unproven
class CThingPhysicalSwitchDef : public CThingBaseDef {
public:
    EPSwitchTriggerType TriggerType;                                     // +0x70
    CEngineGraphic UntriggeredGraphic;                                   // +0x74
    CEngineGraphic TriggeredGraphic;                                     // +0x82
    long TriggerSound;                                                   // +0x90
#ifdef FABLE_RETAIL_CTHINGPHYSICALSWITCHDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CThingPhysicalSwitchDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICALSWITCHDEF_COPY_DECLS
    // Opt-in (NF337, 2026-11-24): declarations only, no layout change, for CThingPhysicalSwitchDef::Copy 0x00464531, which tail-calls the out-of-line
    // operator= (retail `jmp` 0x00464536). ?Copy@CThingPhysicalSwitchDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at 0x00464531 (slot 19; it overrides CDefClassBase::Copy: clang override probe, made-up-name
    // control fails); ??4CThingPhysicalSwitchDef@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb) is the operator= at 0x00464536.
public:
    virtual void Copy(const CDefClassBase*);
    CThingPhysicalSwitchDef& operator=(const CThingPhysicalSwitchDef&);
#endif
};
#ifdef FABLE_RETAIL_CTHINGBASEDEF
FABLE_STATIC_ASSERT_AT(18, sizeof(CThingPhysicalSwitchDef)==0x94);
#endif
#endif
