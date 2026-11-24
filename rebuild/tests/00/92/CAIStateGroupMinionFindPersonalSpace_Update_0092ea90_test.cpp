// Actual shared owner and original locals; unsupported creature/from/offset aliases removed.
// Leaf: ?Update@CAIStateGroup_MinionFindPersonalSpace@@UAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PERSONALSPACE_PASS24
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_KID_EVADE_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_MinionFindPersonalSpace.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureNavigation.h"

#include <cstdio>
#include <cmath>
#include <cstring>
static int failures=0;
#define CHECK(x) do {if(!(x)){++failures;std::printf("FAIL %d\n",__LINE__);}}while(0)
unsigned long GFastOneOverSqrtTable[128];
static void InitReciprocalFixture(){
 for(unsigned i=0;i<128;++i){float n=(i<64?2.0f:1.0f)*(1.0f+(i&63)/64.0f);float r=1.0f/(float)std::sqrt(n);unsigned bits;std::memcpy(&bits,&r,4);GFastOneOverSqrtTable[i]=(bits&0x00ffffffu)>>15;}
}
static bool Near(float a,float b,float eps=0.002f){return std::fabs(a-b)<eps;}

struct SpaceState{C3DVector self,invader,move;float radius;bool is_invader,find;int tests,positions,navigation_queries,searches,moves,terminates,radii;SpaceState():self(0,0,0),invader(0,1,0),radius(3),is_invader(true),find(true),tests(0),positions(0),navigation_queries(0),searches(0),moves(0),terminates(0),radii(0){}};
struct FixtureCreature{SpaceState*s;bool ours;FixtureCreature(SpaceState&state,bool b):s(&state),ours(b){}const C3DVector&PeekPos()const{++s->positions;return ours?s->self:s->invader;}float GetPersonalSpaceRadius(){++s->radii;return s->radius;}bool GetTC(struct FixtureNavigation**p);};
struct FixtureNavigation{SpaceState*s;bool GetNearestFleePosition(const C3DVector&p,float tolerance,C3DVector&out)const{++s->searches;CHECK(tolerance==.5f);s->move=p;out=p+C3DVector(1,2,0);return s->find;}};
static FixtureNavigation navigation;
bool FixtureCreature::GetTC(FixtureNavigation**p){++s->navigation_queries;navigation.s=s;*p=&navigation;return true;}
struct FixturePointer{FixtureCreature*p;FixtureCreature&operator*()const{return*p;}FixtureCreature*operator->()const{return p;}};
struct FixtureGroup{SpaceState&s;FixtureCreature&ours;FixturePointer PThingCreature;FixtureGroup(SpaceState&state,FixtureCreature&a,FixtureCreature&b):s(state),ours(a){PThingCreature.p=&b;}bool IsSpaceInvader(const FixtureCreature&){++s.tests;return s.is_invader;}FixtureCreature&GetCreature(){return ours;}void Terminate(){++s.terminates;}void Update();EAIActionReturn AIAction_MoveToPos(EAIActionRepeat r,C3DVector p,bool&route,EMovementType m,bool a,bool b,const CThing*t,bool c,float f){++s.moves;CHECK((int)r==1&&(int)m==3&&a&&b&&!t&&c&&f==0);CHECK(Near(p.X,s.move.X+1)&&Near(p.Y,s.move.Y+2)&&Near(p.Z,s.move.Z));route=true;return(EAIActionReturn)2;}};
#define CAIStateGroup_MinionFindPersonalSpace FixtureGroup
#define CTCCreatureNavigation FixtureNavigation
void CAIStateGroup_MinionFindPersonalSpace::Update()
{
 if(IsSpaceInvader(*PThingCreature)) {
  float space_radius=GetCreature().GetPersonalSpaceRadius();
  C3DVector away_dir=GetCreature().PeekPos()-PThingCreature->PeekPos();
  away_dir.Normalise();
  C3DVector move_to_pos=GetCreature().PeekPos()+away_dir*space_radius;
  C3DVector wander_pos;
  CTCCreatureNavigation* p_navigation;
  GetCreature().GetTC(&p_navigation);
  if(p_navigation->GetNearestFleePosition(move_to_pos,.5f,wander_pos)) {
   bool nav_route_calculated;
   AIAction_MoveToPos((EAIActionRepeat)1,wander_pos,nav_route_calculated,(EMovementType)3,true,true,0,true,0.0f);
  }
 }
 Terminate();
}

#undef CAIStateGroup_MinionFindPersonalSpace
#undef CTCCreatureNavigation
int main(){InitReciprocalFixture();const C3DVector dirs[]={C3DVector(0,1,0),C3DVector(1,0,0),C3DVector(0,0,1),C3DVector(1,2,3),C3DVector(0,0,0)};for(int gate=0;gate<2;++gate)for(int found=0;found<2;++found)for(int direction=0;direction<5;++direction){SpaceState s;s.is_invader=gate!=0;s.find=found!=0;s.self=C3DVector(3,4,5);s.invader=s.self+dirs[direction];FixtureCreature ours(s,true),other(s,false);FixtureGroup g(s,ours,other);g.Update();CHECK(s.tests==1&&s.terminates==1);CHECK(s.navigation_queries==gate&&s.searches==gate&&s.moves==(gate&&found?1:0));CHECK(s.positions==(gate?3:0)&&s.radii==gate);if(gate){double length=std::sqrt(dirs[direction].X*dirs[direction].X+dirs[direction].Y*dirs[direction].Y+dirs[direction].Z*dirs[direction].Z);if(length>0){CHECK(Near(s.move.X,3-(float)(3*dirs[direction].X/length)));CHECK(Near(s.move.Y,4-(float)(3*dirs[direction].Y/length)));CHECK(Near(s.move.Z,5-(float)(3*dirs[direction].Z/length)));}else CHECK(s.move.X==3&&s.move.Y==4&&s.move.Z==5);}}
 std::puts(failures?"0092ea90_TEST FAIL":"0092ea90_TEST PASS");return failures?1:0;}