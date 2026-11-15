#ifndef FABLE_ENGINE_CTCFISHING_H
#define FABLE_ENGINE_CTCFISHING_H
#ifdef FABLE_RETAIL_CTCFISHING_PASS22
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CFishingDef.h"
#include "fable_string.h"
#ifdef _MSC_VER
enum EFishingStage;
#else
enum EFishingStage {};
#endif
class CTCFishing:public CTCBase {
public:
 CIntelligentPointer<CThing> PFish;
 EFishingStage Stage;
 bool StageInitialised;
 long FrameStageFinishes,HookPresses;
 bool PressedThisFrame;
 long LastFramePressed;
 float Distance,MaxDistance,Strain;
 long FrameReelFinishes;
 bool Pulling;
 long Score;
 CCharString Text;
 long TextTimer;
 bool InstructionsShown;
 long FrameStarted;
 unsigned long ReelingInSoundID,FishEscapingSoundID;
 long CurrentFishWeight;
 CDefPointer<const CFishingDef> PDef;
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_FISHING;}
 CDefPointer<const CFishingDef> GetPDef()const{return PDef;}
};
FABLE_STATIC_ASSERT(offsetof(CTCFishing,PDef)==0x60);
FABLE_STATIC_ASSERT(sizeof(CTCFishing)==100);
#endif
#endif
// Retail opt-in (Codex pass22): complete original both-PDB100-byte component; original counted getter body.
