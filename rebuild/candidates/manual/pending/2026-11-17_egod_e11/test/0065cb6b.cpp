// Behaviour test for 0x0065CB6B CTCVillage::ReportCrime(long): the row's defines, includes and body verbatim over the shared headers,
// run on zeroed object images. Doubles: bitset::test (bit table), CVectorMap::LowerBound (pair table), PeekMyPlayer /
// GetPControlledCreature, the deed-log calls (SetVillageForCrimes, SetCrimeDeedReportedToVillage, GetDeedFromHandle, PeekPDeedEffectsDef,
// FindCriminalDeed), GetPlayerManager / GetMainPlayer, GetPItem, UpdateScoreForActivity, RemoveCrime and the set<long> insert_unique;
// the set's find walks a real one-node STLport tree. Cases: no player (handle kept / -1), no deed, fine -1 / 0 / 50 with and without
// hero stats, murder with each related crime found or not, assault / GBH with no murder, with a reported murder, with an unreported
// murder (recursive report), and the other deed types; negative non-sentinel values (-2, -3, -4, -5, -7) at every `!= -1` test.
#include <stdio.h>
#include <string.h>
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

static int g_bad=0;
static void check(bool ok, int line) { if (!ok) { printf("check failed line %d\n", line); ++g_bad; } }
static long g_ins[8]; static int n_ins;
typedef std::_Rb_tree<long,long,std::_Identity<long>,std::less<long>,std::allocator<long> > TCrimeTree;
template<> std::pair<TCrimeTree::iterator,bool> TCrimeTree::insert_unique(const long& v)
{ g_ins[n_ins++]=v; return std::pair<TCrimeTree::iterator,bool>(TCrimeTree::iterator(this->_M_header._M_data),true); }

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

