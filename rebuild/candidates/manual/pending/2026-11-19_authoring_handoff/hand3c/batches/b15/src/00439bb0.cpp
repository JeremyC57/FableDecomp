// CPlayerGui::UpdateMoney() -- refresh the money stat bar from the hero's gold: when the gold differs from the bar's current value, queue a "+N"/"-N" change text (a drop also plays the GOLD_PAYMENT sound criteria), then animate an initialised bar towards the gold or snap an uninitialised one to it
// Leaf: ?UpdateMoney@CPlayerGui@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00439BB0, length 592 bytes, plain `ret`.
// Shape: FableWin.pdb ?UpdateMoney@CPlayerGui@@AAEXXZ (FableWin 0x01A814F0), statement for statement; local names from Ego_d.pdb
//   (phero, ptc_hero_stats, current_money, stored_money_value, text_prefix, text_colour, ptc_sound_player, change_text,
//   difference_between_money_got_and_money_displaying, amount_to_change_per_frame). The debug build's GetTC ASSERT is not in retail.
//   The uninitialised branch is three stores in member order, each through PMoney (retail +0xC, +0x10, +0x14, re-reading +0x2CC each
//   time, 0x00439DD4..0x00439DED); a chained assignment stores +0x14 first.
// Callees: NPlayerGui::CStatBar::UpdateChangeTexts 0x00646B12, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetPPlayerCharacterThing 0x00487B70, CVectorMap::LowerBound 0x0040F020 (GetTC<CTCHeroStats> / <CTCSoundPlayer>, expanded),
//   CWideString() 0x0099AED0 / operator=(const wchar_t*) 0x0099B800 / ~CWideString 0x0099B510, CPlayerGui::PeekGuiDef 0x00438570,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00,
//   GFIntToWideString 0x00405F00 (__fastcall COMDAT), operator+(const CWideString&, const CWideString&) 0x0099BE70,
//   CStatBar::AddChangeText 0x0064DAE0, _ftol2 0x00BFEA70, CPlayerGui::UpdateStat 0x00437AE0.
// Retail-asm: 0x00439C3D `fld dword ptr [esp+1Ch]` + 0x00439C41 `fistp dword ptr [esp+18h]` -- the engine's inline-assembly
//   float-to-long helper GFFloatToLongNear (engine/GFFloatToLongNear.h; FableWin calls ?GFFloatToLongNear@@YIJM@Z at 0x01A81717) on
//   the bar's CurrentValue. This row carries no asm of its own.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_UPDATEMONEY
#define FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGETEXTS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/NPlayerGui_CStatBar.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/GFFloatToLongNear.h"
#include <math.h>
#include <stdlib.h>

CWideString FABLE_FASTCALL GFIntToWideString(long value);                 // 0x00405F00
// FableWin.pdb ??$GFLimitLower@JH@@YIXAAJABH@Z (debug 0x01A818BD call); inline in retail (`cmp eax,1 ; jge`, 0x00439D63).
template<class T,class L> inline void FABLE_FASTCALL GFLimitLower(T& value,const L& lower) { if (value < lower) value = lower; }

void CPlayerGui::UpdateMoney()
{
    PMoney->UpdateChangeTexts();
    CThing* phero = PlayerManager->GetMainPlayer().GetPPlayerCharacterThing();
    if (!IS_THING_ALIVE(phero))
        return;
    CTCHeroStats* ptc_hero_stats;
    phero->GetTC(&ptc_hero_stats);
    long current_money = ptc_hero_stats->GetMoney();
    if (PMoney->ValueInitialised) {
        long stored_money_value = GFFloatToLongNear(PMoney->CurrentValue);
        if (current_money != stored_money_value) {
            CWideString text_prefix;
            CRGBColour text_colour;
            if (current_money > stored_money_value) {
                text_colour = PeekGuiDef().MoneyIncreaseTextColour;
                text_prefix = L"+";
            } else {
                text_colour = PeekGuiDef().MoneyDecreaseTextColour;
                text_prefix = L"-";
                CTCSoundPlayer* ptc_sound_player;
                if (phero->GetTC(&ptc_sound_player))
                    ptc_sound_player->PlayAttachedCriteria("GOLD_PAYMENT", true, false, false, -1);
            }
            CWideString change_text = text_prefix + GFIntToWideString(abs(current_money - stored_money_value));
            PMoney->AddChangeText(text_colour, change_text, current_money - stored_money_value, true);
        }
        float difference_between_money_got_and_money_displaying = fabsf(PMoney->OldValue - PMoney->CurrentValue) - 0.2f;
        long amount_to_change_per_frame = (long)(difference_between_money_got_and_money_displaying * 0.5f);
        GFLimitLower(amount_to_change_per_frame, 1);
        UpdateStat((float)current_money, 0.01f, (float)amount_to_change_per_frame,
                   PeekGuiDef().SecondsBeforeStatsBackBarMoves, PeekGuiDef().SecondsToShowStatsBars, *PMoney.Get());
        PMoney->OldGraphicFrame = PMoney->GraphicFrame;
        PMoney->GraphicFrame++;
    } else {
        PMoney->CurrentValue = (float)current_money;
        PMoney->OldValue = (float)current_money;
        PMoney->LastOldValue = (float)current_money;
        PMoney->ValueInitialised = true;
    }
}
