// Behaviour test for 0x008DD7E0 NCreatureMode::CModeThunderLightningStorm::UpdateDamage(): the row's defines, includes and body verbatim
// over the shared headers (+ the hand4d overlay header engine/CThunderBattleDef.h), private members opened for the call. Zeroed object
// images: the mode (ParentThing +0x04, PLightningStormDef +0xA0, HitThings +0xA8, Timer +0xB8, StormRadius +0xC0), the caster and five
// creatures (component bits +0x20, component map +0x44, base def +0x70, dead bit +0x91, a fake vtable whose OnHit slot records the hit).
// The caster's base def answers GetPSubDefInfo("CThunderBattleDef") through its vtable and the definition manager double hands back
// the thunder battle def (LightningDamage 7.5, ref count checked after). Covers: the rough-area box (truncated centre +- truncated
// radius) and the IsNot / In2DRange filter contents; null, dead and already-hit creatures skipped; the electrocuted effect added and
// Reset only when missing; the hit (no location, caster, no weapon, creature position, creature - caster direction, knock-down only),
// lightning damage, no move-back; the hit-by-special-ability event (lightning spell) only while the creature is alive after the hit
// and only with an entity-events TC; every hit creature appended to HitThings; the def reference released.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define private public
#define protected public
// ---- row body ----
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThunderBattleDef.h"
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CHitParameters.h"
#include "engine/CTCElectrocutedEffect.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"

void NCreatureMode::CModeThunderLightningStorm::UpdateDamage()
{
    CDefPointer<const CThunderBattleDef> p_thunder_battle_def;
    GetParentThing().PeekBaseDef().GetSubDef(&p_thunder_battle_def);
    float lifetime_in_seconds = (float)Timer / CMainGameComponent::GetConstantFPS();
    float lifetime_of_main_beams = lifetime_in_seconds - PLightningStormDef->MainBeamsStartTime;
    std::vector<CThingCreatureBase*> hit_creatures;
    GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(
        C2DBoxI(GetParentThing().GetPos().ToC2DCoordI(), (long)StormRadius),
        GFFilter_Bind2(CThingFilter_IsNot(GetParentThing()), CThingFilter_In2DRange(GetParentThing().GetPos(), StormRadius)),
        hit_creatures);
    std::vector<CThingCreatureBase*>::iterator it;
    if (false) {} else for (it = hit_creatures.begin(); it != hit_creatures.end(); ++it) {
        CThingCreatureBase* pcreature = *it;
        if (IS_THING_ALIVE(pcreature)) {
            if (std::find(HitThings.begin(), HitThings.end(), CIntelligentPointer<CThing>(pcreature)) != HitThings.end()) {
                CTCElectrocutedEffect* ptc_elec;
                if (!pcreature->HasTC(TCI_ELECTROCUTED_EFFECT)) {
                    ptc_elec = GFDebugPDynamicCast(pcreature->AddTC("CTCElectrocutedEffect", true, NULL), (CTCElectrocutedEffect*)0);
                    ptc_elec->Reset();
                }
                const C3DVector& caster_pos = GetParentThing().GetPos();
                const C3DVector& creature_pos = pcreature->GetPos();
                CHitParameters hpParams(NULL, &GetParentThing(), NULL, pcreature->GetPos(), C3DVector(creature_pos.X - caster_pos.X, creature_pos.Y - caster_pos.Y, creature_pos.Z - caster_pos.Z), true, false, false, false);
                hpParams.AddDamageWithAttribute(DAMAGE_LIGHTNING, p_thunder_battle_def->LightningDamage);
                hpParams.SetMoveBackOnHit(false);
                pcreature->OnHit(hpParams);
                CTCEntityEvents* ptc_entity_events;
                if (IS_THING_ALIVE(pcreature) && pcreature->GetTC(&ptc_entity_events))
                    ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hpParams, HERO_ABILITY_LIGHTNING_SPELL));
                HitThings.push_back(CIntelligentPointer<CThing>(pcreature));
            }
        }
    }
}

