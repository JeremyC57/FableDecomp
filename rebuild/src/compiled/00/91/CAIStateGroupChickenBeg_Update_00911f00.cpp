#include "engine/CAIStateGroup_ChickenBeg.h"

struct ChickenBegStageA; class CThingPlayerCreature;
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };   // 0x00487DD0
ChickenBegStageA* __fastcall f1(void* stateGroup);
CPlayer* __fastcall f2(ChickenBegStageA* stage);
void __fastcall f4(void* stateGroup);

void __fastcall CAIStateGroup_ChickenBeg_Update(CAIStateGroup_ChickenBeg* stateGroup) {
    f2(f1(stateGroup))->PeekPControlledCreature();
    f4(stateGroup);
}
