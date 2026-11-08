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
    CIntelligentPointer<const CThingPlayerCreature> PCreature;   // +0x28
    CDefPointer<const CHeroCombatDef> PHeroCombatDef;              // +0x30
    long DeactivateHoldTimer;                                       // +0x34
    CIntelligentPointer<const CThing> PTarget;                      // +0x38
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessZTarget) == 0x40);
#endif // FABLE_ENGINE_CINPUTPROCESSZTARGET_H
