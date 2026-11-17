// Behaviour test for 0x007993F0 CTCShopKeeper::WitnessedCrime(const COpinionDeed&, long): the row's defines, includes and body verbatim over
// the shared headers, run on zeroed object images (no engine constructor runs). Doubles: CVectorMap::LowerBound (table lookup by key),
// GetPVillage, GetShopTC, GetPItem (work building), GetPos, IsPointInside, PeekThingSearchTools, the GetPFirstThing<CThingFilter_HasTC,CThingAICreature> template (out of line in retail at 0x00799C40) run through the inline filter,
// SetAlarm, StopCurrentSpeech, PlayAISpeech, CCharString, the village calls, KnowsOfDeed,
// InformOfDeed, GetPlayerManager / GetMainPlayer / GetPControlledCreature, PeekPDeedEffectsDef. Cases cover every early return, the
// 10.0 alarm distance exactly on the threshold, the stock state, shop v stall, the talk TC, the reported / known-deed branches and the
// severity 2 / 3 / other security sweep.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_GFGETWORLD2DDISTANCE_X_FIRST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_WITNESSEDCRIME
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_STALL_STOCK_ACCESSORS
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSHOPKEEPER_WITNESSEDCRIME
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM_HANDLE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCTalk.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingSearchTools.h"
#include "engine/ThingAlive.h"
#include "engine/GFWorldDistance.h"
#include <math.h>
namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_FOREGROUND = 0, DIALOGUE_LAYER_MIDGROUND = 1, DIALOGUE_LAYER_BACKGROUND = 2, DIALOGUE_LAYER_LAST = 3 }; }

inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

void CTCShopKeeper::WitnessedCrime(const COpinionDeed& deed, long deed_handle)
{
    CTCVillageMember* ptc_village_member;
    if (GetParentThing().GetTC(TCI_VILLAGE_MEMBER, &ptc_village_member))
    {
        CTCVillage* ptc_village = ptc_village_member->GetPVillage();
        if (ptc_village)
        {
            CTCShop* tc_shop = GetShopTC();
            if (tc_shop == 0)
                return;
            if (GetParentThing().GetType() != 1)
                return;
            CThingAICreature* creature = static_cast<CThingAICreature*>(&GetParentThing());
            if (!IS_THING_ALIVE(creature->GetWorkBuilding()))
                return;
            if (tc_shop->IsStallLike())
            {
                const float MAX_DISTANCE_FOR_ALARM = 10.0f;
                if (GFGetWorld2DDistance(creature->GetWorkBuilding()->GetPos(), creature->GetPos()) > MAX_DISTANCE_FOR_ALARM)
                    return;
                if (tc_shop->GetStockState() == CTCShop::STOCKED_NO)
                    return;
            }
            else if (!creature->GetWorkBuilding()->IsPointInside(creature->GetPos()))
                return;
            CThingAICreature* guard;
            if (!PeekThingSearchTools().GetPFirstThing(CThingFilter_HasTC(TCI_GUARD), &guard))
                return;
            if (&GetParentThing() == &tc_shop->GetParentThing())
                return;
            if (!tc_shop->IsStallLike())
                tc_shop->SetAlarm(true);
            else
            {
                CTCTalk* tc_talk;
                if (GetParentThing().GetTC(TCI_TALK, &tc_talk))
                {
                    tc_talk->StopCurrentSpeech();
                    tc_talk->PlayAISpeech("TEXT_AI_CRIME_RUN_TO_GUARDS", false, NSpeechGainManager::DIALOGUE_LAYER_MIDGROUND, false, true);
                }
            }
            if (!ptc_village->HasCrimeBeenReported(deed_handle))
            {
                ptc_village->ReportCrime(deed_handle);
                CTCCreatureOpinionOfHero* ptc_village_opinion;
                if (ptc_village->GetParentThing().GetTC(TCI_CREATURE_OPINION_OF_HERO, &ptc_village_opinion))
                {
                    if (!ptc_village_opinion->KnowsOfDeed(deed_handle, deed.DeedType, true))
                        ptc_village_opinion->InformOfDeed(deed_handle, false);
                }
            }
            CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroOpinionDeedLog* ptc_deed_log;
            player->GetTC(TCI_HERO_OPINION_DEED_LOG, &ptc_deed_log);
            CDefPointer<const COpinionDeedEffectsDef> pdeed_effects_def = ptc_deed_log->PeekPDeedEffectsDef(deed.DeedType);
            if (pdeed_effects_def->CrimeSeverity == 2 || pdeed_effects_def->CrimeSeverity == 3)
                ptc_village->StartSecuritySweep(true);
        }
    }
}

