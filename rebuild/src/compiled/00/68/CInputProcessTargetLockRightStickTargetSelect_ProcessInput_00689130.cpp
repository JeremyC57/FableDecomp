// CInputProcessTargetLockRightStickTargetSelect::ProcessInput(const CInputEvent&,CProcessedInput&) -- edge-trigger right-stick target changes.
// Leaf: ?ProcessInput@CInputProcessTargetLockRightStickTargetSelect@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was ?ProcessInput@TargetSelect@NativeInput@@QAE.. on a NativeInput namespace model; respelt
//   onto engine/CInputProcessTargetLockRightStickTargetSelect.h FABLE_RETAIL_INPUTPROCESS_VTABLE (Ego_r.pdb / FableWin.pdb public virtual, retail
//   vftable slot 4). Callees on their PDB spellings: CInputEvent::GetType 0x00A03B40 / GetJoystickPosition 0x00A043A0 (engine/CInputEvent.h
//   FABLE_RETAIL_CINPUTEVENT_ACCESSORS) and CTCTargetingPlayer::SetAsTargetNextThingInDirection 0x008023C0 (add-only opt-in
//   FABLE_RETAIL_CTCTARGETINGPLAYER_SETASTARGETNEXTTHINGINDIRECTION); TABLE_CHANGES.tsv rebinds the three NativeInput spellings.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessTargetLockRightStickTargetSelect.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "engine/CProcessedInput.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CINPUTEVENT_ACCESSORS
#include "engine/CInputEvent.h"
#include "engine/_quarantine/CThing.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_SETASTARGETNEXTTHINGINDIRECTION
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
typedef ::CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
// Retail CThing: component bitset36 vs donor40, vector map16 vs20, list8 vs12.
struct ThingPlayerCreature {unsigned char Prefix[0x20];unsigned int ComponentBits[9];ComponentMap Components;unsigned char RemainingState[0x3d];unsigned char LifeFlags;};
}
class CThingPlayerCreature : public NativeInput::ThingPlayerCreature { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

void CInputProcessTargetLockRightStickTargetSelect::ProcessInput(const CInputEvent& event,CProcessedInput& output){
 NativeInput::ThingPlayerCreature* thing=MyPlayer->GetPControlledCreature();
 if(thing && !(thing->LifeFlags&1)){
  CTCTargetingPlayer* target=0;
  if(thing->ComponentBits[0]&(1<<8))target=(CTCTargetingPlayer*)thing->Components.Find(Targeting)->second;
  if(event.GetType()==18){
   C2DVector pos=event.GetJoystickPosition();
   if(pos.X<.25f)ChangedTargetRight=false;
   if(pos.X>-.25f)ChangedTargetLeft=false;
   if(pos.X>.65f && !ChangedTargetRight){target->SetAsTargetNextThingInDirection(CTCTargetingPlayer::TARGET_NEXT_RIGHT);ChangedTargetRight=true;}
   else if(pos.X<-.65f && !ChangedTargetLeft){target->SetAsTargetNextThingInDirection(CTCTargetingPlayer::TARGET_NEXT_LEFT);ChangedTargetLeft=true;}
   output.SetAsPerformedEvent();
  }
 }
}
