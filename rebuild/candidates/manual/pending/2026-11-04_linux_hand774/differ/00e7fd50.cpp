// NScript::CV_FishingCompetitionScript::GenerateSignText() -- write the competition notice onto the FishCompSign readable object
// Grade: DIFFER(802v784) -- retail keeps the STLport vector<CWideString> allocator ctor (0x00430004), _Construct (0x0040DD00, x3), the element ??_G(0) (0x0040EC50) and ~_Vector_base (0x0040F2C0) out of line inside an otherwise expanded vector; VC7.1 /O2 expands them all (try/e7fd50/x2.cpp with explicit-specialisation declarations of three of them reaches DIFFER(782v784), 19 lines: the ??_G push 0 and two register picks)
// Leaf: ?GenerateSignText@CV_FishingCompetitionScript@NScript@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7FD50, length 784 bytes, plain `ret`, no EH frame (catalog autoname
//   "FishingCompetition_Initialize" is wrong).
// Naming: FableWin.pdb ?GenerateSignText@CV_FishingCompetitionScript@NScript@@QAEXXZ (donor 0x039EDD60, immediately followed by
//   GetNextPrizeCategoryWeight 0x039EE1D0 and GetHeroTitleTextTag 0x039EE240, the two members this body uses; Ego_r 0x00EDF460);
//   behaviour: writes the TEXT_QST_B15_FISH_SIGN_INFO_{HEROWON,NORMAL} notice onto the "FishCompSign" readable object.
// Callees: interface (+0x40) slots 72 GetThingWithScriptName (+0x120), 716 GetTextString (+0xB30), 715 GetFormattedString
//   (+0xB2C), 713 SetReadableObjectText (+0xB24); GetHeroTitleTextTag 0x00E80060; CBasicString<char>::Compare 0x00411570
//   (inline CCharString::operator==(const char*)); GFFloatToWideString 0x00486C40; CWideString() 0x0099AED0 / copy 0x0099B720 /
//   operator= 0x0099B7D0 / ~ 0x0099B510; vector<CWideString>::_M_insert_overflow 0x00412330; ~CBaseClass 0x0099A2E0.
// Data: pGlobalScriptDef 0x0143E90C (FishPrizeWeights +0x5D0); "FishCompSign" 0x012EA6D0, "HERO" 0x01255174.
// Layout: Ego_r.pdb CV_FishingCompetitionScript - 0x14 (CompetitionWon +0x4C, PrizeNumber +0x64, Winner +0x68,
//   BiggestCompetitorFish +0x70). The two-decimal rounding is engine/GFPackFloat.h GFRoundFloatNear (shared GFFloatToLongNear
//   helper: bare fistp at 0x00E7FE5D).
// Retail-asm: 0x00E7FE59 `fld dword ptr [esp+1Ch]` + 0x00E7FE5D `fistp dword ptr [esp+20h]` (GFRoundFloatNear's GFFloatToLongNear).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/GFPackFloat.h"
inline CScriptThing::~CScriptThing() {}
template<> inline bool CBasicString<char>::operator==(const char* text) const { return Compare(PData, text) == 0; }
CWideString __fastcall GFFloatToWideString(double value);   // 0x00486C40
class CScriptDef { public:
    unsigned char _pad000[0x5d0];
    std::vector<float> FishPrizeWeights;          // +0x5d0
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishingCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool BountyHuntActive;                               // +0x48
    bool OrganiserRanAway;                               // +0x49
    bool OrganiserKilled;                                // +0x4a
    bool CompetitionRunning;                             // +0x4b
    bool CompetitionWon;                                 // +0x4c
    bool RodOfChampsGiven;                               // +0x4d
    bool CommentIRanAway;                                // +0x4e
    bool CompetitorsAttacked;                            // +0x4f
    bool CompetitorsJustGotAttacked;                     // +0x50
    long AttackCounter;                                  // +0x54
    long AttackDayCount;                                 // +0x58
    long HitCount;                                       // +0x5c
    bool DoneIntro;                                      // +0x60
    bool RodHintGiven;                                   // +0x61
    long PrizeNumber;                                    // +0x64
    CCharString Winner;                                  // +0x68
    float BiggestFish;                                   // +0x6c
    float BiggestCompetitorFish;                         // +0x70
    void GenerateSignText();
    float GetNextPrizeCategoryWeight();
    CCharString GetHeroTitleTextTag();
};
float CV_FishingCompetitionScript::GetNextPrizeCategoryWeight()
{
    long next = PrizeNumber + 1;
    if (PrizeNumber == -1)
        ++next;
    return pGlobalScriptDef->FishPrizeWeights[next];
}
void CV_FishingCompetitionScript::GenerateSignText()
{
    CScriptThing sign = Game->GetThingWithScriptName("FishCompSign");
    CWideString winnerName = Game->GetTextString(Winner);
    if (Winner == "HERO")
        winnerName = Game->GetTextString(GetHeroTitleTextTag());
    float nextWeight = GetNextPrizeCategoryWeight();
    std::vector<CWideString> params;
    params.push_back(GFFloatToWideString(GFRoundFloatNear(BiggestCompetitorFish, 2)));
    params.push_back(winnerName);
    CWideString text;
    if (CompetitionWon)
        text = Game->GetFormattedString("TEXT_QST_B15_FISH_SIGN_INFO_HEROWON", params);
    else {
        params.push_back(GFFloatToWideString(nextWeight));
        text = Game->GetFormattedString("TEXT_QST_B15_FISH_SIGN_INFO_NORMAL", params);
    }
    Game->SetReadableObjectText(sign, text);
}
}
