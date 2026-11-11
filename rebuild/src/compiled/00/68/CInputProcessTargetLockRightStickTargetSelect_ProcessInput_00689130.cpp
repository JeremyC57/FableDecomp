// CInputProcessTargetLockRightStickTargetSelect::ProcessInput(const CInputEvent&,CProcessedInput&) -- edge-trigger right-stick target changes.
// Leaf: ProcessInput
#include "engine/CInputProcessTargetLockRightStickTargetSelect.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CAInputProcess.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CProcessedInput.h"
#include "engine/_quarantine/CThing.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTCBase.h"
// agentNF17: the namespace-local ETCInterfaceType / ComponentEntry / ComponentMap stand-ins are the global ETCInterfaceType and the
//   explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the landed 0x0074EE90 idiom) with its std::pair value_type;
//   0x0040F020 is its LowerBound (NF14 / established spelling). The inline Find keeps the row's lookup shape.
enum ETCInterfaceType { Targeting=8 };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
 typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
 value_type *_M_start,*_M_finish,*_M_end_of_storage;unsigned char Compare,Dirty;
 value_type* LowerBound(const ETCInterfaceType& key);   // 0x0040F020
 value_type* Find(ETCInterfaceType key){ETCInterfaceType lookup=key;value_type* i=LowerBound(lookup);if(i==_M_finish||key<i->first)i=_M_finish;return i;}
};
namespace NativeInput {
struct Vector2 {float X,Y;};
enum EInputEventType { RightStick=18 };
struct InputEvent {EInputEventType GetType()const;Vector2 GetJoystickPosition()const;};
struct ProcessedInput:CProcessedInput {void SetAsPerformedEvent();};
struct TargetingPlayer:CTCTargetingPlayer {enum ETargetNextThingMethod { Left=2,Right=3 };void SetAsTargetNextThingInDirection(ETargetNextThingMethod);};
typedef ::CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
// Retail CThing: component bitset36 vs donor40, vector map16 vs20, list8 vs12.
struct ThingPlayerCreature {unsigned char Prefix[0x20];unsigned int ComponentBits[9];ComponentMap Components;unsigned char RemainingState[0x3d];unsigned char LifeFlags;};
}
class CThingPlayerCreature : public NativeInput::ThingPlayerCreature { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
namespace NativeInput {
// Field/method projection of the actual flattened owner header (no engine construction claimed).
struct TargetSelect:CAInputProcess {
 CWorld* World;CPlayer* MyPlayer;CGameDefinitionManager* DefinitionManager;CMainGameComponent* Component;CGamePlayerInterface* PGamePlayerInterface;
 bool ChangedTargetRight,ChangedTargetLeft;
 void ProcessInput(const InputEvent&,ProcessedInput&);
};
void TargetSelect::ProcessInput(const InputEvent& event,ProcessedInput& output){
 ThingPlayerCreature* thing=MyPlayer->GetPControlledCreature();
 if(thing && !(thing->LifeFlags&1)){
  TargetingPlayer* target=0;
  if(thing->ComponentBits[0]&(1<<8))target=(TargetingPlayer*)thing->Components.Find(Targeting)->second;
  if(event.GetType()==RightStick){
   Vector2 pos=event.GetJoystickPosition();
   if(pos.X<.25f)ChangedTargetRight=false;
   if(pos.X>-.25f)ChangedTargetLeft=false;
   if(pos.X>.65f && !ChangedTargetRight){target->SetAsTargetNextThingInDirection(TargetingPlayer::Right);ChangedTargetRight=true;}
   else if(pos.X<-.65f && !ChangedTargetLeft){target->SetAsTargetNextThingInDirection(TargetingPlayer::Left);ChangedTargetLeft=true;}
   output.SetAsPerformedEvent();
  }
 }
}
}