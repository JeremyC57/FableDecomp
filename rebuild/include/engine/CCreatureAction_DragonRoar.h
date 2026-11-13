#ifndef FABLE_ENGINE_CCREATUREACTION_DRAGONROAR_H
#define FABLE_ENGINE_CCREATUREACTION_DRAGONROAR_H
// New header (chunk 2009, row 0x00952890 CAIStateGroup_JackDragon::UpdateGround). RTTI .?AVCCreatureAction_DragonRoar@@ :
// CCombatActionBase (retail 0x114 bytes, no own members: Ego_r size 300 = the Ego_r CCombatActionBase). Constructor Ego_r.pdb /
// FableWin.pdb ??0CCreatureAction_DragonRoar@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z, out of line in retail at 0x00953300; the
// destructor is implicit (UpdateGround destroys the temporary with ~CCombatActionBase 0x0062E900 directly).
#include "engine/CCombatActionBase.h"
class CThingCreatureBase; class CThing;
class CCreatureAction_DragonRoar : public CCombatActionBase {
public:
    CCreatureAction_DragonRoar(CThingCreatureBase& creature, CThing& target);   // 0x00953300
};
#endif
