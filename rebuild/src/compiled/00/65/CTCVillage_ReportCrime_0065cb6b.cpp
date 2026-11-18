// CTCVillage::ReportCrime(long deed_handle) -- record a crime against the village: tell the hero's deed log, add its fine to the hero's crime stats (and the online scoreboard), for a murder mark the village hostile, drop the assault / theft crimes against the same victim and count the murder, for an assault / theft fold it into a known murder; keep the handle in the village crime set
// Leaf: ?ReportCrime@CTCVillage@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065CB6B, 747 bytes, `ret 4`, ebp frame (size TU: optimize("y",off) + ("s",on)).
//   Ego_r.pdb / FableWin.pdb ?ReportCrime@CTCVillage@@QAEXJ@Z; Ego_d.pdb tc_village.obj (locals player / ptc_deed_log / deed /
//   pdeed_effects_def / cur_fine_amount / HeroStats / ptc_hero_scoreboard / ptc_hero_stats / old_crime and their scopes).
// Callees: CMainGameComponent::Get() / PeekWorld() (inline), CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetPControlledCreature 0x00487DC0,
//   std::bitset<274>::test 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the expanded CThing::GetTC),
//   CTCHeroOpinionDeedLog::SetVillageForCrimes 0x006C55E0 / SetCrimeDeedReportedToVillage 0x006C5A50 / GetDeedFromHandle 0x006C58F0 /
//   PeekPDeedEffectsDef 0x006C5630 / FindCriminalDeed 0x006C5800, CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer
//   0x00449970, CPlayer::GetPPlayerCharacter (inline: GetPItem 0x00A01B50 on +0x34), CTCHeroOnlineScoreboard::UpdateScoreForActivity
//   0x0055F3B0, CTCVillage::RemoveCrime 0x0065C146, set<long> _M_find 0x00614A10 (STLport const find) and set<long>::insert 0x00514230.
// Shape: Ego_d's scopes verbatim (player; { ptc_deed_log { deed { pdeed_effects_def, cur_fine_amount { HeroStats { ptc_hero_scoreboard } }
//   { ptc_hero_stats, old_crime, ptc_hero_scoreboard } { old_crime } } } }); every GetTC result except HeroStats is unused (on a miss
//   retail reads the never-written local's home: 0x0065CBD0, 0x0065CCB6, 0x0065CD74, 0x0065CDB5). The CTCHeroStats crime counters are
//   the inline Get/Set accessors Ego_d keeps right after ReportCrime in tc_village.obj.
// Set lookup: the one-expression `PlayersCrimes.find(old_crime) == PlayersCrimes.end()` (no local: Ego_d lists only old_crime in that
//   block) gives retail's find-then-compare (0x0065CDFD `call 0x00614A10 ; cmp eax,[esi]`); audit e11: identical object to the named-iterator text.
// Inline-budget: FABLE_RETAIL_STLP_BITSET_TEST_DECL (existing out-of-line declaration opt-in, owner-approved 2026-11-04): retail calls
//   std::bitset<274>::test 0x00410DE0 at every presence test of this row (0x0065CBA1, 0x0065CC1F, 0x0065CC82, 0x0065CD41, 0x0065CD86).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#define FABLE_RETAIL_CTCHEROSTATS_CRIMECOUNTS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CRIMES
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES
#define FABLE_RETAIL_CTCVILLAGE_CRIMES
#define FABLE_RETAIL_CTCVILLAGE_WITNESSEDCRIME
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCVillage.h"

void CTCVillage::ReportCrime(long deed_handle)
{
    CThingPlayerCreature* player = CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
    if (player) {
        CTCHeroOpinionDeedLog* ptc_deed_log;
        player->GetTC(&ptc_deed_log);
        ptc_deed_log->SetVillageForCrimes(GetParentThing());
        ptc_deed_log->SetCrimeDeedReportedToVillage(deed_handle);
        {
            const COpinionDeed* deed = ptc_deed_log->GetDeedFromHandle(deed_handle);
            if (deed) {
                CDefPointer<const COpinionDeedEffectsDef> pdeed_effects_def = ptc_deed_log->PeekPDeedEffectsDef(deed->DeedType);
                long cur_fine_amount = pdeed_effects_def->CrimeFineAmount;
                if (cur_fine_amount != -1) {
                    CTCHeroStats* HeroStats;
                    if (player->GetTC(&HeroStats)) {
                        HeroStats->SetNumberOfCrimes(HeroStats->GetNumberOfCrimes() + 1);
                        HeroStats->SetTotalFine(HeroStats->GetTotalFine() + cur_fine_amount);
                        CTCHeroOnlineScoreboard* ptc_hero_scoreboard;
                        GetPlayerManager().GetMainPlayer().GetPPlayerCharacter()->GetTC(&ptc_hero_scoreboard);
                        ptc_hero_scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0x23, (float)HeroStats->GetTotalFine(), false);
                        ptc_hero_scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0x22, (float)HeroStats->GetNumberOfCrimes(), false);
                    }
                }
                if (deed->DeedType == OPINION_DEED_TYPE_CRIME_MURDER) {
                    CTCHeroStats* ptc_hero_stats;
                    IsEnemyBecauseOfCrime = true;
                    long old_crime = ptc_deed_log->FindCriminalDeed(OPINION_DEED_TYPE_CRIME_ASSAULT, deed->PTarget.Get());
                    if (old_crime != -1)
                        RemoveCrime(old_crime);
                    old_crime = ptc_deed_log->FindCriminalDeed(OPINION_DEED_TYPE_CRIME_GBH, deed->PTarget.Get());
                    if (old_crime != -1)
                        RemoveCrime(old_crime);
                    player->GetTC(&ptc_hero_stats);
                    ptc_hero_stats->SetNumberOfCrimesOfMurder(ptc_hero_stats->GetNumberOfCrimesOfMurder() + 1);
                    CTCHeroOnlineScoreboard* ptc_hero_scoreboard;
                    player->GetTC(&ptc_hero_scoreboard);
                    ptc_hero_scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0, (float)ptc_hero_stats->GetNumberOfCrimesOfMurder(), false);
                } else if (deed->DeedType == OPINION_DEED_TYPE_CRIME_ASSAULT || deed->DeedType == OPINION_DEED_TYPE_CRIME_GBH) {
                    long old_crime = ptc_deed_log->FindCriminalDeed(OPINION_DEED_TYPE_CRIME_MURDER, deed->PTarget.Get());
                    if (old_crime != -1) {
                        if (PlayersCrimes.find(old_crime) == PlayersCrimes.end())
                            ReportCrime(old_crime);
                        RemoveCrime(deed_handle);
                        deed_handle = -1;
                    }
                }
            }
        }
    }
    if (deed_handle != -1)
        PlayersCrimes.insert(deed_handle);
}