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
#ifdef FABLE_RETAIL_CREATEFISH_PASS29
protected:
 bool CreateFish(bool using_fishing_spot);
public:
#endif
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
#ifdef FABLE_RETAIL_CTCFISHING_GOFISHING
 // Nested opt-in (NF435, row 0x00895A90 CGameScriptInterface::HeroGoFishing): Ego_r.pdb / FableWin.pdb ?GoFishing@CTCFishing@@QAEX_N@Z, out of
 // line in retail at 0x007B1EB0 (landed). Declaration only: no layout change.
 void GoFishing(bool);                                                  // 0x007B1EB0
#endif
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
#ifdef FABLE_RETAIL_CTCFISHING_CANCELBUTTONPRESSED
 // NF120 opt-in (PRIORITY q6450, row 0x007B0CC0): Ego_r.pdb / FableWin.pdb ?CancelButtonPressed@CTCFishing@@QAEXXZ (public; Ego_r order
 // ReelInButtonPressed, CancelButtonPressed, ..., FrameUpdate = retail 0x007B0BA0, 0x007B0CC0, 0x007B0CD0) and the protected
 // ?FinishFishing@CTCFishing@@IAEX_N@Z (out of line at 0x007B0950). Declarations only: no layout change.
 void CancelButtonPressed();                                                 // 0x007B0CC0
protected:
 void FinishFishing(bool caught);                                            // 0x007B0950
public:
#endif
};
FABLE_STATIC_ASSERT_AT(36, offsetof(CTCFishing,PDef)==0x60);
FABLE_STATIC_ASSERT_AT(37, sizeof(CTCFishing)==100);
#endif
#endif
// Retail opt-in (Codex pass22): complete original both-PDB100-byte component; original counted getter body.
