// Original locals, actual shared owner and original operand/evaluation order.
// Leaf: ?GetRushToPos@CAIStateGroup_JackOfBladesStage1@@IAE_NAAVC3DVector@@@Z
// Flags: (none)
#define FABLE_RETAIL_JACK_STAGE1_GETRUSHTOPOS_PASS26
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#pragma intrinsic(atan2,fmod,sqrt)
bool CAIStateGroup_JackOfBladesStage1::GetRushToPos(C3DVector& rush_to_pos)
{
 C3DVector centre_pos;
 if(IS_THING_ALIVE(PArenaCentre)) centre_pos=PArenaCentre->GetPos();
 else centre_pos=GetCreature().GetPos();
 if(IS_THING_ALIVE(PTargetThing)) {
  long count=0;
  C3DVector vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
  if(vector_to_target.GetMagnitude2D()>8.0f*2.0f) {
   float angle_to_target=vector_to_target.GetAngleXY();
   C3DVector zig_vector(.5f,1.0f,0.0f),zag_vector(-.5f,1.0f,0.0f);
   C3DVector move_vector;
   if(ZigZagCounter%2)move_vector=zig_vector;else move_vector=zag_vector;
   move_vector.RotateXY(angle_to_target);
   move_vector*=8.0f;
   rush_to_pos=GetCreature().GetPos()+move_vector;
   return true;
  }
  return false;
 }
 return false;
}

#include <cstdio>
#include <cmath>
#include <cstring>
static int failed=0,position_calls=0,rotation_calls=0;
#define CHECK(x) do{if(!(x)){++failed;std::printf("FAIL %d\n",__LINE__);}}while(0)
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}
CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{}
void CClass::Save(CCharString&){}
CCharString::CCharString(){}
CCharString::CCharString(const CCharString&){}
CCharString::~CCharString(){}
void CThingGameObject::OnHit(CHitParameters&){}
void CThingCreatureBase::OnHit(CHitParameters&){}
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&){return false;}
struct PointerSlot{const CBaseIntelligentPointer* key;CBaseObject* item;};
static PointerSlot slots[512];static int slot_count=0;
CBaseIntelligentPointer::CBaseIntelligentPointer(){slots[slot_count].key=this;slots[slot_count++].item=0;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{for(int i=0;i<slot_count;++i)if(slots[i].key==this)return slots[i].item;CHECK(false);return 0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){for(int i=0;i<slot_count;++i)if(slots[i].key==this){slots[i].item=p;return;}CHECK(false);}
struct PositionSlot{const CThing* key;C3DVector value;};static PositionSlot positions[3];
const C3DVector& CThing::GetPos()const{++position_calls;for(int i=0;i<3;++i)if(positions[i].key==this)return positions[i].value;CHECK(false);return positions[0].value;}
void C3DVector::RotateXY(float angle){++rotation_calls;const double a=angle*6.283185307179586;const float x=X,y=Y;X=(float)(x*std::cos(a)-y*std::sin(a));Y=(float)(x*std::sin(a)+y*std::cos(a));}
CAIStateGroupBase::CAIStateGroupBase(){}
void CAIStateGroupBase::Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&){}
void CAIStateGroupBase::OnActivate(){}
bool CAIStateGroupBase::InitCreaturePosition(){return false;}
bool CAIStateGroupBase::AllowPriorityOverlap(){return false;}
bool CAIStateGroupBase::IsStillValid()const{return true;}
const CCharString* CAIStateGroupBase::GetDebugText()const{return 0;}
bool CAIStateGroupBase::IsTimingCritical()const{return false;}
EAIStateGroupType CAIStateGroupBase::GetStateGroupType()const{return(EAIStateGroupType)0;}
void CAIStateGroupBase::Cleanup(){}
void CAIStateGroupBase::OnReplace(){}
bool CAIStateGroupBase::IsRunnable(){return true;}
void CAIStateGroupBase::InitStateGroup(const CWorld&,CThingAICreature&){}
class TestCreature:public CThingAICreature{public:void SetDead(bool b){Dead=b;}};
class TestJack:public CAIStateGroup_JackOfBladesStage1{public:
 using CAIStateGroup_JackOfBladesStage1::GetRushToPos;
 void Update(){}
 void Setup(CAIBrain& brain,CThing* centre,CThingCreatureBase* target,long counter){PBrain=&brain;PArenaCentre.Set(centre);PTargetThing.Set(target);ZigZagCounter=counter;}
};
static bool Near(float x,float y){return std::fabs(x-y)<.0001f;}
int main(){TestCreature self,centre,target;CAIBrain brain;brain.Creature=&self;positions[0].key=&self;positions[1].key=&centre;positions[2].key=&target;positions[0].value=C3DVector(3,4,2);positions[1].value=C3DVector(100,100,20);
 TestJack jack;const C3DVector directions[]={C3DVector(0,20,0),C3DVector(20,0,0),C3DVector(-20,0,0),C3DVector(0,-20,0),C3DVector(0,16,50),C3DVector(0,15.99f,0),C3DVector(0,0,20)};
 for(int center_mode=0;center_mode<3;++center_mode)for(int target_mode=0;target_mode<3;++target_mode)for(int d=0;d<7;++d)for(int counter=-2;counter<3;++counter){self.SetDead(false);centre.SetDead(center_mode==2);target.SetDead(target_mode==2);positions[2].value=positions[0].value+directions[d];jack.Setup(brain,center_mode?&centre:0,target_mode?&target:0,counter);C3DVector output(-90,-91,-92);position_calls=rotation_calls=0;bool yes=jack.GetRushToPos(output);bool expected=target_mode==1&&d<4;CHECK(yes==expected);CHECK(rotation_calls==(expected?1:0));
 if(!expected){CHECK(output.X==-90&&output.Y==-91&&output.Z==-92);}else{double angle=std::atan2(directions[d].X,directions[d].Y);double x=counter%2?.5:-.5,y=1;CHECK(Near(output.X,3+(float)(8*(x*std::cos(angle)-y*std::sin(angle)))));CHECK(Near(output.Y,4+(float)(8*(x*std::sin(angle)+y*std::cos(angle)))));CHECK(output.Z==2);}
 CHECK(position_calls==1+(target_mode==1?2:0)+(expected?1:0));}
 std::puts(failed?"00971cd0_TEST FAIL":"00971cd0_TEST PASS");return failed?1:0;}