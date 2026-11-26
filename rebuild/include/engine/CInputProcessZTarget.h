#ifndef FABLE_ENGINE_CINPUTPROCESSZTARGET_H
#define FABLE_ENGINE_CINPUTPROCESSZTARGET_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessZTarget : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01238028;
// retail sizeof 0x40 (`push 40h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessZTarget +0x108).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessZTarget@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable and the members below.
// PCreature's and PTarget's base constructor 0x00A01B10 is called out of line, each followed by its CIntelligentPointer vtable (0x01237F14 / 0x01237F1C); PHeroCombatDef is the CDefPointer null constructor (`mov [edi+30h],ebx`); DeactivateHoldTimer is not set.
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CIntelligentPointer.h"   // FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CDefPointer.h"           // FABLE_RETAIL_DEF_CLASS_BASE
class CThing; class CThingPlayerCreature; class CHeroCombatDef;
class CInputProcessZTarget : public CInputProcessGameBase {
public:
    CInputProcessZTarget(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
#ifdef FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
    // Nested opt-in (wf1, row 0x00688D40): Ego_r.pdb ?AddToInterface@CInputProcessZTarget@@UAE_NAAVCInputProcessManager@@@Z (vtable 0x01238028 slot 1). Declaration only.
    virtual bool AddToInterface(CInputProcessManager& manager);          // 0x00688D40
#endif
    CIntelligentPointer<const CThingPlayerCreature> PCreature;   // +0x28
    CDefPointer<const CHeroCombatDef> PHeroCombatDef;              // +0x30
    long DeactivateHoldTimer;                                       // +0x34
    CIntelligentPointer<const CThing> PTarget;                      // +0x38
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    // Opt-in (NF477, r467d / r459a): the retail virtual list, declaration only. Retail vftable 0x01238028, 9 counted function slots
    // (RTTI locator at vftable - 4, then consecutive slots at .text function starts); listed: each slot whose target differs from the
    // base class table, named by the base slot it fills (order from retail, spellings from Ego_r.pdb / FableWin.pdb).
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);           // slot 4, 0x00688EB0
    virtual void Update(CProcessedInput& processed_input);                                           // slot 6, 0x00688C40
public:
#endif
};
FABLE_STATIC_ASSERT_AT(26, sizeof(CInputProcessZTarget) == 0x40);
#endif // FABLE_ENGINE_CINPUTPROCESSZTARGET_H
