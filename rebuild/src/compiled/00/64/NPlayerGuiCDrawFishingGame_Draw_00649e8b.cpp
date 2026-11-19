// NPlayerGui::CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo&) -- while the main player's controlled creature is alive and its fishing component (TCI_FISHING) has initialised its stage: mark the drawer as drawing and, in the hooking / reeling stages (2..4), fill the strain and line-distance texts, then push the distance fraction (0 in stages 2 / 3) and the strain colour (red = 255*strain, green = 255*(1-strain)) into the quest-info mini-game bar
// Leaf: ?Draw@CDrawFishingGame@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Inline-budget: __declspec(noinline) through the header opt-in FABLE_RETAIL_IS_THING_ALIVE_NOINLINE -- retail calls the
//   IS_THING_ALIVE<const CThingPlayerCreature*> COMDAT 0x0043DBF0 out of line (visible body, so the phero frame word is shared).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00649E8B, 469 bytes, thiscall, ebp frame (`leave`), `ret 4`; /Os TU (#pragma optimize "s").
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0, IS_THING_ALIVE<const
//   CThingPlayerCreature*> 0x0043DBF0, std::bitset<274>::test 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0,
//   CTCFishing::GetPFishingRodDef const 0x007B0880, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0, NGameText::ConstructString(CWideString&, long) 0x009C7B70 (__fastcall),
//   _ftol 0x00BFEA70, CDrawQuestInfo::UpdateMiniGame(float, long, long) 0x00644EFD / (const CRGBColour&) 0x00644F85, ~CWideString 0x0099B510.
// Literals: "TEXT_GUI_FISHING_STRAIN" 0x0125A428, "TEXT_GUI_FISHING_DISTANCE" 0x0125A40C; 1.0f 0x0122DED8, 255.0f 0x01230014.
// Layout: NPlayerGui::CDrawBase (engine header: PlayerGui +4, PlayerManager +8, Component +0x10, Drawing +0x18); CPlayerGui::PDrawQuestInfo
//   +0x1C8; CMainGameComponent::PTextBank +0x14; CTCFishing (shared engine/CTCFishing.h): Stage +0x14, StageInitialised +0x18, Distance
//   +0x2C, Strain +0x34; CFishingRodDef (Ego_r.pdb; its only header is the generated
//   engine/_quarantine one, so the row keeps a local model): MaxStrain +0x34, MaxLengthMetres +0x38. Getters are the FableWin.pdb inline CTCFishing (overlay opt-in FABLE_RETAIL_CTCFISHING_DRAW_GETTERS)
//   ones (GetStage, IsStageInitialised, GetDistance, GetStrain).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
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
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_GETPLAYER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CFISHINGDEF_PASS22
#define FABLE_RETAIL_CTCFISHING_PASS22
#define FABLE_RETAIL_CTCFISHING_DRAW_GETTERS
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
#include "engine/CTCFishing.h"

class CFishingRodDef {                                       // Ego_r.pdb (60 bytes; CDefClassBase members below +0x28 not modelled)
public:
    unsigned char _pad_0x00[0x28];
    float StrainReductionPerSecond;                          // +0x28
    float DistanceDecreasePerPress;                          // +0x2C
    float PullingDistanceDecreasePerPress;                   // +0x30
    float MaxStrain;                                         // +0x34
    float MaxLengthMetres;                                   // +0x38
};

// Same-TU body: FableWin.pdb puts NPlayerGui::CDrawQuestInfo::UpdateMiniGame(const CRGBColour&) (0x020EC3DC; locals this, colour,
// it, pelement, pbar) and CDrawFishingGame::Draw in player_gui_draw_modules.obj. Retail calls it out of line (0x00644F85, landed),
// but seeing that it only reads the colour lets VC7.1 load PlayerGui->PDrawQuestInfo (`mov ecx,[eax+1C8h]`, 0x0064A036) before the
// CRGBColour temporary's B / A stores. With the declaration only, the may-alias edges from those stores hold the load back (ndiff 2).
// The debug ASSERTs (pelement, the element type, the index) emit nothing in release.
namespace NPlayerGui {
void CDrawQuestInfo::UpdateMiniGame(const CRGBColour& colour)
{
    if (MiniGameElementIndex > 0) {
        for (std::list<CCountedPointer<CQuestInfoElement> >::iterator it = Elements.begin(); it != Elements.end(); ++it) {
            CQuestInfoElement* pelement = (*it).Get();
            if (pelement->ID == MiniGameElementIndex) {
                if (pelement->GetType() == QUEST_INFO_ELEMENT_MINIGAME_BAR) {
                    CQuestInfoElementMiniGameBar* pbar = static_cast<CQuestInfoElementMiniGameBar*>(pelement);
                    if (pbar->Updatable)
                        pbar->Colour = colour;
                }
                return;
            }
        }
    }
}
}   // namespace NPlayerGui

namespace NPlayerGui {
class CDrawFishingGame : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* phero = GetPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(phero)) {
        const CTCFishing* ptc_fishing;
        if (phero->PeekTC(&ptc_fishing) && ptc_fishing->IsStageInitialised()) {
            SetAsDrawing(true);
            if (ptc_fishing->GetStage()) {
                const CFontBank& font = Component->PeekCutSceneMainFont();
                switch (ptc_fishing->GetStage()) {
                case 2:
                case 3:
                case 4: {
                    const CFishingRodDef* pfishing_rod_def = ptc_fishing->GetPFishingRodDef();
                    CWideString strain_text = Component->GetTextBank().GetTextBySymbol("TEXT_GUI_FISHING_STRAIN");
                    NGameText::ConstructString(strain_text, (long)ptc_fishing->GetStrain());
                    float max_strain = ptc_fishing->GetStrain();
                    if (pfishing_rod_def)
                        max_strain = pfishing_rod_def->MaxStrain;
                    float strain_percentage = ptc_fishing->GetStrain() / max_strain;
                    if (strain_percentage > 1.0f)
                        strain_percentage = 1.0f;
                    unsigned char strain_colour_red = (unsigned char)(strain_percentage * 255.0f);
                    unsigned char strain_colour_green = (unsigned char)((1.0f - strain_percentage) * 255.0f);
                    CWideString distance_text = Component->GetTextBank().GetTextBySymbol("TEXT_GUI_FISHING_DISTANCE");
                    NGameText::ConstructString(distance_text, (long)ptc_fishing->GetDistance());
                    float max_length = ptc_fishing->GetDistance();
                    if (pfishing_rod_def)
                        max_length = pfishing_rod_def->MaxLengthMetres;
                    float distance_percentage = ptc_fishing->GetDistance() / max_length;
                    if (distance_percentage > 1.0f)
                        distance_percentage = 1.0f;
                    unsigned char distance_colour_red = (unsigned char)(distance_percentage * 255.0f);
                    unsigned char distance_colour_green = (unsigned char)((1.0f - distance_percentage) * 255.0f);
                    if (ptc_fishing->GetStage() == 3 || ptc_fishing->GetStage() == 2)
                        PlayerGui->GetQuestInfo().UpdateMiniGame(0.0f, 0, 0);
                    else
                        PlayerGui->GetQuestInfo().UpdateMiniGame(ptc_fishing->GetDistance() / max_length, 0, 0);
                    PlayerGui->GetQuestInfo().UpdateMiniGame(CRGBColour(strain_colour_red, strain_colour_green, 0));
                    break;
                }
                }
            }
        }
    }
}
}   // namespace NPlayerGui