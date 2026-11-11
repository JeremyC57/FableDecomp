#ifndef FABLE_ENGINE_CTCSUMMONABLECREATURE_H
#define FABLE_ENGINE_CTCSUMMONABLECREATURE_H
// NEW (agentNF11, row 0x008D08A0 CGameScriptThing::IsSummonedCreature): the summonable-creature component. RTTI
// .?AVCTCSummonableCreature@@ : CTCBase (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CTCSummonableCreature (struct_layouts_egor.tsv,
// 0x1C): PDef +0x0C, HasBeenSummoned +0x10 (retail `mov cl,[eax+10h]`, 0x008D0901), Summoner +0x11 (byte-packed CIntelligentPointer,
// opaque here). Methods: FableWin.pdb ?StatGetInterfaceType@CTCSummonableCreature@@SI?AW4ETCInterfaceType@@XZ (inline: TCI_SUMMONABLE_CREATURE
// 0xA0 folds into the caller's PeekTC lookup, 0x008D08E0) and Ego_r.pdb / FableWin.pdb ?GetHasBeenSummoned@CTCSummonableCreature@@QBE_NXZ
// (inline in retail: the caller reads +0x10 in place). Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/ETCInterfaceType.h"
class CSummonableCreatureDef;
class CTCSummonableCreature : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SUMMONABLE_CREATURE; }
    bool GetHasBeenSummoned() const { return HasBeenSummoned; }
private:
    CDefPointer<const CSummonableCreatureDef> PDef;                      // +0x0c
    bool                                      HasBeenSummoned;           // +0x10
    unsigned char                             Summoner[8];               // +0x11 CIntelligentPointer<CThingCreatureBase> (byte-packed)
    unsigned char                             _pad_0x19[3];
#ifdef FABLE_RETAIL_CTCSUMMONABLECREATURE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0079C4A0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCSummonableCreature) == 0x1c);
#endif
