// CGameScriptInterface::GetPerceivingHeroEntityList -- collect live entities with active hero perception
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail008A8440; const lower_bound004365B0, temporary ScriptThing destructor004AA840.
// NF48 (round 7) attempt: the local projection model of CScriptThing (and its CountedScriptImp / CBaseClassNative / CCPPointerInfo
// pieces) is replaced by engine/CScriptThing.h's retail CScriptThing (FABLE_RETAIL_NSCRIPT), the local vector stand-in by STLport 4.6.2
// std::vector<CScriptThing>: erase(begin(), end()) is the out-of-line 0x008AC9D0 (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE), push_back
// the out-of-line 0x008ADF90 (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE); the definition is the CGameScriptInterface virtual (slot 233).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "stlport/fable_containers.h"
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish,*Capacity;const ComponentEntry* LowerBound(const ETCInterfaceType& key) const;};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //004365B0 (const lower_bound specialization)
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Rest[0x41];unsigned char DeletedFlags;CScriptThing GetScriptThing();}; //004C7E50 //004C73D0,004C7E50


#include <stdlib.h>
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CTCPerceiveThing.h"
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct ThingVector{CThingNative **Start,**Finish,**Capacity;ThingVector():Start(0),Finish(0),Capacity(0){}~ThingVector(){if(Start)free(Start);}};
struct HeroPerceptionManager{void GetPerceivingThings(ThingVector&)const;}; //00437910 ->0064D3A5 via manager+1B8
extern HeroPerceptionManager* g_HeroPerceptionManager; //013B8790
// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004C7E50 ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ (was ?GetScriptThing@CThingNative@@QAE?AVCScriptThing@@XZ)
class CThing { public: CScriptThing GetScriptThing() const; };
bool CGameScriptInterface::GetPerceivingHeroEntityList(std::vector<CScriptThing>& output)const{
 bool found=false;output.erase(output.begin(),output.end());CThingPlayerCreature* hero=PlayerManager->GetMainPlayer().GetPControlledCreature();if(hero&&!(hero->DeletedFlags&1)){
 ThingVector things;g_HeroPerceptionManager->GetPerceivingThings(things);for(CThingNative** it=things.Start;it!=things.Finish;++it){CThingNative* thing=*it;if(thing&&!(thing->DeletedFlags&1)&&(thing->UsingComponents[1]&0x100000)){
 const ComponentMap* map=&thing->Components;const ComponentEntry* entry=map->LowerBound((ETCInterfaceType)52);const ComponentEntry* end=map->Finish;const CTCPerceiveThing* perceive=static_cast<CTCPerceiveThing*>(((void*)(entry==end||entry->first>52?end:entry)->second));if(perceive->LineOfSight||perceive->AwarenessTimer!=0.0f||perceive->ExtendedLineOfSight){output.push_back(reinterpret_cast<const CThing*>(thing)->GetScriptThing());found=true;}
 }}
 }return found;
}