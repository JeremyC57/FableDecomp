// 00935160 NCondition00935160::Condition::Check
// Flags: /O2 /Oy /W3
// Leaf: Check
#include <math.h>
#pragma intrinsic(sqrt)
namespace NCondition00935160 {
struct V {float x,y,z;};struct Thing;
struct Component {char pad[56];int enabled;float distance;int frame;};
struct Target {Thing* Get();bool Ready();bool Allowed();};
struct Pair {int key;void* value;};struct Map {Pair* first;Pair* last;Pair* end;Pair* Lower(const int&);Pair* Lower(const int&)const;};
// Both retail lower bounds read a signed key and stride eight bytes; neither changes its input.
__declspec(noinline) Pair* Map::Lower(const int& key){Pair* p=first;int count=last-first;while(count>0){int half=count/2;Pair* mid=p+half;if(mid->key<key){p=mid+1;count-=half+1;}else count=half;}return p;}
__declspec(noinline) Pair* Map::Lower(const int& key)const{Pair* p=first;int count=last-first;while(count>0){int half=count/2;Pair* mid=p+half;if(mid->key<key){p=mid+1;count-=half+1;}else count=half;}return p;}
struct Thing {char pad[36];unsigned mask24;char gap28[12];unsigned mask34;char gap38[12];Map map;const V& Position()const;};
struct Context {char pad[24];Thing* thing;};struct Player {Thing* Character();};struct Players {Player* Main();};
int __fastcall Frame();
// Layout: condition context +4, activation frame+0x14; component170 enabled+0x38, range+0x3c.
struct Condition {void* table;Context* context;char pad[12];int frame;Players* PlayersManager();bool Check();};
bool Condition::Check(){int earliest=frame;if(Frame()<earliest)return false;Thing* thing=context->thing;Component* component;if(thing->mask34&0x400){Map& map=thing->map;int key=170;Pair* p=map.Lower(key);if(p==map.last||key<p->key)p=map.last;component=(Component*)p->value;}if(component->frame==-1&&earliest)return false;if(!component->enabled)return false;Thing* hero=PlayersManager()->Main()->Character();if(!hero)return false;if(component->distance!=-1.0f){const V& a=context->thing->Position();const V& b=hero->Position();float x=b.x-a.x,y=b.y-a.y,z=b.z-a.z;if(sqrt(z*z+y*y+x*x)>component->distance)return false;}thing=context->thing;if(!(thing->mask24&0x100000))return false;int key=52;const Map& map=thing->map;Pair* p=map.Lower(key);if(p==map.last||key<p->key)p=map.last;Target* target=(Target*)p->value;bool found=target->Get()==hero&&target->Ready()&&target->Allowed();if(found)return true;return false;}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace NCondition00935160 {
static int now=1;static Thing hero;static Thing* character=&hero;static Thing* targetThing=&hero;static Player player;static Players players;static bool ready=true,allowed=true;static V origin={0,0,0},destination={3,4,0};
int __fastcall Frame(){return now;}Players* Condition::PlayersManager(){return &players;}Player* Players::Main(){return &player;}Thing* Player::Character(){return character;}Thing* Target::Get(){return targetThing;}bool Target::Ready(){return ready;}bool Target::Allowed(){return allowed;}const V& Thing::Position()const{return this==&hero?destination:origin;}
}
int main(){using namespace NCondition00935160;Condition c;memset(&c,0,sizeof(c));c.frame=2;assert(!c.Check());Thing t;memset(&t,0,sizeof(t));Context ctx;memset(&ctx,0,sizeof(ctx));ctx.thing=&t;c.context=&ctx;c.frame=0;Component comp;memset(&comp,0,sizeof(comp));comp.enabled=1;comp.distance=-1;Target target;Pair pairs[3];pairs[0].key=52;pairs[0].value=&target;pairs[1].key=170;pairs[1].value=&comp;pairs[2].key=999;pairs[2].value=0;t.map.first=pairs;t.map.last=pairs+2;t.map.end=pairs+3;t.mask34=0x400;t.mask24=0x100000;
assert(c.Check());int k=51;assert(t.map.Lower(k)==pairs&&k==51);k=53;assert(t.map.Lower(k)==pairs+1);k=171;assert(((const Map&)t.map).Lower(k)==pairs+2);
comp.enabled=0;assert(!c.Check());comp.enabled=1;comp.frame=-1;c.frame=1;assert(!c.Check());c.frame=0;comp.frame=0;
character=0;assert(!c.Check());character=&hero;comp.distance=4;assert(!c.Check());comp.distance=5;assert(c.Check());comp.distance=6;assert(c.Check());comp.distance=-1;t.mask24=0;assert(!c.Check());t.mask24=0x100000;targetThing=&t;assert(!c.Check());targetThing=&hero;ready=false;assert(!c.Check());ready=true;allowed=false;assert(!c.Check());allowed=true;assert(c.Check());Map empty;empty.first=empty.last=empty.end=pairs;k=1;assert(empty.Lower(k)==pairs);puts("00935160_TEST PASS");return 0;}