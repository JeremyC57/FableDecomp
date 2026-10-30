#include "engine/CAIStateGroup_ChickenBeg.h"

struct ChickenBegStageA; class CThingPlayerCreature;
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };   // 0x00487DD0
ChickenBegStageA* __fastcall f1(void* stateGroup);
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
void __fastcall f4(void* stateGroup);

void __fastcall CAIStateGroup_ChickenBeg_Update(CAIStateGroup_ChickenBeg* stateGroup) {
    (&((CPlayerManager*)(f1(stateGroup)))->GetMainPlayer())->PeekPControlledCreature();
    f4(stateGroup);
}
