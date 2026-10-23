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