// CAIStateGroup_MinionReactToApology::Update() -- speak accepted or rejected apology response then terminate
// Retail0x0091D190,270 bytes. GetApology91D0A0; player manager8FCF90,main player449970,CPlayer::GetPControlledCreature 487DC0;
// RTTI pointers1237f1c/123e7d8 slot0 deleting dtors48a4f0/4d6915, genuine virtual base destructor.
// intelligent pointer ctor/set A01B10/A01B90; speech981EA0; terminate8FCF40.
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase

class CAIStateGroupBase : public CAIStateGroupBaseLayout
{
public:
    void Terminate();   // 0x008FCF40
};
#include "engine/CAIBrain.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
class CCharString;
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*); };
class CThing { public:
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead;
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        bool present=(bits[type>>5]>>(type&31))&1;
        if(present) {
            long key=type; CInterfaceMap* map=&interfaces; CInterfaceMap::Entry* it=map->LowerBound(&key);
            *out=(T*)((it==map->last || it->type>type)?map->last:it)->component;
        } return present;
    }
};


struct CEventApologyReceived { void* vtable; long WorldFrame; bool Accepted; };
struct CEventFilter_None { CEventFilter_None() {} };
struct CTCEntityEventsMethods : CTCEntityEvents { bool GetApology(const CEventApologyReceived**,const CEventFilter_None&);
 const CEventApologyReceived* GetApology() { const CEventApologyReceived* event=0; GetApology(&event,CEventFilter_None()); return event; } };
class CThingPlayerCreature : public CThing { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
struct CPlayerManagerMethods : CPlayerManager { CPlayer* GetMainPlayer(); };
struct CBaseIntelligentPointer { void* Handle; CBaseIntelligentPointer(); virtual ~CBaseIntelligentPointer(); void SetPItem(CThing*); CThing* GetPItem() const; };
struct CIntelligentConstThing : CBaseIntelligentPointer { CIntelligentConstThing(CThing* p) { SetPItem(p); } CIntelligentConstThing(const CIntelligentConstThing& p) { SetPItem(p.GetPItem()); } };
struct CIntelligentCreature : CBaseIntelligentPointer { CIntelligentCreature(CThing* p) { SetPItem(p); } CIntelligentCreature(const CIntelligentCreature& p) { SetPItem(p.GetPItem()); } };
extern CCharString g_ApologyAccepted; // 0x13BB684
extern CCharString g_ApologyRejected; // 0x13BB688
struct CAIStateGroup_MinionReactToApology : CAIStateGroupBase {
 CPlayerManagerMethods* GetPPlayerManager(); 
 long AIAction_SpeakAISpeechToTarget(long,CIntelligentCreature,CIntelligentConstThing,const CCharString&,bool,long);
 void Update();
};
void CAIStateGroup_MinionReactToApology::Update() {
 CTCEntityEventsMethods* events;
 bool hasEvents=((CThing*)PBrain->Creature)->GetInterface(&events,0x42);
 if(hasEvents) {
  const CEventApologyReceived* apology=events->GetApology();
  if(apology) {
   CThing* hero=GetPPlayerManager()->GetMainPlayer()->GetPControlledCreature();
   if(apology->Accepted) AIAction_SpeakAISpeechToTarget(1,(CThing*)PBrain->Creature,hero,g_ApologyAccepted,true,0x93);
   else AIAction_SpeakAISpeechToTarget(1,(CThing*)PBrain->Creature,hero,g_ApologyRejected,true,0x93);
  }
 }
 Terminate();
}