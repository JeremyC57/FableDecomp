// NPlayerGui::CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo&) -- while the main player's controlled creature is alive and its
// Grade: DIFFER(469v469) -- Q22 PORTED onto the shared headers (4 grades; CDrawQuestInfo UpdateMiniGame + CDrawBase SetAsDrawing overlay opt-ins, CThing GetTC expanded, CPlayerManager/CPlayer/CPlayerGui/CMainGameComponent text bank): residue is frame packing only -- the two implicit CCharString temporaries (the GetTextBySymbol names) share [ebp-0Ch] where retail gives them separate slots (-0Ch, -10h), so the frame is 0x14 not 0x18 and distance_text / the hero-key-strain group sit 4 bytes higher; plus one `mov ecx,[eax+1C8h]` scheduled before the colour byte stores. A nested block splits the temps but then the colour shares with them; an explicit CCharString local is worse (474); a 96-variant toy model never separates them. CTCFishing / CFishingRodDef stay local (no shared header).
//   fishing component (TCI_FISHING) has initialised its stage: mark the drawer as drawing and, in the hooking / reeling stages (2..4),
//   fill the strain and line-distance texts, then push the distance fraction (0 in stages 2 / 3) and the strain colour (red = 255*strain,
//   green = 255*(1-strain)) into the quest-info mini-game bar.
// Leaf: ?Draw@CDrawFishingGame@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00649E8B, 469 bytes, thiscall, ebp frame (`leave`), `ret 4`; /Os TU (#pragma optimize "s").
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0, IS_THING_ALIVE<const
//   CThingPlayerCreature*> 0x0043DBF0, std::bitset<274>::test 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0,
//   CTCFishing::GetPFishingRodDef const 0x007B0880, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0, NGameText::ConstructString(CWideString&, long) 0x009C7B70 (__fastcall),
//   _ftol 0x00BFEA70, CDrawQuestInfo::UpdateMiniGame(float, long, long) 0x00644EFD / (const CRGBColour&) 0x00644F85, ~CWideString 0x0099B510.
// Literals: "TEXT_GUI_FISHING_STRAIN" 0x0125A428, "TEXT_GUI_FISHING_DISTANCE" 0x0125A40C; 1.0f 0x0122DED8, 255.0f 0x01230014.
// Layout: NPlayerGui::CDrawBase (engine header: PlayerGui +4, PlayerManager +8, Component +0x10, Drawing +0x18); CPlayerGui::PDrawQuestInfo
//   +0x1C8; CMainGameComponent::PTextBank +0x14; CTCFishing (Ego_r.pdb, no shared header): Stage +0x14, StageInitialised +0x18, Distance
//   +0x2C, Strain +0x34; CFishingRodDef (Ego_r.pdb): MaxStrain +0x34, MaxLengthMetres +0x38. Getters are the FableWin.pdb inline CTCFishing
//   ones (GetStage, IsStageInitialised, GetDistance, GetStrain).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CDRAWQUESTINFO_UPDATEMINIGAME
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/ThingAlive.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"

enum EFishingStage { };
class CFishingRodDef {                                       // Ego_r.pdb (60 bytes; CDefClassBase members below +0x28 not modelled)
public:
    unsigned char _pad_0x00[0x28];
    float StrainReductionPerSecond;                          // +0x28
    float DistanceDecreasePerPress;                          // +0x2C
    float PullingDistanceDecreasePerPress;                   // +0x30
    float MaxStrain;                                         // +0x34
    float MaxLengthMetres;                                   // +0x38
};
class CTCFishing : public CTCBase {                          // Ego_r.pdb CTCFishing (no shared header)
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_FISHING; }
    EFishingStage GetStage() const { return Stage; }         // FableWin.pdb inline
    bool IsStageInitialised() const { return StageInitialised; }   // FableWin.pdb inline
    float GetDistance() const { return Distance; }           // FableWin.pdb inline
    float GetStrain() const { return Strain; }               // FableWin.pdb inline
    const CFishingRodDef* GetPFishingRodDef() const;         // 0x007B0880
    CIntelligentPointer<CThing> PFish;                       // +0x0C
    EFishingStage Stage;                                     // +0x14
    bool StageInitialised;                                   // +0x18
    long FrameStageFinishes;                                 // +0x1C
    long HookPresses;                                        // +0x20
    bool PressedThisFrame;                                   // +0x24
    long LastFramePressed;                                   // +0x28
    float Distance;                                          // +0x2C
    float MaxDistance;                                       // +0x30
    float Strain;                                            // +0x34
};

namespace NPlayerGui {
class CDrawFishingGame : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    const CTCFishing* fishing;
    if (IS_THING_ALIVE(hero) && hero->PeekTC(&fishing) && fishing->IsStageInitialised()) {
        SetAsDrawing(true);
        EFishingStage stage = fishing->GetStage();
        if (stage != 0 && stage >= 2 && stage <= 4) {
            const CFishingRodDef* rod = fishing->GetPFishingRodDef();
            CWideString strain_text = Component->GetTextBank().GetTextBySymbol("TEXT_GUI_FISHING_STRAIN");
            NGameText::ConstructString(strain_text, (long)fishing->GetStrain());
            float max_strain = fishing->GetStrain();
            if (rod)
                max_strain = rod->MaxStrain;
            float strain = fishing->GetStrain() / max_strain;
            if (strain > 1.0f)
                strain = 1.0f;
            unsigned char red = (unsigned char)(strain * 255.0f);
            unsigned char green = (unsigned char)((1.0f - strain) * 255.0f);
            CWideString distance_text = Component->GetTextBank().GetTextBySymbol("TEXT_GUI_FISHING_DISTANCE");
            NGameText::ConstructString(distance_text, (long)fishing->GetDistance());
            float max_distance = fishing->GetDistance();
            if (rod)
                max_distance = rod->MaxLengthMetres;
            if (fishing->GetStage() == 3 || fishing->GetStage() == 2)
                PlayerGui->GetQuestInfo().UpdateMiniGame(0.0f, 0, 0);
            else
                PlayerGui->GetQuestInfo().UpdateMiniGame(fishing->GetDistance() / max_distance, 0, 0);
            CRGBColour colour(red, green, 0, 0xff);
            PlayerGui->GetQuestInfo().UpdateMiniGame(colour);
        }
    }
}
}   // namespace NPlayerGui