// ---- doubles ----
static int g_bad=0;
static void check(bool ok, int line) { if (!ok) { printf("check failed line %d\n", line); ++g_bad; } }
struct SPair { long key; void* value; };
static SPair g_pairs[16]; static int g_npairs=0;
static void* g_map_owner[16];
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
    for (int i=0;i<g_npairs;++i) if (g_map_owner[i]==(void*)this && g_pairs[i].key==(long)key) return (iterator)&g_pairs[i];
    return end();
}
static unsigned char g_keeper[0x200], g_shopthing[0x200], g_villthing[0x200], g_player[0x200], g_building[0x200], g_guard[0x200];
static unsigned char g_vm[0x40], g_village[0x40], g_shop[0x60], g_talk[0x60], g_opinion[0x60], g_log[0x40], g_keeptc[0x40];
static C3DVector g_keeperpos, g_buildpos;
static bool g_building_set, g_inside, g_guard_found, g_reported, g_knows;
static long g_severity;
static int n_shoptc, n_alarm, n_stop, n_speech, n_report, n_inform, n_sweep, n_knows; static bool g_alarm_arg, g_sweep_arg;
static long g_knows_handle, g_knows_type, g_inform_handle, g_report_handle, g_reported_handle; static bool g_inform_flag, g_knows_flag;
static char g_speech_text[64]; static bool g_sp_a,g_sp_b,g_sp_c; static long g_sp_layer;
static long g_peek_type;
CTCVillage* CTCVillageMember::GetPVillage() const { return (this==(const CTCVillageMember*)g_vm && g_village[0x3f]==0) ? (CTCVillage*)g_village : 0; }
static bool g_has_shop;
CTCShop* CTCShopKeeper::GetShopTC() const { ++n_shoptc; return g_has_shop ? (CTCShop*)g_shop : 0; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { if ((const void*)this==(const void*)(g_keeper+0x170)) return g_building_set ? (CBaseObject*)g_building : 0; return 0; }
const C3DVector& CThing::GetPos() const { return (const void*)this==(const void*)g_building ? g_buildpos : g_keeperpos; }
bool CThingBuilding::IsPointInside(const C3DVector& p) const { check((const void*)this==g_building, __LINE__); check(&p==&g_keeperpos, __LINE__); return g_inside; }
static CThingSearchTools* g_tools=(CThingSearchTools*)0x1000;
const CThingSearchTools& CTCBase::PeekThingSearchTools() const { check((const void*)this==g_keeptc, __LINE__); return *g_tools; }
template<class Filter,class T> bool CThingSearchTools::GetPFirstThing(const Filter& filter,T** out) const
{ check(this==g_tools, __LINE__); if (!g_guard_found || !filter(*(const CThing*)g_guard)) return false; *out=(T*)g_guard; return true; }
void CTCShop::SetAlarm(bool on) { ++n_alarm; g_alarm_arg=on; }
void CTCTalk::StopCurrentSpeech() { check((void*)this==g_talk, __LINE__); check(n_speech==0, __LINE__); ++n_stop; }
CTCTalk::ESpeechPlaybackResult CTCTalk::PlayAISpeech(const CCharString& text, bool a, NSpeechGainManager::EDialogueLayer layer, bool b, bool c)
{ check((void*)this==g_talk, __LINE__); check(n_stop==1, __LINE__); ++n_speech; strcpy(g_speech_text, *(const char* const*)&text); g_sp_a=a; g_sp_layer=layer; g_sp_b=b; g_sp_c=c; return (ESpeechPlaybackResult)0; }
CCharString::CCharString(const char* text, long) { *(const char**)this=text; }
CCharString::~CCharString() { }
bool CTCVillage::HasCrimeBeenReported(long h) { check((void*)this==g_village, __LINE__); g_reported_handle=h; return g_reported; }
void CTCVillage::ReportCrime(long h) { check((void*)this==g_village, __LINE__); ++n_report; g_report_handle=h; }
void CTCVillage::StartSecuritySweep(bool urgent) { check((void*)this==g_village, __LINE__); ++n_sweep; g_sweep_arg=urgent; }
bool CTCCreatureOpinionOfHero::KnowsOfDeed(long h, EOpinionDeedType t, bool f) const { check((const void*)this==g_opinion, __LINE__); ++n_knows; g_knows_handle=h; g_knows_type=t; g_knows_flag=f; return g_knows; }
void CTCCreatureOpinionOfHero::InformOfDeed(long h, bool f) { check((void*)this==g_opinion, __LINE__); ++n_inform; g_inform_handle=h; g_inform_flag=f; }
static unsigned char g_pm[8], g_pl[8];
CPlayerManager& CTCBase::GetPlayerManager() { check((void*)this==g_keeptc, __LINE__); return *(CPlayerManager*)g_pm; }
CPlayer& CPlayerManager::GetMainPlayer() { check((void*)this==g_pm, __LINE__); return *(CPlayer*)g_pl; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { check((const void*)this==g_pl, __LINE__); return (CThingPlayerCreature*)g_player; }
struct SFakeDefVtbl { void* slot[4]; };
static void* g_def_vtbl[4];
static unsigned char g_def[0x80];
CDefPointer<const COpinionDeedEffectsDef> CTCHeroOpinionDeedLog::PeekPDeedEffectsDef(EOpinionDeedType t) const
{
    check((const void*)this==g_log, __LINE__); g_peek_type=t;
    *(long*)(g_def+4)=2; *(long*)(g_def+0x70)=g_severity; *(void***)g_def=g_def_vtbl;
    CDefPointer<const COpinionDeedEffectsDef> r; *(const void**)&r=g_def; return r;
}

static void set_bit(unsigned char* thing, long type) { ((unsigned long*)(thing+0x20))[type>>5] |= 1UL<<(type&31); }
static void add_tc(unsigned char* thing, long type, void* tc) { set_bit(thing,type); g_map_owner[g_npairs]=thing+0x44; g_pairs[g_npairs].key=type; g_pairs[g_npairs].value=tc; ++g_npairs; }
struct SCase { bool member, village, shop; unsigned char type; bool building, dead, stall; float dist; long stock; bool inside, guard, same, talk, reported, opinion, knows; long severity; };
static void setup(const SCase& c)
{
    memset(g_keeper,0,sizeof g_keeper); memset(g_shopthing,0,sizeof g_shopthing); memset(g_villthing,0,sizeof g_villthing); memset(g_player,0,sizeof g_player);
    memset(g_building,0,sizeof g_building); memset(g_village,0,sizeof g_village); memset(g_shop,0,sizeof g_shop); memset(g_keeptc,0,sizeof g_keeptc);
    g_npairs=0; n_shoptc=n_alarm=n_stop=n_speech=n_report=n_inform=n_sweep=n_knows=0; g_peek_type=-1; g_speech_text[0]=0; g_reported_handle=-1;
    *(void**)(g_keeptc+4)=g_keeper;
    // the keeper's end sentinel: CVectorMap end() (+4) points at an empty pair
    static SPair endpair[4];
    *(void**)(g_keeper+0x44+4)=&endpair[0]; *(void**)(g_villthing+0x44+4)=&endpair[1]; *(void**)(g_player+0x44+4)=&endpair[2]; *(void**)(g_shopthing+0x44+4)=&endpair[3];
    endpair[0].value=endpair[1].value=endpair[2].value=endpair[3].value=0;
    if (c.member) add_tc(g_keeper,TCI_VILLAGE_MEMBER,g_vm);
    g_village[0x3f]=c.village?0:1;
    g_has_shop=c.shop;
    g_keeper[0x10]=c.type;
    g_building_set=c.building; g_building[0x91]=c.dead?1:0;
    g_shop[0x50]=c.stall; *(long*)(g_shop+0x0c)=c.stock; *(void**)(g_shop+4)=c.same?(void*)g_keeper:(void*)g_shopthing;
    g_keeperpos=C3DVector(10.0f,20.0f,5.0f); g_buildpos=C3DVector(10.0f+c.dist*0.6f,20.0f+c.dist*0.8f,99.0f);
    g_inside=c.inside; g_guard_found=c.guard;
    if (c.talk) add_tc(g_keeper,TCI_TALK,g_talk);
    g_reported=c.reported; *(void**)(g_village+4)=g_villthing;
    if (c.opinion) add_tc(g_villthing,TCI_CREATURE_OPINION_OF_HERO,g_opinion);
    g_knows=c.knows; g_severity=c.severity;
    add_tc(g_player,TCI_HERO_OPINION_DEED_LOG,g_log);
}
static void run(const SCase& c) { setup(c); unsigned char deed[0x40]; memset(deed,0,sizeof deed); *(long*)deed=17; ((CTCShopKeeper*)g_keeptc)->WitnessedCrime(*(const COpinionDeed*)deed, 1234); }

int main()
{
    memset(g_guard,0,sizeof g_guard); set_bit(g_guard,TCI_GUARD);
    const SCase base={true,true,true,1,true,false,false,0.0f,0,true,true,false,true,false,true,false,1};
    SCase c;
    // full shop path: alarm, report, opinion informed, severity 1 -> no sweep
    c=base; run(c);
    check(n_alarm==1 && g_alarm_arg, __LINE__); check(n_stop==0 && n_speech==0, __LINE__); check(g_reported_handle==1234, __LINE__); check(n_report==1 && g_report_handle==1234, __LINE__);
    check(n_knows==1 && g_knows_handle==1234 && g_knows_type==17 && g_knows_flag, __LINE__); check(n_inform==1 && g_inform_handle==1234 && !g_inform_flag, __LINE__);
    check(g_peek_type==17, __LINE__); check(n_sweep==0, __LINE__); check(*(long*)(g_def+4)==1, __LINE__);
    // severity 2 and 3 sweep, 4 does not
    c=base; c.severity=2; run(c); check(n_sweep==1 && g_sweep_arg, __LINE__);
    c=base; c.severity=3; run(c); check(n_sweep==1 && g_sweep_arg, __LINE__);
    c=base; c.severity=4; run(c); check(n_sweep==0, __LINE__);
    // early returns
    c=base; c.member=false; run(c); check(n_shoptc==0 && n_report==0 && g_peek_type==-1, __LINE__);
    c=base; c.village=false; run(c); check(n_shoptc==0 && n_report==0, __LINE__);
    c=base; c.shop=false; run(c); check(n_shoptc==1 && n_alarm==0 && n_report==0, __LINE__);
    c=base; c.type=2; run(c); check(n_alarm==0 && n_report==0, __LINE__);
    c=base; c.building=false; run(c); check(n_alarm==0 && n_report==0, __LINE__);
    c=base; c.dead=true; run(c); check(n_alarm==0 && n_report==0, __LINE__);
    c=base; c.inside=false; run(c); check(n_alarm==0 && n_report==0, __LINE__);
    c=base; c.guard=false; run(c); check(n_alarm==0 && n_report==0, __LINE__);
    c=base; c.same=true; run(c); check(n_alarm==0 && n_report==0 && g_peek_type==-1, __LINE__);
    // stall: inside test skipped, distance threshold exactly 10 passes, just over fails, stock state 1 fails, 2 passes
    c=base; c.stall=true; c.inside=false; c.dist=10.0f; run(c); check(n_alarm==0 && n_stop==1 && n_speech==1, __LINE__); check(strcmp(g_speech_text,"TEXT_AI_CRIME_RUN_TO_GUARDS")==0, __LINE__);
    check(!g_sp_a && g_sp_layer==1 && !g_sp_b && g_sp_c, __LINE__); check(n_report==1, __LINE__);
    c=base; c.stall=true; c.dist=10.5f; run(c); check(n_stop==0 && n_report==0, __LINE__);
    c=base; c.stall=true; c.dist=3.0f; c.stock=1; run(c); check(n_stop==0 && n_report==0, __LINE__);
    c=base; c.stall=true; c.dist=3.0f; c.stock=2; run(c); check(n_stop==1 && n_report==1, __LINE__);
    c=base; c.stall=true; c.dist=3.0f; c.talk=false; run(c); check(n_stop==0 && n_speech==0 && n_alarm==0 && n_report==1, __LINE__);
    // reported already: no report / inform, still the deed-effects check
    c=base; c.reported=true; c.severity=2; run(c); check(n_report==0 && n_knows==0 && n_inform==0 && n_sweep==1, __LINE__);
    c=base; c.opinion=false; run(c); check(n_report==1 && n_knows==0 && n_inform==0, __LINE__);
    c=base; c.knows=true; run(c); check(n_knows==1 && n_inform==0, __LINE__);
    if (g_bad==0) printf("007993f0_TEST PASS\n"); else printf("007993f0_TEST FAIL %d\n", g_bad);
    return 0;
}
