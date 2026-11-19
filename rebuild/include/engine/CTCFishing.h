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
#ifdef FABLE_RETAIL_CTCFISHING_DRAW_GETTERS
 // Nested opt-in (hand4a, row 0x00649E8B NPlayerGui::CDrawFishingGame::Draw): FableWin.pdb's inline const getters
 // ?GetStage@CTCFishing@@QBE?AW4EFishingStage@@XZ, ?IsStageInitialised@CTCFishing@@QBE_NXZ, ?GetDistance@CTCFishing@@QBEMXZ and
 // ?GetStrain@CTCFishing@@QBEMXZ (retail reads +0x14 / +0x18 / +0x2C / +0x34 in place), and Ego_r.pdb + FableWin.pdb
 // ?GetPFishingRodDef@CTCFishing@@QBEPBVCFishingRodDef@@XZ (out of line in retail at 0x007B0880). No layout change.
 EFishingStage GetStage()const{return Stage;}
 bool IsStageInitialised()const{return StageInitialised;}
 float GetDistance()const{return Distance;}
 float GetStrain()const{return Strain;}
 const class CFishingRodDef* GetPFishingRodDef()const;                       // 0x007B0880
#endif
};
FABLE_STATIC_ASSERT_AT(36, offsetof(CTCFishing,PDef)==0x60);
FABLE_STATIC_ASSERT_AT(37, sizeof(CTCFishing)==100);
#endif
#endif
// Retail opt-in (Codex pass22): complete original both-PDB100-byte component; original counted getter body.
