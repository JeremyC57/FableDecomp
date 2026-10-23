// 00935160 NCondition00935160::Condition::Check (catalog CParticleMorphs_CanMorphBeQuickAssigned does not fit the body: an AI condition testing the hero's distance and CTCPerceiveThing awareness; owner class unidentified)
// Flags: /O2 /Oy /W3
// Leaf: Check
#include <math.h>
#pragma intrinsic(sqrt)
namespace NCondition00935160 {
struct C3DVector {float x,y,z;};struct CThing;
struct Component {char pad[56];int enabled;float distance;int frame;};
struct CTCPerceiveThing {CThing* GetPerceivedThing();bool IsAwareOfThing();bool GetDirectAwareness();};
struct Pair {int key;void* value;};struct Map {Pair* first;Pair* last;Pair* end;Pair* LowerBound(const int&);Pair* LowerBound(const int&)const;};
// Both retail lower bounds read a signed key and stride eight bytes; neither changes its input.
__declspec(noinline) Pair* Map::LowerBound(const int& key){Pair* p=first;int count=last-first;while(count>0){int half=count/2;Pair* mid=p+half;if(mid->key<key){p=mid+1;count-=half+1;}else count=half;}return p;}
__declspec(noinline) Pair* Map::LowerBound(const int& key)const{Pair* p=first;int count=last-first;while(count>0){int half=count/2;Pair* mid=p+half;if(mid->key<key){p=mid+1;count-=half+1;}else count=half;}return p;}
struct CThing {char pad[36];unsigned mask24;char gap28[12];unsigned mask34;char gap38[12];Map map;const C3DVector& GetPosition()const;};
struct Context {char pad[24];CThing* thing;};struct CPlayer {CThing* GetControlledCreature();};struct CPlayerManager {CPlayer* GetMainPlayer();};
int __fastcall GFGetCurrentFrame();
// Layout: condition context +4, activation frame+0x14; component170 enabled+0x38, range+0x3c.
struct Condition {void* table;Context* context;char pad[12];int frame;CPlayerManager* GetPPlayerManager();bool Check();};
bool Condition::Check(){int earliest=frame;if(GFGetCurrentFrame()<earliest)return false;CThing* thing=context->thing;Component* component;if(thing->mask34&0x400){Map& map=thing->map;int key=170;Pair* p=map.LowerBound(key);if(p==map.last||key<p->key)p=map.last;component=(Component*)p->value;}if(component->frame==-1&&earliest)return false;if(!component->enabled)return false;CThing* hero=GetPPlayerManager()->GetMainPlayer()->GetControlledCreature();if(!hero)return false;if(component->distance!=-1.0f){const C3DVector& a=context->thing->GetPosition();const C3DVector& b=hero->GetPosition();float x=b.x-a.x,y=b.y-a.y,z=b.z-a.z;if(sqrt(z*z+y*y+x*x)>component->distance)return false;}thing=context->thing;if(!(thing->mask24&0x100000))return false;int key=52;const Map& map=thing->map;Pair* p=map.LowerBound(key);if(p==map.last||key<p->key)p=map.last;CTCPerceiveThing* target=(CTCPerceiveThing*)p->value;bool found=target->GetPerceivedThing()==hero&&target->IsAwareOfThing()&&target->GetDirectAwareness();if(found)return true;return false;}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace NCondition00935160 {
static int now=1;static CThing hero;static CThing* character=&hero;static CThing* targetThing=&hero;static CPlayer player;static CPlayerManager players;static bool ready=true,allowed=true;static C3DVector origin={0,0,0},destination={3,4,0};
int __fastcall GFGetCurrentFrame(){return now;}CPlayerManager* Condition::GetPPlayerManager(){return &players;}CPlayer* CPlayerManager::GetMainPlayer(){return &player;}CThing* CPlayer::GetControlledCreature(){return character;}CThing* CTCPerceiveThing::GetPerceivedThing(){return targetThing;}bool CTCPerceiveThing::IsAwareOfThing(){return ready;}bool CTCPerceiveThing::GetDirectAwareness(){return allowed;}const C3DVector& CThing::GetPosition()const{return this==&hero?destination:origin;}
}
int main(){using namespace NCondition00935160;Condition c;memset(&c,0,sizeof(c));c.frame=2;assert(!c.Check());CThing t;memset(&t,0,sizeof(t));Context ctx;memset(&ctx,0,sizeof(ctx));ctx.thing=&t;c.context=&ctx;c.frame=0;Component comp;memset(&comp,0,sizeof(comp));comp.enabled=1;comp.distance=-1;CTCPerceiveThing target;Pair pairs[3];pairs[0].key=52;pairs[0].value=&target;pairs[1].key=170;pairs[1].value=&comp;pairs[2].key=999;pairs[2].value=0;t.map.first=pairs;t.map.last=pairs+2;t.map.end=pairs+3;t.mask34=0x400;t.mask24=0x100000;
assert(c.Check());int k=51;assert(t.map.LowerBound(k)==pairs&&k==51);k=53;assert(t.map.LowerBound(k)==pairs+1);k=171;assert(((const Map&)t.map).LowerBound(k)==pairs+2);
comp.enabled=0;assert(!c.Check());comp.enabled=1;comp.frame=-1;c.frame=1;assert(!c.Check());c.frame=0;comp.frame=0;
character=0;assert(!c.Check());character=&hero;comp.distance=4;assert(!c.Check());comp.distance=5;assert(c.Check());comp.distance=6;assert(c.Check());comp.distance=-1;t.mask24=0;assert(!c.Check());t.mask24=0x100000;targetThing=&t;assert(!c.Check());targetThing=&hero;ready=false;assert(!c.Check());ready=true;allowed=false;assert(!c.Check());allowed=true;assert(c.Check());Map empty;empty.first=empty.last=empty.end=pairs;k=1;assert(empty.LowerBound(k)==pairs);puts("00935160_TEST PASS");return 0;}