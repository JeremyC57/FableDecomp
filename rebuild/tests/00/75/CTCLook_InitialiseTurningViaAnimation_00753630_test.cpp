#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_DEF_CLASS_BASE
// Original named locals and actual GetResult on for_each temporary, no fitted snapshots.
// Leaf: ?InitialiseTurningViaAnimation@CTCLook@@AAEXABVC3DVector@@J@Z
// Flags: (none)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CANIMATIONENTRY_BLENDTIMES
#include "engine/CTCLook.h"
#include "engine/CGetBestTurningAnim.h"
#include "engine/CAnimationEntry.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimateThing.h"
#include "engine/GFCos.h"
#include "stlport/fable_algorithm.h"


#include "fable_assert.h"
#include <cstdio>
#include <cmath>
#include <cstring>
static int failures=0,normalises=0,constructs=0,iterations=0,animations=0;
#define CHECK(x) do{if(!(x)){++failures;std::printf("FAIL %d\n",__LINE__);}}while(0)
const float DELTA_NOTIONAL_ZERO=.0001f;
C3DVector&C3DVector::AccurateNormalise(){++normalises;double length=std::sqrt(X*X+Y*Y+Z*Z);if(length){X/=(float)length;Y/=(float)length;Z/=(float)length;}return*this;}
struct FixtureThing;
struct FixtureEntry{long BankIndex;long GetDelayTime()const{return 12;}long GetTimeToBlendIn(const FixtureThing&)const{return 13;}unsigned long GetAnimateThingFlags()const{return 14;}};
static FixtureEntry entry={17};static float requested_fraction=.5f;
struct FixtureFunctor{FixtureFunctor(const C3DVector&a,const C3DVector&b){++constructs;CHECK(a.Z==0&&b.Z==0);}void operator()(const std::pair<float,const FixtureEntry*>&){++iterations;}void GetResult(const FixtureEntry**p,float&f)const{*p=&entry;f=requested_fraction;}};
namespace std {template<class I,class F>F FABLE_FASTCALL for_each(I first,I last,F f){for(;first!=last;++first)f(*first);return f;}}
struct FixtureAnimInit{FixtureAnimInit(unsigned char type,unsigned long tf,long anim,EAnimUpdateMode mode,bool loop,unsigned long flags,long priority){CHECK(type==7&&tf==0&&anim==17&&(int)mode==1&&!loop&&flags==14&&priority==100);}};
struct FixtureAnimator{void SetMainAnim(const FixtureAnimInit&,long blend,long delay){++animations;CHECK(blend==13&&delay==12);}};
struct FixtureComplex{FixtureAnimator animator;FixtureAnimator&GetAnimation(){return animator;}};
struct FixturePhysics{C3DVector pos,facing;FixturePhysics():pos(0,0,0),facing(0,1,0){}C3DVector GetFacingVector()const{return facing;}const C3DVector&GetPosition()const{return pos;}};
struct FixtureThing{FixturePhysics p;FixtureComplex c;int queries;FixtureThing():queries(0){}FixturePhysics*GetPhysicsTC(){return&p;}bool GetTC(FixtureComplex**out){++queries;*out=&c;return true;}};
struct FixtureDef{bool AllowOversteer,AllowUndersteer;};
struct FixtureLook{FixtureThing parent;FixtureDef def;FixtureDef*PDef;float TurningViaAnimMinAngle,FractionOfTurningToAcceptEachFrame;bool UsingOldStyleTurning;const FixtureEntry*PCurrentTurningAnim;std::vector<std::pair<float,const FixtureEntry*> >TurningAnims;FixtureLook():PDef(&def),TurningViaAnimMinAngle(.1f),FractionOfTurningToAcceptEachFrame(99),UsingOldStyleTurning(false),PCurrentTurningAnim(0),TurningAnims(2){}FixtureThing&GetParentThing(){return parent;}void InitialiseTurningViaAnimation(const C3DVector&,long);};
#define CTCLook FixtureLook
#define CTCPhysicsBase FixturePhysics
#define CAnimationEntry FixtureEntry
#define CGetBestTurningAnim FixtureFunctor
#define CTCAnimationComplex FixtureComplex
#define CAnimateThing FixtureAnimator
#define CAnimInit FixtureAnimInit
void CTCLook::InitialiseTurningViaAnimation(const C3DVector& look_at,long look_flags)
{
 FABLE_RETAIL_ASSERT(GetParentThing().GetType() != THING_TYPE_PLAYER_CREATURE);
 CTCPhysicsBase* ptc_phys=GetParentThing().GetPhysicsTC();
 C3DVector facing_dir=ptc_phys->GetFacingVector();
 C3DVector look_vector=look_at-ptc_phys->GetPosition();
 facing_dir.Z=look_vector.Z=0;
 if(look_vector.IsNonZero()) {
  look_vector.AccurateNormalise();facing_dir.AccurateNormalise();
  float fraction_of_movement_to_accept_each_frame=1;
  bool old_style_movement=false;
  if(look_vector.Dot(facing_dir)>GFCos(TurningViaAnimMinAngle/2.0f))old_style_movement=true;
  else {
   const CAnimationEntry* panim_entry;
   std::for_each(TurningAnims.begin(),TurningAnims.end(),CGetBestTurningAnim(facing_dir,look_vector)).GetResult(&panim_entry,fraction_of_movement_to_accept_each_frame);
   if(fraction_of_movement_to_accept_each_frame>1){if(!PDef->AllowOversteer)fraction_of_movement_to_accept_each_frame=1;}
   else if(fraction_of_movement_to_accept_each_frame<1){if(!PDef->AllowUndersteer)fraction_of_movement_to_accept_each_frame=1;}
   CTCAnimationComplex* ptc_anim_complex;
   GetParentThing().GetTC(&ptc_anim_complex);
   CAnimateThing& anim=ptc_anim_complex->GetAnimation();
   anim.SetMainAnim(CAnimInit(7,0,panim_entry->BankIndex,(EAnimUpdateMode)1,false,panim_entry->GetAnimateThingFlags(),100),panim_entry->GetTimeToBlendIn(GetParentThing()),panim_entry->GetDelayTime());
   PCurrentTurningAnim=panim_entry;
  }
  FractionOfTurningToAcceptEachFrame=fraction_of_movement_to_accept_each_frame;
  UsingOldStyleTurning=old_style_movement;
 }
}


