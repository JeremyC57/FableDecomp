// CBS_Teacher::GetBookSpecificArgs(long, map<CCharString, CCharString>&) -- fill the text arguments of the teacher's comment on a returned book: books 15/17/19/21/22/23 pick their line by the game-flow position, the hero's attractiveness or scariness, the Twinblade outcome or the teddy solution ($ARG1..$ARG3)
// Leaf: ?GetBookSpecificArgs@CBS_Teacher@CV_BookCollectingScript@NScript@@AAEXJAAV?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E57020, length 1296 bytes, `ret 8`.
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / (const CCharString&)
//   0x0099EC30 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0 / operator=(const CCharString&) 0x0099EFB0,
//   operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`, hidden result), map<CCharString,
//   CCharString>::operator[] 0x009AC700, Compare 0x00411570 (the inline CCharString == const char*).
//   CGameScriptInterface slots 178 GetHeroScariness, 179 GetHeroAttractiveness.
// Layout: Ego_r.pdb CBS_Teacher (68 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
//   +0x18 (CQ_SunnyvaleMasterData: PostSavePosition +4, BanditCampTwinbladeKilled +0x3a, TeddySolution +0x54).
// NF87 (2026-11-17): the args map is the STLport std::map<CCharString, CCharString> (stlport/fable_map.h), so its operator[] binds
//   0x009AC700 under its real spelling, and the class is the nested NScript::CV_BookCollectingScript::CBS_Teacher with the private
//   member Ego_r.pdb + FableWin.pdb spell (?GetBookSpecificArgs@CBS_Teacher@CV_BookCollectingScript@NScript@@AAEXJAAV?$map@...@Z).
//   Shared headers: fable_string.h (FORCEINLINE_TEXT_EQ, CBASICSTRING_TEXT_EQ_INLINE: the inline == "A" with Compare 0x00411570),
//   engine/CEntityScriptBase.h, engine/CQ_SunnyvaleMasterData.h (PostSavePosition, BanditCampTwinbladeKilled, TeddySolution).
//   CV_BookCollectingScript has no shared header: the minimal local shell below (only the nested class and the two members this
//   row reads), as the landed sibling 0x00E55CE0 models it locally.
#include "fable_compiler.h"
#include <string.h>
#pragma intrinsic(strcmp)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
namespace NScript {
class CV_BookCollectingScript {
public:
    class CBS_Teacher : public CEntityScriptBase {
    private:
        void GetBookSpecificArgs(long book, std::map<CCharString, CCharString>& args);
    protected:
        CV_BookCollectingScript* ParentClass;                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                 // +0x18
    };
};
}

void NScript::CV_BookCollectingScript::CBS_Teacher::GetBookSpecificArgs(long book, std::map<CCharString, CCharString>& args)
{
    if (book == 15) {
        if (IsActiveThreadTerminating())
            return;
        if (PMasterData->PostSavePosition <= 1250) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90D";
        } else if (PMasterData->PostSavePosition <= 1700) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90A";
        } else if (PMasterData->PostSavePosition <= 2300) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90B";
        } else if (PMasterData->PostSavePosition > 2600) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90B";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90C";
        }
    } else if (book == 17) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHeroAttractiveness() > 0.5) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK17_20B";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK17_20A";
        }
    } else if (book == 19) {
        if (IsActiveThreadTerminating())
            return;
        if (PMasterData->PostSavePosition <= 700) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK19_80A";
        } else if (PMasterData->BanditCampTwinbladeKilled) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK19_80C";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK19_80B";
        }
    } else if (book == 21) {
        if (IsActiveThreadTerminating())
            return;
        CCharString solution(PMasterData->TeddySolution);
        args["$ARG1"] = "TEXT_CS_B16_BOOK21_20" + solution;
        args["$ARG2"] = "TEXT_CS_B16_BOOK21_30" + solution;
        if (solution == "A") {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG3"] = "TEXT_CS_B16_BOOK21_40A";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG3"] = "TEXT_CS_B16_BOOK21_40B";
        }
    } else if (book == 22) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHeroScariness() > 0.5) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK22_20B";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK22_20A";
        }
    } else if (book == 23) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHeroAttractiveness() < -0.5) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK23_10A";
        } else if (Game->GetHeroAttractiveness() > 0.5) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK23_10C";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK23_10B";
        }
    }
}
