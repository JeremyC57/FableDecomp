#include "engine/CAIStateGroup_ChickenBeg.h"

struct ChickenBegStageA; class CThingPlayerCreature;
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };   // 0x00487DD0
ChickenBegStageA* __fastcall f1(void* stateGroup);
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
void __fastcall f4(void* stateGroup);

void __fastcall CAIStateGroup_ChickenBeg_Update(CAIStateGroup_ChickenBeg* stateGroup) {
    (&((CPlayerManager*)(f1(stateGroup)))->GetMainPlayer())->PeekPControlledCreature();
    f4(stateGroup);
}
