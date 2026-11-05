// NScript::CV_FishingCompetitionScript::GetHeroTitleTextTag() -- the GUI text tag of the hero's current title ("" for none)
// Leaf: ?GetHeroTitleTextTag@CV_FishingCompetitionScript@NScript@@QAE?AVCCharString@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E80060, length 312 bytes (jump table of 20 at 0x00E80148 inside the row), `ret 4`.
// Naming: Ego_r.pdb / FableWin.pdb ?GetHeroTitleTextTag@CV_FishingCompetitionScript@NScript@@QAE?AVCCharString@@XZ; the
//   catalog's own name. EHeroTitle enumerators from the game's misc_def_types.h (ghidra_out/installed_game/header_enums.csv),
//   each one paired by hand with the retail table: 1 REAPER -> 0x012EA818 ... 20 ARSEFACE -> 0x012EA9E4.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E), CCharString::operator=(const char*) 0x0099EFE0,
//   copy ctor 0x0099EC30 (the by-value return: VC7.1 has no NRVO), ~CCharString 0x0099EAE0; through the game script
//   interface (+0x40) GetHeroTitle (slot 205, +0x334).
// Shape: the cases are written in the alphabetical order of their tags, which is retail's block order; unknown titles
//   (NONE and the TLC titles 21..32) keep "".
// Layout: CLevelScriptBase<T> (engine/CLevelScriptBase.h): Game +0x40; the master-data type follows the landed ctor 0x00E7E110.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
enum EHeroTitle { TITLE_NONE=0, TITLE_REAPER=1, TITLE_SHADOWHUNTER=2, TITLE_MALEFICUS=3, TITLE_DEATHBRINGER=4, TITLE_ASSASSIN=5, TITLE_NECROMANCER=6, TITLE_AVATAR=7, TITLE_PILGRIM=8, TITLE_LIBERATOR=9, TITLE_PALADIN=10, TITLE_DRUID=11, TITLE_RANGER=12, TITLE_RUNEMASTER=13, TITLE_HOOD=14, TITLE_GLADIATOR=15, TITLE_SABRE=16, TITLE_ARROWDODGER=17, TITLE_PIEMASTER=18, TITLE_CHICKEN_CHASER=19, TITLE_ARSEFACE=20, TITLE_JACK=21, TITLE_MAZE=22, TITLE_SCARLET_ROBE=23, TITLE_SCYTHE=24, TITLE_THUNDER=25, TITLE_WHISPER=26, TITLE_TWINBLADE=27, TITLE_BRIAR_ROSE=28, TITLE_LADY_GREY=29, TITLE_GUILDMASTER=30, TITLE_SCORPION_SLAYER=31, TITLE_DEATH_BRINGER=32 };
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishingCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CCharString GetHeroTitleTextTag();
};
}

CCharString NScript::CV_FishingCompetitionScript::GetHeroTitleTextTag()
{
    CCharString tag = "";
    switch (Game->GetHeroTitle())
    {
    case TITLE_ARROWDODGER:    tag = "TEXT_GUI_HERO_TITLE_ARROWDODGER"; break;
    case TITLE_ARSEFACE:       tag = "TEXT_GUI_HERO_TITLE_ARSEFACE"; break;
    case TITLE_ASSASSIN:       tag = "TEXT_GUI_HERO_TITLE_ASSASSIN"; break;
    case TITLE_AVATAR:         tag = "TEXT_GUI_HERO_TITLE_AVATAR"; break;
    case TITLE_CHICKEN_CHASER: tag = "TEXT_GUI_HERO_TITLE_CHICKEN_CHASER"; break;
    case TITLE_DEATHBRINGER:   tag = "TEXT_GUI_HERO_TITLE_DEATHBRINGER"; break;
    case TITLE_DRUID:          tag = "TEXT_GUI_HERO_TITLE_DRUID"; break;
    case TITLE_GLADIATOR:      tag = "TEXT_GUI_HERO_TITLE_GLADIATOR"; break;
    case TITLE_HOOD:           tag = "TEXT_GUI_HERO_TITLE_HOOD"; break;
    case TITLE_LIBERATOR:      tag = "TEXT_GUI_HERO_TITLE_LIBERATOR"; break;
    case TITLE_MALEFICUS:      tag = "TEXT_GUI_HERO_TITLE_MALEFICUS"; break;
    case TITLE_NECROMANCER:    tag = "TEXT_GUI_HERO_TITLE_NECROMANCER"; break;
    case TITLE_PALADIN:        tag = "TEXT_GUI_HERO_TITLE_PALADIN"; break;
    case TITLE_PIEMASTER:      tag = "TEXT_GUI_HERO_TITLE_PIEMASTER"; break;
    case TITLE_PILGRIM:        tag = "TEXT_GUI_HERO_TITLE_PILGRIM"; break;
    case TITLE_RANGER:         tag = "TEXT_GUI_HERO_TITLE_RANGER"; break;
    case TITLE_REAPER:         tag = "TEXT_GUI_HERO_TITLE_REAPER"; break;
    case TITLE_RUNEMASTER:     tag = "TEXT_GUI_HERO_TITLE_RUNEMASTER"; break;
    case TITLE_SABRE:          tag = "TEXT_GUI_HERO_TITLE_SABRE"; break;
    case TITLE_SHADOWHUNTER:   tag = "TEXT_GUI_HERO_TITLE_SHADOWHUNTER"; break;
    }
    return tag;
}