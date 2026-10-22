struct CAIStateGroup_StandStill; int __fastcall ss_sub(CAIStateGroup_StandStill* stateGroup);
struct CAIStateGroup_StandStill { int x; void Update(); };
void CAIStateGroup_StandStill::Update(){ int r; do { r = ss_sub(this); } while(r==4); }