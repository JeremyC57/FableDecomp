// 00935160 NCondition00935160::Condition::Check (catalog CParticleMorphs_CanMorphBeQuickAssigned does not fit the body: an AI condition testing the hero's distance and CTCPerceiveThing awareness; owner class unidentified)
// Flags: /O2 /Oy /W3
// Leaf: Check
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
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
// Layout: condition context +4, activation frame+0x14; component170 enabled+0x38, range+0x3c.
struct Condition {void* table;Context* context;char pad[12];int frame;CPlayerManager* GetPPlayerManager();bool Check();};
bool Condition::Check(){int earliest=frame;if(CWorld::GetFrame()<earliest)return false;CThing* thing=context->thing;Component* component;if(thing->mask34&0x400){Map& map=thing->map;int key=170;Pair* p=map.LowerBound(key);if(p==map.last||key<p->key)p=map.last;component=(Component*)p->value;}if(component->frame==-1&&earliest)return false;if(!component->enabled)return false;CThing* hero=GetPPlayerManager()->GetMainPlayer()->GetControlledCreature();if(!hero)return false;if(component->distance!=-1.0f){const C3DVector& a=context->thing->GetPosition();const C3DVector& b=hero->GetPosition();float x=b.x-a.x,y=b.y-a.y,z=b.z-a.z;if(sqrt(z*z+y*y+x*x)>component->distance)return false;}thing=context->thing;if(!(thing->mask24&0x100000))return false;int key=52;const Map& map=thing->map;Pair* p=map.LowerBound(key);if(p==map.last||key<p->key)p=map.last;CTCPerceiveThing* target=(CTCPerceiveThing*)p->value;bool found=target->GetPerceivedThing()==hero&&target->IsAwareOfThing()&&target->GetDirectAwareness();if(found)return true;return false;}
}