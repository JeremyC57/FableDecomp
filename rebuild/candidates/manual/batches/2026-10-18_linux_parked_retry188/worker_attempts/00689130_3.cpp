// CInputProcessTargetLockRightStickTargetSelect::ProcessInput(const CInputEvent&,CProcessedInput&) -- edge-trigger right-stick target changes.
// Leaf: ProcessInput
#include "engine/CInputProcessTargetLockRightStickTargetSelect.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CAInputProcess.h"
#include "engine/CPlayer.h"
#include "engine/CProcessedInput.h"
#include "engine/_quarantine/CThing.h"
#include "engine/CTCTargetingPlayer.h"
struct CTCBase;
namespace NativeInput {
struct Vector2 {float X,Y;};
enum EInputEventType { RightStick=18 };
struct InputEvent {EInputEventType GetType()const;Vector2 GetJoystickPosition()const;};
struct ProcessedInput:CProcessedInput {void SetAsPerformedEvent();};
struct TargetingPlayer:CTCTargetingPlayer {enum ETargetNextThingMethod { Left=2,Right=3 };void SetAsTargetNextThingInDirection(ETargetNextThingMethod);};
enum ETCInterfaceType { Targeting=8 };
struct ComponentEntry {ETCInterfaceType Key;CTCBase* Value;};
struct ComponentMap {
 ComponentEntry *First,*Last,*Capacity;unsigned char Compare,Dirty;
 ComponentEntry* LowerBound(const ETCInterfaceType&);
 ComponentEntry* Find(ETCInterfaceType key){ETCInterfaceType lookup=key;ComponentEntry* i=LowerBound(lookup);if(i==Last||key<i->Key)i=Last;return i;}
};
// Retail CThing: component bitset36 vs donor40, vector map16 vs20, list8 vs12.
struct ThingPlayerCreature {unsigned char Prefix[0x20];unsigned int ComponentBits[9];ComponentMap Components;unsigned char RemainingState[0x3d];unsigned char LifeFlags;};
struct Player:CPlayer {ThingPlayerCreature* GetPControlledCreature()const;};
// Field/method projection of the actual flattened owner header (no engine construction claimed).
struct TargetSelect:CAInputProcess {
 CWorld* World;Player* MyPlayer;CGameDefinitionManager* DefinitionManager;CMainGameComponent* Component;CGamePlayerInterface* PGamePlayerInterface;
 bool ChangedTargetRight,ChangedTargetLeft;
 void ProcessInput(const InputEvent&,ProcessedInput&);
};
void TargetSelect::ProcessInput(const InputEvent& event,ProcessedInput& output){
 ThingPlayerCreature* thing=MyPlayer->GetPControlledCreature();
 if(thing && !(thing->LifeFlags&1)){
  TargetingPlayer* target=0;
  if(thing->ComponentBits[0]&(1<<8))target=(TargetingPlayer*)thing->Components.Find(Targeting)->Value;
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
