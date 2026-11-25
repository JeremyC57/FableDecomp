// 00935160 NCondition00935160::Condition::Check (catalog CParticleMorphs_CanMorphBeQuickAssigned does not fit the body: an AI condition testing the hero's distance and CTCPerceiveThing awareness; owner class unidentified)
// Flags: /O2 /Oy /W3
// Leaf: Check
// Name fix (agentNF24): the namespaced CTCPerceiveThing stand-in replaced by the shared engine/CTCPerceiveThing.h (opt-in FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS_CALLS): 0x007446A0 / 0x00643B3B / 0x00744430 now bind the Ego_r/FableWin const spellings; 0x00744430 was GetDirectAwareness
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS_CALLS
#include "engine/CTCPerceiveThing.h"   // 0x007446A0 GetPerceivedThing, 0x00643B3B IsAwareOfThing, 0x00744430 IsAwarenessOfThingDirect (all const)
#include <math.h>
#pragma intrinsic(sqrt)
class CTCBase;
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in CInterfaceMap (?LowerBound@Map@NCondition00935160@@QAEPAUPair@2@ABH@Z / ...QBEPAUPair@2@ABH@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): the thing's Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAE / QBE spellings at 0x0040F020 / 0x004365B0.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
namespace NCondition00935160 {
struct C3DVector {float x,y,z;};struct CThing;
struct Component {char pad[56];int enabled;float distance;int frame;};
struct Pair {int key;void* value;};typedef ::CInterfaceMap Map;
// Both retail lower bounds read a signed key and stride eight bytes; neither changes its input.
struct CThing {char pad[36];unsigned mask24;char gap28[12];unsigned mask34;char gap38[12];Map map;const C3DVector& GetPosition()const;};
struct Context {char pad[24];CThing* thing;};struct CPlayer {CThing* GetControlledCreature();};struct CPlayerManager {CPlayer* GetMainPlayer();};
// Layout: condition context +4, activation frame+0x14; component170 enabled+0x38, range+0x3c.
struct Condition {void* table;Context* context;char pad[12];int frame;CPlayerManager* GetPPlayerManager();bool Check();};
bool Condition::Check(){int earliest=frame;if(CWorld::GetFrame()<earliest)return false;CThing* thing=context->thing;Component* component;if(thing->mask34&0x400){Map& map=thing->map;ETCInterfaceType key=(ETCInterfaceType)170;Map::value_type* p=map.LowerBound(key);if(p==map.end()||key<p->first)p=map.end();component=(Component*)p->second;}if(component->frame==-1&&earliest)return false;if(!component->enabled)return false;CThing* hero=GetPPlayerManager()->GetMainPlayer()->GetControlledCreature();if(!hero)return false;if(component->distance!=-1.0f){const C3DVector& a=context->thing->GetPosition();const C3DVector& b=hero->GetPosition();float x=b.x-a.x,y=b.y-a.y,z=b.z-a.z;if(sqrt(z*z+y*y+x*x)>component->distance)return false;}thing=context->thing;if(!(thing->mask24&0x100000))return false;ETCInterfaceType key=(ETCInterfaceType)52;const Map& map=thing->map;const Map::value_type* p=map.LowerBound(key);if(p==map.end()||key<p->first)p=map.end();::CTCPerceiveThing* target=(::CTCPerceiveThing*)p->second;bool found=(const void*)target->GetPerceivedThing()==(const void*)hero&&target->IsAwareOfThing()&&target->IsAwarenessOfThingDirect();if(found)return true;return false;}
}