#undef CTCLook
#undef CTCPhysicsBase
#undef CAnimationEntry
#undef CGetBestTurningAnim
#undef CTCAnimationComplex
#undef CAnimateThing
#undef CAnimInit
int main(){float values[]={-.5f,.5f,1,1.5f,0};unsigned nan=0x7fc00000;std::memcpy(&values[4],&nan,4);
 for(int over=0;over<2;++over)for(int under=0;under<2;++under)for(int v=0;v<5;++v)for(int direction=0;direction<3;++direction){FixtureLook look;look.def.AllowOversteer=over!=0;look.def.AllowUndersteer=under!=0;requested_fraction=values[v];constructs=iterations=normalises=animations=0;const C3DVector pos=direction==0?C3DVector(0,0,20):(direction==1?C3DVector(0,5,0):C3DVector(5,0,0));look.InitialiseTurningViaAnimation(pos,0);
 if(direction==0){CHECK(look.FractionOfTurningToAcceptEachFrame==99&&!look.UsingOldStyleTurning&&!look.PCurrentTurningAnim&&normalises==0&&animations==0&&constructs==0);}else if(direction==1){CHECK(look.FractionOfTurningToAcceptEachFrame==1&&look.UsingOldStyleTurning&&!look.PCurrentTurningAnim&&normalises==2&&animations==0&&constructs==0);}else{float expected=values[v];if(expected>1&&!over)expected=1;if(expected<1&&!under)expected=1;CHECK((v==4?look.FractionOfTurningToAcceptEachFrame!=look.FractionOfTurningToAcceptEachFrame:look.FractionOfTurningToAcceptEachFrame==expected));CHECK(!look.UsingOldStyleTurning&&look.PCurrentTurningAnim==&entry&&normalises==2&&animations==1&&constructs==1&&iterations==2&&look.parent.queries==1);}}
 std::puts(failures?"00753630_TEST FAIL":"00753630_TEST PASS");return failures?1:0;}