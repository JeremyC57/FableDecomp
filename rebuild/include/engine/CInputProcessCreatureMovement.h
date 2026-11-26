#ifndef FABLE_ENGINE_CINPUTPROCESSCREATUREMOVEMENT_H
#define FABLE_ENGINE_CINPUTPROCESSCREATUREMOVEMENT_H
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CInputProcessCreatureMovement : CInputProcessGameBase (ghidra_out/rtti_bases.tsv), vtable 0x01237F4C;
// retail sizeof 0x30 (`push 30h ; call operator new` before the construction in InitInterfaces, CPlayer::InputProcessCreatureMovement +0x150).
// The constructor is inline in retail (FableWin.pdb ??0CInputProcessCreatureMovement@@QAE@ABVCInputProcessGameBaseInit@@@Z, no Ego_r.pdb or retail
// out-of-line copy): the base constructor (0x00687A30), then this vtable and the members below.
// PCreature default-constructed (0x00A01B10, then vtable 0x01237F14).
// Member names: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Virtuals other than the inherited destructor are not modelled.
#include "rebuild_abi.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CIntelligentPointer.h"   // FABLE_RETAIL_CINTELLIGENTPOINTER
class CThingPlayerCreature;
#ifdef FABLE_RETAIL_INPUTPROCESS_CREATUREMOVEMENT_PROCESSINPUT
class CInputEvent;
class CProcessedInput;
#endif
class CInputProcessCreatureMovement : public CInputProcessGameBase {
public:
    CInputProcessCreatureMovement(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
#ifdef FABLE_RETAIL_INPUTPROCESS_CREATUREMOVEMENT_PROCESSINPUT
    // Retail opt-in (agent1022, row 0x00691D70): Ego_r.pdb ?ProcessInput@CInputProcessCreatureMovement@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
    // (vtable 0x01237F4C). Declaration only; the other virtuals stay unmodelled (the vtable is not emitted by the row's TU).
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);   // 0x00691D70
#endif
    CIntelligentPointer<const CThingPlayerCreature> PCreature;   // +0x28
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    // Opt-in (NF477, r467d / r459a): the retail virtual list, declaration only. Retail vftable 0x01237F4C, 9 counted function slots
    // (RTTI locator at vftable - 4, then consecutive slots at .text function starts); listed: each slot whose target differs from the
    // base class table, named by the base slot it fills (order from retail, spellings from Ego_r.pdb / FableWin.pdb).
public:
    virtual void Init();                                                                             // slot 2, 0x00691D20
#if !(defined(FABLE_RETAIL_INPUTPROCESS_CREATUREMOVEMENT_PROCESSINPUT))
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input);           // slot 4, 0x00691D70
#endif
public:
#endif
};
FABLE_STATIC_ASSERT_AT(27, sizeof(CInputProcessCreatureMovement) == 0x30);
#endif // FABLE_ENGINE_CINPUTPROCESSCREATUREMOVEMENT_H
