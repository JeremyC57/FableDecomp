// 007e5570 CTCBetting::GetPlayersMoney
// Leaf: ?GetPlayersMoney@CTCBetting@@QAE?AVCWideString@@XZ
// Flags: /Zc:wchar_t
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKPLAYER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CBETTINGDEF
#define FABLE_RETAIL_CTCBETTING
#define FABLE_RETAIL_CTCAISCRATCHPAD_BETTING
#include "fable_string.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroStats.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "fable_game.h"
#include "engine/CTCBetting.h"
#include "engine/CBettingDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCAIScratchpad.h"
CWideString FABLE_FASTCALL GFIntToWideString(long);
// Original GetHeroStats0279ae70 uses the const player-manager accessor and GetTC.
inline void CTCBetting::GetHeroStats(CTCHeroStats** ptc_hero_stats)
{
 CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(ptc_hero_stats);
}
// Original template body at native007e5820; ordinary call in this row.
#if defined(_MSC_VER)
#pragma auto_inline(off)
#endif
#include "engine/CDefClassBase_GetSubDef.h"
#if defined(_MSC_VER)
#pragma auto_inline(on)
#endif
CWideString CTCBetting::GetPlayersMoney()
{
 CTCHeroStats* ptc_hero_stats=0;
 GetHeroStats(&ptc_hero_stats);
 CDefPointer<const CBettingDef> betting_def;
 long money=ptc_hero_stats->GetMoney();
 if(CTCAIScratchpad::CountBetMoneyDown)money-=CTCAIScratchpad::BettingAmount;
 if(CTCAIScratchpad::BettingActive){
  if(GetParentThing().PeekBaseDef().GetSubDef(&betting_def))
   return GetText(betting_def->PlayersMoney)+GFIntToWideString(money);
 }
 return CWideString(L"");
}