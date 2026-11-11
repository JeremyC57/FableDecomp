#ifndef FABLE_ENGINE_CCREATUREACTION_SHEATHEWEAPONS_H
#define FABLE_ENGINE_CCREATUREACTION_SHEATHEWEAPONS_H
// CCreatureAction_SheatheWeapons (new shared header, chunk 2026, row 0x0092DA20 CAIStateGroup_MinionSleep::Update). RTTI
// .?AVCCreatureAction_SheatheWeapons@@ : CCombatActionBase (vtable 0x01282CEC). Ego_r.pdb / FableWin.pdb
// ??0CCreatureAction_SheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z (out of line in retail at 0x008CC370, landed; already bound by that
// spelling) and member PlayedSound (Ego_r +0x12C = retail +0x114 after the CCombatActionBase container shrink; the landed ctor stores
// it there). No user destructor (both PDBs list none): a temporary's destruction calls ~CCombatActionBase 0x0062E900 (0x0092DDE0).
// Needs the retail CCombatActionBase (engine/CCombatActionBase.h, FABLE_RETAIL_CCREATUREACTIONBASE).
#include "rebuild_abi.h"
#include "engine/CCombatActionBase.h"
class CThingCreatureBase;
class CCreatureAction_SheatheWeapons : public CCombatActionBase {
public:
    CCreatureAction_SheatheWeapons(CThingCreatureBase& creature);        // 0x008CC370
private:
    bool PlayedSound;                                                    // +0x114
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_SheatheWeapons) == 0x118);
#endif // FABLE_ENGINE_CCREATUREACTION_SHEATHEWEAPONS_H