// ---- doubles ----
struct SPair { long key; void* value; };
static SPair g_pairs[32]; static void* g_map_owner[32]; static int g_npairs;
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
    for (int i=0;i<g_npairs;++i) if (g_map_owner[i]==(void*)this && g_pairs[i].key==(long)key) return (iterator)&g_pairs[i];
    return end();
}
template<> bool std::bitset<274>::test(size_t n) const { return (((const unsigned long*)this)[n>>5] >> (n&31)) & 1; }
static unsigned char g_mgc[0x40], g_world[0x40], g_myplayer[0x80], g_mainplayer[0x80], g_pm[0x20];
static unsigned char g_player[0x200], g_maincharthing[0x200], g_villthing[0x40], g_village[0xe8], g_targetthing[0x10];
static unsigned char g_log[0x40], g_stats[0x200], g_sb_main[0x40], g_sb_player[0x40];
static unsigned char g_deeds[4][0x40]; static long g_deed_handles[4]; static int g_ndeeds;
static unsigned char g_def[0x80];
static bool g_has_player;
static int n_setvillage, n_setreported, n_peek; static void* g_setvillage_arg; static long g_setreported_arg; static long g_peek_type;
static long g_find_ret[16]; static long g_find_types[8]; static const void* g_find_targets[8]; static int n_find;
static long g_removed[8]; static int n_removed;
static void* g_sb_this[8]; static long g_sb_act[8]; static float g_sb_score[8]; static bool g_sb_flag[8]; static int n_sb;
CMainGameComponent* CMainGameComponent::PMainGameComponent;
struct SMgcAccess : CMainGameComponent { static void Set(CMainGameComponent* p) { PMainGameComponent = p; } };
const CPlayer* CWorld::PeekMyPlayer() const { check((const void*)this==g_world, __LINE__); return (const CPlayer*)g_myplayer; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { check((const void*)this==g_myplayer, __LINE__); return g_has_player ? (CThingPlayerCreature*)g_player : 0; }
void CTCHeroOpinionDeedLog::SetVillageForCrimes(CThing& village) { check((void*)this==g_log, __LINE__); check(n_setreported==n_setvillage, __LINE__); ++n_setvillage; g_setvillage_arg=&village; }
void CTCHeroOpinionDeedLog::SetCrimeDeedReportedToVillage(long h) { check((void*)this==g_log, __LINE__); ++n_setreported; g_setreported_arg=h; }
const COpinionDeed* CTCHeroOpinionDeedLog::GetDeedFromHandle(long h) const
{ check((const void*)this==g_log, __LINE__); for (int i=0;i<g_ndeeds;++i) if (g_deed_handles[i]==h) return (const COpinionDeed*)g_deeds[i]; return 0; }
CDefPointer<const COpinionDeedEffectsDef> CTCHeroOpinionDeedLog::PeekPDeedEffectsDef(EOpinionDeedType t) const
{
    check((const void*)this==g_log, __LINE__); ++n_peek; g_peek_type=t; *(long*)(g_def+4)+=1;
    CDefPointer<const COpinionDeedEffectsDef> r; *(const void**)&r=g_def; return r;
}
long CTCHeroOpinionDeedLog::FindCriminalDeed(EOpinionDeedType t, const CThing* target)
{ check((void*)this==g_log, __LINE__); g_find_types[n_find]=t; g_find_targets[n_find]=target; ++n_find; return g_find_ret[t]; }
CPlayerManager& CTCBase::GetPlayerManager() { check((void*)this==g_village, __LINE__); return *(CPlayerManager*)g_pm; }
CPlayer& CPlayerManager::GetMainPlayer() { check((void*)this==g_pm, __LINE__); return *(CPlayer*)g_mainplayer; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    if ((const void*)this==(const void*)(g_mainplayer+0x34)) return (CBaseObject*)g_maincharthing;
    for (int i=0;i<4;++i) if ((const void*)this==(const void*)(g_deeds[i]+0x18)) return (CBaseObject*)g_targetthing;
    check(false, __LINE__); return 0;
}
void CTCHeroOnlineScoreboard::UpdateScoreForActivity(EOnlineScoreboardActivity a, float s, bool f)
{ g_sb_this[n_sb]=this; g_sb_act[n_sb]=a; g_sb_score[n_sb]=s; g_sb_flag[n_sb]=f; ++n_sb; }
void CTCVillage::RemoveCrime(long h) { check((void*)this==g_village, __LINE__); g_removed[n_removed++]=h; }
static void set_bit(unsigned char* thing, long type) { ((unsigned long*)(thing+0x20))[type>>5] |= 1UL<<(type&31); }
static void add_tc(unsigned char* thing, long type, void* tc) { set_bit(thing,type); g_map_owner[g_npairs]=thing+0x44; g_pairs[g_npairs].key=type; g_pairs[g_npairs].value=tc; ++g_npairs; }
static std::_Rb_tree_node<long> g_hdr, g_node;
static void setup(bool player, bool stats, long fine, long murder_in_set)
{
    memset(g_player,0,sizeof g_player); memset(g_maincharthing,0,sizeof g_maincharthing); memset(g_village,0,sizeof g_village);
    memset(g_stats,0,sizeof g_stats); memset(g_def,0,sizeof g_def); memset(g_deeds,0,sizeof g_deeds);
    g_npairs=0; g_ndeeds=0; n_ins=0; n_setvillage=n_setreported=n_peek=n_find=n_removed=n_sb=0; g_peek_type=-1; g_setreported_arg=0; g_setvillage_arg=0;
    for (int i=0;i<16;++i) g_find_ret[i]=-1;
    *(void**)(g_mgc+0x24)=g_world; SMgcAccess::Set((CMainGameComponent*)g_mgc);
    g_has_player=player;
    static SPair endpair[2]; endpair[0].value=endpair[1].value=0;
    *(void**)(g_player+0x44+4)=&endpair[0]; *(void**)(g_maincharthing+0x44+4)=&endpair[1];
    add_tc(g_player,TCI_HERO_OPINION_DEED_LOG,g_log);
    if (stats) add_tc(g_player,TCI_HERO_STATS,g_stats);
    add_tc(g_player,TCI_HERO_ONLINE_SCOREBOARD,g_sb_player);
    add_tc(g_maincharthing,TCI_HERO_ONLINE_SCOREBOARD,g_sb_main);
    *(void**)(g_village+4)=g_villthing;
    // PlayersCrimes: a real STLport header with one node (key murder_in_set) or none
    g_hdr._M_left=g_hdr._M_right=&g_hdr; g_hdr._M_parent=0;
    if (murder_in_set!=-1) { g_node._M_parent=&g_hdr; g_node._M_left=g_node._M_right=0; g_node._M_value_field=murder_in_set; g_hdr._M_parent=&g_node; g_hdr._M_left=g_hdr._M_right=&g_node; }
    *(void**)(g_village+0x64)=&g_hdr;
    *(long*)(g_def+4)=1; *(long*)(g_def+0x74)=fine;
}
static void add_deed(long handle, long type) { g_deed_handles[g_ndeeds]=handle; *(long*)g_deeds[g_ndeeds]=type; ++g_ndeeds; }
static CTCVillage* V() { return (CTCVillage*)g_village; }
static long& NC() { return *(long*)(g_stats+0xf4); }
static long& NM() { return *(long*)(g_stats+0xf8); }
static long& TF() { return *(long*)(g_stats+0xfc); }
int main()
{
    // no player: only the final insert, kept handle
    setup(false,true,5,-1); V()->ReportCrime(42);
    check(n_setvillage==0 && n_peek==0 && n_find==0, __LINE__); check(n_ins==1 && g_ins[0]==42, __LINE__);
    // no player, handle -1: nothing inserted
    setup(false,true,5,-1); V()->ReportCrime(-1); check(n_ins==0, __LINE__);
    // player, no deed: the log is told, handle inserted
    setup(true,true,5,-1); V()->ReportCrime(7);
    check(n_setvillage==1 && g_setvillage_arg==g_villthing, __LINE__); check(n_setreported==1 && g_setreported_arg==7, __LINE__);
    check(n_peek==0 && n_find==0 && n_sb==0, __LINE__); check(n_ins==1 && g_ins[0]==7, __LINE__);
    // fine -1, theft (6): no stats, no lookups
    setup(true,true,-1,-1); add_deed(7,6); NC()=3; TF()=100; V()->ReportCrime(7);
    check(n_peek==1 && g_peek_type==6, __LINE__); check(NC()==3 && TF()==100 && n_sb==0 && n_find==0, __LINE__);
    check(*(long*)(g_def+4)==1, __LINE__); check(n_ins==1 && g_ins[0]==7 && V()->IsEnemyBecauseOfCrime==false, __LINE__);
    // fine 0 (not -1) but no hero stats: nothing counted
    setup(true,false,0,-1); add_deed(7,6); V()->ReportCrime(7); check(n_sb==0 && n_ins==1, __LINE__);
    // fine 0 with stats: counted, fine unchanged
    setup(true,true,0,-1); add_deed(7,6); NC()=3; TF()=100; V()->ReportCrime(7);
    check(NC()==4 && TF()==100 && n_sb==2, __LINE__);
    // fine 50 with stats: count and fine, two scoreboard updates on the main player's character
    setup(true,true,50,-1); add_deed(7,6); NC()=3; TF()=100; NM()=9; V()->ReportCrime(7);
    check(NC()==4 && TF()==150 && NM()==9, __LINE__);
    check(n_sb==2 && g_sb_this[0]==g_sb_main && g_sb_act[0]==0x23 && g_sb_score[0]==150.0f && g_sb_flag[0]==false, __LINE__);
    check(g_sb_this[1]==g_sb_main && g_sb_act[1]==0x22 && g_sb_score[1]==4.0f && g_sb_flag[1]==false, __LINE__);
    check(n_find==0 && n_ins==1 && g_ins[0]==7, __LINE__);
    // murder, fine -1: assault found (11), GBH not; murder counted, scoreboard on the player
    setup(true,true,-1,-1); add_deed(8,9); NM()=2; NC()=5; g_find_ret[7]=11; V()->ReportCrime(8);
    check(V()->IsEnemyBecauseOfCrime==true, __LINE__);
    check(n_find==2 && g_find_types[0]==7 && g_find_types[1]==8 && g_find_targets[0]==g_targetthing && g_find_targets[1]==g_targetthing, __LINE__);
    check(n_removed==1 && g_removed[0]==11, __LINE__); check(NM()==3 && NC()==5, __LINE__);
    check(n_sb==1 && g_sb_this[0]==g_sb_player && g_sb_act[0]==0 && g_sb_score[0]==3.0f && g_sb_flag[0]==false, __LINE__);
    check(n_ins==1 && g_ins[0]==8, __LINE__);
    // murder: GBH found (12) only
    setup(true,true,-1,-1); add_deed(8,9); NM()=0; g_find_ret[8]=12; V()->ReportCrime(8);
    check(n_removed==1 && g_removed[0]==12 && NM()==1 && g_sb_score[0]==1.0f, __LINE__);
    // murder with fine 50: both stat blocks run
    setup(true,true,50,-1); add_deed(8,9); NM()=0; NC()=0; TF()=0; V()->ReportCrime(8);
    check(n_removed==0 && NC()==1 && TF()==50 && NM()==1 && n_sb==3 && g_sb_act[2]==0 && g_sb_this[2]==g_sb_player, __LINE__);
    // assault (7), no murder known: kept
    setup(true,true,-1,-1); add_deed(30,7); V()->ReportCrime(30);
    check(n_find==1 && g_find_types[0]==9 && g_find_targets[0]==g_targetthing && n_removed==0 && V()->IsEnemyBecauseOfCrime==false, __LINE__);
    check(n_ins==1 && g_ins[0]==30, __LINE__);
    // GBH (8), murder 200 already reported: folded into it (removed, not inserted), no recursive report
    setup(true,true,-1,200); add_deed(31,8); add_deed(200,9); g_find_ret[9]=200; V()->ReportCrime(31);
    check(n_find==1 && n_removed==1 && g_removed[0]==31 && n_ins==0 && n_setreported==1, __LINE__);
    // GBH (8), murder 200 reported in the set under another key: not found -> recursive report of the murder
    setup(true,true,-1,150); add_deed(31,8); add_deed(200,9); g_find_ret[9]=200; g_find_ret[7]=-1; g_find_ret[8]=-1; V()->ReportCrime(31);
    check(n_setreported==2 && g_setreported_arg==200, __LINE__); check(V()->IsEnemyBecauseOfCrime==true, __LINE__);
    check(n_removed==1 && g_removed[0]==31, __LINE__); check(n_ins==1 && g_ins[0]==200, __LINE__);
    // assault (7), murder 250 with an empty set: recursive report
    setup(true,true,-1,-1); add_deed(32,7); add_deed(250,9); g_find_ret[9]=250; V()->ReportCrime(32);
    check(n_setreported==2 && n_removed==1 && g_removed[0]==32 && n_ins==1 && g_ins[0]==250, __LINE__);
    // other deed types (6 and 10): no murder lookup
    setup(true,true,-1,-1); add_deed(33,10); g_find_ret[9]=250; V()->ReportCrime(33); check(n_find==0 && n_ins==1 && g_ins[0]==33, __LINE__);
    setup(true,true,-1,-1); add_deed(33,6); g_find_ret[9]=250; V()->ReportCrime(33); check(n_find==0 && n_ins==1, __LINE__);
    // audit e11: the -1 sentinels are equality tests (retail `cmp ...,-1 ; je/jne`): other negative values are not sentinels
    // fine -2 (not -1) with stats: counted, the fine added
    setup(true,true,-2,-1); add_deed(7,6); NC()=3; TF()=100; V()->ReportCrime(7); check(NC()==4 && TF()==98 && n_sb==2, __LINE__);
    // murder: assault -5 and GBH -7 found (not -1): both removed
    setup(true,true,-1,-1); add_deed(8,9); g_find_ret[7]=-5; g_find_ret[8]=-7; V()->ReportCrime(8);
    check(n_removed==2 && g_removed[0]==-5 && g_removed[1]==-7, __LINE__);
    // GBH, murder handle -3 (not -1) already in the set: folded into it, nothing inserted
    setup(true,true,-1,-3); add_deed(31,8); g_find_ret[9]=-3; V()->ReportCrime(31);
    check(n_removed==1 && g_removed[0]==31 && n_ins==0 && n_setreported==1, __LINE__);
    // no player, handle -4 (not -1): inserted
    setup(false,true,5,-1); V()->ReportCrime(-4); check(n_ins==1 && g_ins[0]==-4, __LINE__);
    if (g_bad==0) printf("0065cb6b_TEST PASS\n");
    return g_bad;
}