// ---- doubles ----
long CMainGameComponent::ConstantFPS = 30;
static const char* g_strText[16]; static int g_strCtors = 0, g_strDtors = 0;
CCharString::CCharString(const char* text, long) { *(const char**)this = text; ++g_strCtors; }
CCharString::~CCharString() { ++g_strDtors; }
CCharString __fastcall CThunderBattleDef::StatGetTypeName() { return CCharString("CThunderBattleDef"); }
static int g_ipLive = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { ++g_ipLive; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_ipLive; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = (CBaseObjectPointer*)item; }
namespace std {
template<class It,class T> It FABLE_FASTCALL __find(It first,It last,const T& value,const random_access_iterator_tag&)
{
    for (; first != last; ++first) if (first->GetPItem() == value.GetPItem()) return first;
    return last;
}
}
enum { NT = 6 };                                           // 0 = caster, 1..5 = creatures
static unsigned char things[NT][0x100];
static C3DVector g_pos[NT];
static int TI(const void* p) { return (int)(((const unsigned char*)p - (const unsigned char*)things) / 0x100); }
const C3DVector& CThing::GetPos() const { return g_pos[TI(this)]; }
static unsigned char g_effect[0x40]; static int g_addTC = 0, g_resets = 0; static const void* g_addTCOn = 0; static const char* g_addTCName = 0;
static bool g_addTCActivate = false; static const void* g_addTCParams = (void*)1;
CTCBase* CThing::AddTC(const CCharString& name, bool activate, const CTCParamsBase* params)
{
    ++g_addTC; g_addTCOn = this; g_addTCName = *(const char* const*)&name; g_addTCActivate = activate; g_addTCParams = params;
    return (CTCBase*)g_effect;
}
void CTCElectrocutedEffect::Reset() { if ((void*)this == g_effect) ++g_resets; else g_resets += 100; }
struct SHit { const void* location; const void* hitter; const void* weapon; C3DVector pos, dir; bool knock, block, flourish, counter; };
static SHit g_hit; static int g_hitCtors = 0, g_hitDtors = 0, g_damageCalls = 0, g_damageAttr = -1; static float g_damage = 0.0f;
CHitParameters::CHitParameters(NHitLocations::CHitLocation* location, CThing* hitter, CThing* weapon, const C3DVector& position,
                               const C3DVector& direction, bool knock_down, bool blockable, bool flourish, bool block_counter)
{
    ++g_hitCtors;
    g_hit.location = location; g_hit.hitter = hitter; g_hit.weapon = weapon; g_hit.pos = position; g_hit.dir = direction;
    g_hit.knock = knock_down; g_hit.block = blockable; g_hit.flourish = flourish; g_hit.counter = block_counter;
    SetMoveBackOnHit(true);
}
CHitParameters::~CHitParameters() { ++g_hitDtors; }
void CHitParameters::AddDamageWithAttribute(EDamageAttribute attribute, float damage) { ++g_damageCalls; g_damageAttr = attribute; g_damage = damage; }
static int g_onHit[8]; static int g_onHits = 0; static int g_moveBackAtHit = 0, g_damageAtHit = 0, g_badHit = 0;
static void __fastcall OnHit(void* self, void*, CHitParameters* params)
{
    int i = TI(self);
    if (g_onHits < 8) g_onHit[g_onHits] = i;
    ++g_onHits;
    if (params->IsMoveBackOnHit()) g_moveBackAtHit = 1;
    if (g_damageCalls != g_onHits || g_damageAttr != 2 || g_damage != 7.5f) g_damageAtHit = 1;
    C3DVector d = g_pos[i] - g_pos[0];
    if (g_hit.location || g_hit.hitter != things[0] || g_hit.weapon || g_hit.pos.X != g_pos[i].X || g_hit.pos.Y != g_pos[i].Y
        || g_hit.pos.Z != g_pos[i].Z || g_hit.dir.X != d.X || g_hit.dir.Y != d.Y || g_hit.dir.Z != d.Z || !g_hit.knock || g_hit.block
        || g_hit.flourish || g_hit.counter) g_badHit = 1;
    if (i == 5) things[5][0x91] |= 1;                     // creature 5 dies of the hit
}
static void __fastcall Trap(void*, void*) { printf("FAIL unexpected virtual\n"); exit(1); }
static void* g_thingVt[128];
static int g_events = 0, g_eventAbility = -1; static const void* g_eventOn = 0; static const void* g_eventParams = 0; static const void* g_event = 0;
NEntityEvents::CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters& params, EHeroAbility ability)
{
    g_eventParams = &params; g_eventAbility = ability;
}
NEntityEvents::CEventHitBase::~CEventHitBase() {}
NEntityEvents::CEventBase::~CEventBase() {}
void CTCEntityEvents::AddEvent(NEntityEvents::CEventBase* event) { ++g_events; g_eventOn = this; g_event = event; }
static unsigned char g_entityEvents[3][0x40];
static unsigned char g_world[0x40], g_tools[0x40];
CWorld& CTCCreatureModeManager::CModeBase::GetWorld() const { return *(CWorld*)g_world; }
static int g_searches = 0; static C2DBoxI g_area; static const void* g_isNotThing = 0; static float g_rangePos[3], g_range = 0.0f; static const void* g_searchOn = 0;
static CThingCreatureBase* g_found[8]; static int g_nFound = 0;
template<class T,class C,class F> void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const F& filter, C& result) const
{
    ++g_searches; g_area = area; g_searchOn = this;
    const void* const* refs = (const void* const*)&filter;  // CFilter_Bind2: the IsNot then the In2DRange filter
    g_isNotThing = *(const void* const*)refs[0];
    const float* range = (const float*)refs[1];
    g_rangePos[0] = range[0]; g_rangePos[1] = range[1]; g_rangePos[2] = range[2]; g_range = range[3];
    for (int i = 0; i < g_nFound; ++i) result.push_back(g_found[i]);
}
static unsigned char g_def[0x4c], g_baseDef[0x40], g_defMgr[0x10], g_stormDef[0x80];
static void* g_defVt[32]; static void* g_baseVt[32];
static int g_destroyed = 0; static long g_askedIndex = -1; static const char* g_askedName = 0;
static void __fastcall Destroy(void*, void*) { ++g_destroyed; }
static CDefClassBase::CSubDefInfo g_subInfo;
static const CDefClassBase::CSubDefInfo* __fastcall SubDefInfo(void*, void*, const CCharString* name)
{
    g_askedName = *(const char* const*)name;
    return strcmp(g_askedName, "CThunderBattleDef") == 0 ? &g_subInfo : 0;
}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    g_askedIndex = index;
    CDefPointer<CDefClassBase> result;
    *(void**)&result = g_def; ++*(long*)(g_def + 4);
    return result;
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SPair { long type; void* tc; };
static SPair comps[NT][2];
int main()
{
    for (int k = 0; k < 128; ++k) g_thingVt[k] = (void*)Trap;
    g_thingVt[0x104 / 4] = (void*)OnHit;
    for (int k = 0; k < 32; ++k) { g_defVt[k] = (void*)Trap; g_baseVt[k] = (void*)Trap; }
    g_defVt[1] = (void*)Destroy; g_baseVt[1] = (void*)Destroy; g_baseVt[14] = (void*)SubDefInfo;
    *(void**)g_def = g_defVt; *(long*)(g_def + 4) = 100; *(float*)(g_def + 0x44) = 7.5f;
    *(void**)g_baseDef = g_baseVt; *(long*)(g_baseDef + 4) = 100; *(void**)(g_baseDef + 0x1c) = g_defMgr;
    g_subInfo.DefIndex = 42;
    for (int i = 0; i < NT; ++i) {
        *(void**)things[i] = g_thingVt;
        comps[i][0].type = 0x42; comps[i][0].tc = g_entityEvents[i % 3];
        comps[i][1].type = 0x8a; comps[i][1].tc = g_effect;
        *(void**)(things[i] + 0x44) = comps[i]; *(void**)(things[i] + 0x48) = comps[i] + 2; *(void**)(things[i] + 0x4c) = comps[i] + 2;
        g_pos[i] = C3DVector(1.0f + i, 2.0f * i, 0.5f * i);
    }
    *(void**)(things[0] + 0x70) = g_baseDef;
    g_pos[0] = C3DVector(10.7f, 20.2f, 1.0f);
    things[1][0x91] = 1;                                        // creature 1: dead
    *(unsigned long*)(things[2] + 0x28) |= 1UL << 2;             // creature 2: entity events, effect missing -> added
    *(unsigned long*)(things[3] + 0x28) |= 1UL << 2;             // creature 3: already hit
    *(unsigned long*)(things[4] + 0x30) |= 1UL << 10;            // creature 4: has the effect, no entity events
    *(unsigned long*)(things[5] + 0x28) |= 1UL << 2;             // creature 5: entity events, dies of the hit
    *(unsigned long*)(things[5] + 0x30) |= 1UL << 10;
    g_found[0] = (CThingCreatureBase*)things[1]; g_found[1] = 0; g_found[2] = (CThingCreatureBase*)things[2];
    g_found[3] = (CThingCreatureBase*)things[3]; g_found[4] = (CThingCreatureBase*)things[4]; g_found[5] = (CThingCreatureBase*)things[5];
    g_nFound = 6;
    *(void**)(g_world + 0x20) = g_tools;
    static unsigned char modeImage[0xc8];
    *(void**)(modeImage + 0x04) = things[0];
    NCreatureMode::CModeThunderLightningStorm* mode = (NCreatureMode::CModeThunderLightningStorm*)modeImage;
    *(void**)&mode->PLightningStormDef = g_stormDef;
    mode->Timer = 90;
    mode->StormRadius = 3.9f;
    check(&mode->HitThings == (void*)(modeImage + 0xa8) && &mode->StormRadius == (void*)(modeImage + 0xc0), "layout");
    mode->HitThings.push_back(CIntelligentPointer<CThing>((CThing*)things[3]));
    mode->UpdateDamage();
    check(g_askedName && strcmp(g_askedName, "CThunderBattleDef") == 0 && g_askedIndex == 42, "thunder battle sub-def looked up");
    check(*(long*)(g_def + 4) == 100 && !g_destroyed, "def reference released, not destroyed");
    check(g_searches == 1 && g_searchOn == g_tools, "one rough-area search on the world's search tools");
    check(g_area.TLX == 7 && g_area.TLY == 17 && g_area.BRX == 13 && g_area.BRY == 23, "area = truncated centre +- truncated radius");
    check(g_isNotThing == things[0], "IsNot filter holds the caster");
    check(g_rangePos[0] == 10.7f && g_rangePos[1] == 20.2f && g_rangePos[2] == 1.0f && g_range == 3.9f, "In2DRange filter: caster pos, storm radius");
    check(g_onHits == 3 && g_onHit[0] == 2 && g_onHit[1] == 4 && g_onHit[2] == 5, "live, not yet hit creatures hit in order");
    check(!g_badHit, "hit parameters: caster, creature pos, creature - caster direction, knock-down only");
    check(!g_moveBackAtHit && !g_damageAtHit && g_damageCalls == 3, "lightning damage from the def, no move-back, before OnHit");
    check(g_hitCtors == 3 && g_hitDtors == 3, "hit parameters destroyed");
    check(g_addTC == 1 && g_addTCOn == things[2] && g_addTCName && strcmp(g_addTCName, "CTCElectrocutedEffect") == 0
          && g_addTCActivate && g_addTCParams == 0 && g_resets == 1, "electrocuted effect added and reset only when missing");
    check(g_events == 1 && g_eventOn == g_entityEvents[2] && g_eventAbility == 11 && g_event != 0, "hit-by-special-ability event only for the surviving creature with entity events");
    int n = (int)(mode->HitThings.end() - mode->HitThings.begin());
    check(n == 4 && mode->HitThings[0].GetPItem() == (void*)things[3] && mode->HitThings[1].GetPItem() == (void*)things[2]
          && mode->HitThings[2].GetPItem() == (void*)things[4] && mode->HitThings[3].GetPItem() == (void*)things[5], "hit creatures remembered");
    check(g_ipLive == 4, "temporary intelligent pointers destroyed");
    if (!g_fail) printf("008dd7e0_TEST PASS\n");
    return g_fail;
}
