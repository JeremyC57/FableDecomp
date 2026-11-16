// CTCHeroOnlineScoreboard::CActivity::GetScoreAsString(float) -- format the score to its configured precision and append the localized unit.
// Leaf: ?GetScoreAsString@CActivity@CTCHeroOnlineScoreboard@@QAE?AVCWideString@@M@Z
// Retail: 0x0055E6B0, 263 bytes. Name: Ego_r.pdb 0x005D7890 ?GetScoreAsString@CActivity@CTCHeroOnlineScoreboard@@QAE?AVCWideString@@M@Z
//   (nested class, non-const); the landed caller DisplayScoreboardGUI 0x00563920 calls it on ActivityInfo[i] with that spelling.
// Layout: engine/CTCHeroOnlineScoreboard.h CActivity (Ego_r.pdb): Name +0, Description +4, UnitOfMeasurement +8,
//   NumDecPlacesToShow +0xC, LowerIsBetter +0xD.
// Calls: string ctor 0x0099AED0 / copy 0x0099B720 / dtor 0x0099B510, assignment 0x0099B7D0, append 0x0099B940 / 0x0099B8D0;
//   Sprintf 0x0099BA70 (the inline GFIntToWideString); GFFloatToWideString 0x00486C40; FindCharPos 0x0099B070, SetLength
//   0x0099BC50; NGameText::CDataBank::GetTextBySymbol 0x009C95E0 on CMainGameComponent::Get()->GetTextBank() (0x013B86A0 +0x14).
// NF41 (2026-11-16): was a global `struct CActivity` (const) with a local CMainGameComponent model and a local IntToWideString;
//   now the nested class of the shared engine/CTCHeroOnlineScoreboard.h, engine/CMainGameComponent.h (TEXTBANK),
//   engine/NGameText_CDataBank.h and engine/GFIntToWideString.h (the include chain of the landed caller 0x00563920).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/GFIntToWideString.h"
#include "engine/CTCHeroOnlineScoreboard.h"
CWideString FABLE_FASTCALL GFFloatToWideString(double value);   // 0x00486C40 Ego_r ?GFFloatToWideString@@YI?AVCWideString@@N@Z

CWideString CTCHeroOnlineScoreboard::CActivity::GetScoreAsString(float score)
{
    CWideString result;
    if (!NumDecPlacesToShow) result=GFIntToWideString((long)score);
    else {
        CWideString value=GFFloatToWideString(score);
        value.SetLength(value.FindCharPos(L'.')+1+NumDecPlacesToShow);
        result=value;
    }
    result+=L" ";
    result+=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(UnitOfMeasurement);
    return result;
}
