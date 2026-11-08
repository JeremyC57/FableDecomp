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
class CInputProcessCreatureMovement : public CInputProcessGameBase {
public:
    CInputProcessCreatureMovement(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init) { }
    CIntelligentPointer<const CThingPlayerCreature> PCreature;   // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessCreatureMovement) == 0x30);
#endif // FABLE_ENGINE_CINPUTPROCESSCREATUREMOVEMENT_H
