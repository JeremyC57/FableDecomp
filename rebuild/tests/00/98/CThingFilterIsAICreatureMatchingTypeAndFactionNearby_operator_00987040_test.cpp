// Verbatim complete method, unchanged conditions/math; owner/engine query contracts use deterministic fixture adapters.
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
// Original source locals to_vec and block-local ptc_enemy; original helpers replace fitted scalar snapshots.
// Leaf: ??RCThingFilter_IsAICreatureMatchingTypeAndFactionNearby@@QBE_NABVCThingAICreature@@@Z
// Flags: (none)
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCEnemy.h"
#include "engine/CThingFilter_IsAICreatureMatchingTypeAndFactionNearby.h"


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

struct FixtureEnemy{bool allied;mutable int calls;void* expected;FixtureEnemy():allied(true),calls(0),expected(0){}bool IsAlliedWithFaction(void* f)const{++calls;CHECK(f==expected);return allied;}};
struct FixtureCreature{int type;C3DVector pos;bool present;FixtureEnemy* enemy;mutable int pos_calls,peek_calls;FixtureCreature(FixtureEnemy* e):type(1),pos(1,0,0),present(true),enemy(e),pos_calls(0),peek_calls(0){}int GetCreatureType()const{return type;}const C3DVector& GetPos()const{++pos_calls;return pos;}bool PeekTC(ETCInterfaceType,const FixtureEnemy** p)const{++peek_calls;if(present)*p=enemy;return present;}};
struct FixtureFilter{int OurCreatureType;C3DVector OurPos,SideVec;float MaxSqDist,MinCosine;bool RestrictAngle;void* PFaction;FixtureFilter():OurCreatureType(1),OurPos(0,0,0),SideVec(1,0,0),MaxSqDist(4),MinCosine(.9f),RestrictAngle(false),PFaction(0){}bool operator()(const FixtureCreature&)const;};
#define CThingFilter_IsAICreatureMatchingTypeAndFactionNearby FixtureFilter
#define CThingAICreature FixtureCreature
#define CTCEnemy FixtureEnemy
bool CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::operator()(const CThingAICreature& creature) const
{
    if (creature.GetCreatureType()==OurCreatureType) {
        C3DVector to_vec=creature.GetPos()-OurPos;
        if (to_vec.GetSquaredMagnitude()<MaxSqDist) {
            const CTCEnemy* ptc_enemy;
            if (creature.PeekTC(TCI_ENEMY,&ptc_enemy) && ptc_enemy->IsAlliedWithFaction(PFaction))
                return !RestrictAngle?true:to_vec.Normalise().Dot(SideVec)>MinCosine;
        }
    }
    return false;
}

#undef CThingFilter_IsAICreatureMatchingTypeAndFactionNearby
#undef CThingAICreature
#undef CTCEnemy
int main(){InitReciprocalFixture();
 for(int type=0;type<2;++type)for(int present=0;present<2;++present)for(int ally=0;ally<2;++ally)for(int restrict_angle=0;restrict_angle<2;++restrict_angle)for(int direction=0;direction<5;++direction){FixtureEnemy enemy;enemy.allied=ally!=0;FixtureCreature creature(&enemy);FixtureFilter filter;creature.type=type;creature.present=present!=0;filter.RestrictAngle=restrict_angle!=0;
 if(direction==1)creature.pos=C3DVector(-1,0,0);if(direction==2)creature.pos=C3DVector(0,1,0);if(direction==3)creature.pos=C3DVector(2,0,0);if(direction==4)creature.pos=C3DVector(0,0,0);
 bool expected=type&&present&&ally&&direction!=3&&(!restrict_angle||direction==0);CHECK(filter(creature)==expected);CHECK(creature.pos_calls==(type?1:0));CHECK(creature.peek_calls==(type&&direction!=3?1:0));CHECK(enemy.calls==(type&&direction!=3&&present?1:0));}
 {FixtureEnemy e;FixtureCreature c(&e);FixtureFilter f;f.RestrictAngle=true;c.pos=C3DVector(0,1,0);f.MinCosine=0;CHECK(!f(c));f.MinCosine=-.01f;CHECK(f(c));}
 {FixtureEnemy e;FixtureCreature c(&e);FixtureFilter f;unsigned bits=0x7fc00000;std::memcpy(&c.pos.X,&bits,4);CHECK(!f(c)&&c.peek_calls==0);}
 std::puts(failures?"00987040_TEST FAIL":"00987040_TEST PASS");return failures?1:0;}