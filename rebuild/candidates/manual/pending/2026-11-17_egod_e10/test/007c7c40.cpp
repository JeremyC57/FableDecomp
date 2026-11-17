// Behaviour test for 0x007C7C40 CTCTurncoat::FrameUpdate(): the row's defines, includes and bodies verbatim (incl. the same-TU
// IsTurncoatable / IsTurncoated) over the shared headers, run on zeroed object images. Doubles: CThing::GetPos, CreateParticleEffect,
// CBaseIntelligentPointer (item kept in PData), CThing::Kill, CVectorMap::LowerBound (table lookup), PlayAttachedCriteria, AttachToThing,
// CCharString, the out-of-line vector push_back (placement copy into a static array; pop_back runs inline). Cases: not overridable / not
// turncoatable; turncoated with the timer above / reaching 0 (deactivate effect present / absent, turncoated effect alive / dead / null,
// sound TC present / absent); not turncoated (parent null / dead / turncoatedness just below the resistance); recovery on / off and the
// clamp at 0; the stage count growing (alive / dead / null new effects), shrinking (alive / dead old effects), unchanged, and clamped at 0 / 1.
#include <stdio.h>
#include <string.h>
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCTURNCOAT
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTurncoatDef.h"
#include "engine/CTCTurncoat.h"
#include "engine/ThingAlive.h"

void CTCTurncoat::FrameUpdate()
{
    if (!IsTurncoatable())
        return;
    CThingCreatureBase& parent = static_cast<CThingCreatureBase&>(GetParentThing());
    if (IS_THING_ALIVE(&parent) && IsTurncoated())
    {
        if (--WillTurncoatTimer > 0)
            return;
        CurrentRecoveryTimer = 0;
        WillTurncoatTimer = TurncoatTimeLast;
        if (PDef->TurncoatDeactivateEffect)
            CTCSpecialAbilities::CreateParticleEffect(PDef->TurncoatDeactivateEffect, parent.GetPos());
        CurrentTurncoatedness = 0.0f;
        if (IS_THING_ALIVE(TurncoatedEffect))
            TurncoatedEffect->Kill(true);
        CTCSoundPlayer* ptc_sound_player;
        if (GetParentThing().GetTC(TCI_SOUND_PLAYER, &ptc_sound_player))
            ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_TURNCOAT_RELEASE", true, false, false, -1);
    }
    else
    {
        if (CurrentRecoveryTimer == 0)
        {
            CurrentTurncoatedness -= ResistanceRecoveryPerFrame;
            if (CurrentTurncoatedness < 0.0f)
                CurrentTurncoatedness = 0.0f;
        }
        else
            --CurrentRecoveryTimer;
        long cur_stage = TurncoatStageEffects.size();
        long max_stage = PDef->TurncoatStageEffects.size();
        float stage_proportion = CurrentTurncoatedness / CreatureTurncoatResistance;
        float interp = stage_proportion / (1.0f - 1.0f / max_stage);
        if (interp < 0.0f)
            interp = 0.0f;
        else if (interp > 1.0f)
            interp = 1.0f;
        long new_cur_stage = (long)(interp * max_stage);
        if (new_cur_stage > cur_stage)
        {
            for (long i = cur_stage; i < new_cur_stage; ++i)
            {
                CThing* fx = CTCSpecialAbilities::CreateParticleEffect(PDef->TurncoatStageEffects[i], parent.GetPos());
                if (IS_THING_ALIVE(fx))
                {
                    CTCDParticleEmitter* ptc_emitter;
                    fx->GetTC(TCI_PARTICLE_EMITTER, &ptc_emitter);
                    ptc_emitter->AttachToThing(parent, 8, "", 0, 0.0f);
                }
                TurncoatStageEffects.push_back(CIntelligentPointer<CThing>(fx));
            }
        }
        else if (new_cur_stage < cur_stage)
        {
            long num_effects_to_kill = cur_stage - new_cur_stage;
            for (long i = 0; i < num_effects_to_kill; ++i)
            {
                CIntelligentPointer<CThing>& fx = TurncoatStageEffects.back();
                if (IS_THING_ALIVE(fx.Get()))
                    fx->Kill(true);
                TurncoatStageEffects.pop_back();
            }
        }
    }
}

bool CTCTurncoat::IsTurncoatable() const
{
    return TurncoatOverride && PDef->Turncoatable;
}

bool CTCTurncoat::IsTurncoated() const
{
    return IsTurncoatable() && CurrentTurncoatedness >= CreatureTurncoatResistance;
}

// ---- doubles ----
static int g_bad=0;
static void check(bool ok, int line) { if (!ok) { printf("check failed line %d\n", line); ++g_bad; } }
struct SPair { long key; void* value; };
static SPair g_pairs[16]; static int g_npairs=0; static void* g_map_owner[16];
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
    for (int i=0;i<g_npairs;++i) if (g_map_owner[i]==(void*)this && g_pairs[i].key==(long)key) return (iterator)&g_pairs[i];
    return end();
}
static void set_bit(unsigned char* thing, long type) { ((unsigned long*)(thing+0x20))[type>>5] |= 1UL<<(type&31); }
static SPair g_endpair;
static void add_tc(unsigned char* thing, long type, void* tc) { set_bit(thing,type); g_map_owner[g_npairs]=thing+0x44; g_pairs[g_npairs].key=type; g_pairs[g_npairs].value=tc; ++g_npairs; }
CCharString::CCharString(const char* text, long) { *(const char**)this=text; }
CCharString::~CCharString() { }
static unsigned char g_tc[0x4c], g_parent[0x100], g_def[0x60], g_effect[0x100], g_sound[0x40], g_emitter[0x40];
static unsigned char g_fx[8][0x100]; static int g_nfx; static bool g_fx_null, g_fx_dead;
static C3DVector g_parentpos(1,2,3);
static long g_stages[4]={11,12,13,14};
const C3DVector& CThing::GetPos() const { check((const void*)this==g_parent, __LINE__); return g_parentpos; }
static int n_create; static long g_create_ids[8];
CThing* FABLE_FASTCALL CTCSpecialAbilities::CreateParticleEffect(long id, const C3DVector& pos)
{
    check(&pos==&g_parentpos, __LINE__); g_create_ids[n_create++]=id;
    if (id<10) return 0;                     // the deactivate effect: result unused
    if (g_fx_null) return 0;
    unsigned char* fx=g_fx[g_nfx++]; memset(fx,0,0x100); *(void**)(fx+0x48)=&g_endpair; if (g_fx_dead) fx[0x91]=1;
    add_tc(fx,TCI_PARTICLE_EMITTER,g_emitter); return (CThing*)fx;
}
CBaseIntelligentPointer::CBaseIntelligentPointer() { PData=0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData=(CBaseObjectPointer*)item; }
static int n_kill; static void* g_killed[8]; static bool g_kill_arg;
void CThing::Kill(bool b) { g_killed[n_kill++]=this; g_kill_arg=b; }
static int n_play; static char g_play_text[64]; static bool g_pa,g_pb,g_pc; static long g_pd;
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& s, bool a, bool b, bool c, long d)
{ check((void*)this==g_sound, __LINE__); ++n_play; strcpy(g_play_text,*(const char* const*)&s); g_pa=a; g_pb=b; g_pc=c; g_pd=d; return 0; }
static int n_attach; static void* g_attach_this[8];
void CTCDParticleEmitter::AttachToThing(CThing& thing, long type, const CCharString& helper, long index, float offset)
{ check(&thing==(CThing*)g_parent && type==8 && strcmp(*(const char* const*)&helper,"")==0 && index==0 && offset==0.0f, __LINE__); g_attach_this[n_attach++]=this; }
static unsigned char g_slots[8][sizeof(CIntelligentPointer<CThing>)];
static int n_push;
template<> void std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > >::push_back(const CIntelligentPointer<CThing>& x)
{ ++n_push; new (this->_M_finish) CIntelligentPointer<CThing>(x); ++this->_M_finish; }

struct STcImage { long* timer() { return (long*)(g_tc+0x0c); } };
static long& L(int off) { return *(long*)(g_tc+off); }
static float& F(int off) { return *(float*)(g_tc+off); }
static CIntelligentPointer<CThing>* g_vec_start() { return (CIntelligentPointer<CThing>*)g_slots[0]; }
static void setup(bool override_, bool turncoatable, int nstages)
{
    memset(g_tc,0,sizeof g_tc); memset(g_parent,0,sizeof g_parent); memset(g_def,0,sizeof g_def); memset(g_effect,0,sizeof g_effect);
    g_npairs=0; n_create=n_kill=n_play=n_attach=n_push=0; g_nfx=0; g_fx_null=g_fx_dead=false; g_play_text[0]=0;
    *(void**)(g_tc+4)=g_parent; *(void**)(g_parent+0x48)=&g_endpair; *(void**)(g_tc+0x14)=g_def;
    g_tc[0x2c]=override_; g_def[0x25]=turncoatable; *(long*)(g_def+0x30)=5;
    *(long**)(g_def+0x40)=g_stages; *(long**)(g_def+0x44)=g_stages+4;
    L(0x0c)=1; L(0x20)=40; F(0x48)=3.0f; F(0x38)=0.5f;
    for (int i=0;i<nstages;++i) { unsigned char* fx=g_fx[4+i]; memset(fx,0,0x100); new (g_slots[i]) CIntelligentPointer<CThing>((CThing*)fx); }
    *(void**)(g_tc+0x3c)=g_slots[0]; *(void**)(g_tc+0x40)=g_slots[nstages]; *(void**)(g_tc+0x44)=g_slots[8];
}
static long nstage() { return (long)(((unsigned char*)*(void**)(g_tc+0x40)-(unsigned char*)*(void**)(g_tc+0x3c))/sizeof(CIntelligentPointer<CThing>)); }
static void run() { ((CTCTurncoat*)g_tc)->CTCTurncoat::FrameUpdate(); }

int main()
{
    // not overridable / not turncoatable: untouched
    setup(false,true,0); F(0x10)=3.0f; run(); check(L(0x0c)==1 && n_create==0 && F(0x10)==3.0f, __LINE__);
    setup(true,false,0); F(0x10)=3.0f; run(); check(L(0x0c)==1 && n_create==0 && F(0x10)==3.0f, __LINE__);
    // turncoated (turncoatedness exactly at the resistance), timer 2 -> 1: nothing else
    setup(true,true,0); F(0x10)=3.0f; L(0x0c)=2; L(0x34)=7; run(); check(L(0x0c)==1 && L(0x34)==7 && n_create==0 && F(0x10)==3.0f, __LINE__);
    // timer 1 -> 0: expiry with deactivate effect, live turncoated effect, sound TC
    setup(true,true,0); F(0x10)=3.0f; L(0x34)=7; ((CBaseIntelligentPointer*)(g_tc+0x18))->SetPItem((CBaseObject*)g_effect); add_tc(g_parent,TCI_SOUND_PLAYER,g_sound); run();
    check(L(0x0c)==40 && L(0x34)==0 && F(0x10)==0.0f, __LINE__); check(n_create==1 && g_create_ids[0]==5, __LINE__);
    check(n_kill==1 && g_killed[0]==g_effect && g_kill_arg, __LINE__);
    check(n_play==1 && strcmp(g_play_text,"HERO_ABILITY_TURNCOAT_RELEASE")==0 && g_pa && !g_pb && !g_pc && g_pd==-1, __LINE__);
    // expiry with no deactivate effect, dead turncoated effect, no sound TC
    setup(true,true,0); F(0x10)=3.5f; *(long*)(g_def+0x30)=0; g_effect[0x91]=1; ((CBaseIntelligentPointer*)(g_tc+0x18))->SetPItem((CBaseObject*)g_effect); run();
    check(L(0x0c)==40 && n_create==0 && n_kill==0 && n_play==0 && F(0x10)==0.0f, __LINE__);
    // null turncoated effect
    setup(true,true,0); F(0x10)=3.0f; run(); check(n_kill==0 && n_create==1, __LINE__);
    // not turncoated: just below the resistance -> recovery: 2.75 - 0.5 = 2.25; interp 0.75/0.75 = 1 -> 4 stages from 0
    setup(true,true,0); F(0x10)=2.75f; run();
    check(L(0x0c)==1 && F(0x10)==2.25f, __LINE__); check(n_create==4 && g_create_ids[0]==11 && g_create_ids[3]==14 && n_push==4 && nstage()==4, __LINE__);
    check(n_attach==4 && g_attach_this[0]==g_emitter, __LINE__);
    // audit fix: the pushed pointers are the new effects, in order (kills push_back(CIntelligentPointer<CThing>(0)))
    check(((CBaseIntelligentPointer*)g_slots[0])->GetPItem()==(CBaseObject*)g_fx[0] && ((CBaseIntelligentPointer*)g_slots[3])->GetPItem()==(CBaseObject*)g_fx[3], __LINE__);
    // parent null (4 stages: interp clamped to 1, nothing to grow) / dead with turncoatedness at the resistance -> not turncoated (recovery timer on: decremented, value kept)
    setup(true,true,4); F(0x10)=3.0f; L(0x34)=2; *(void**)(g_tc+4)=0; run();
    check(L(0x34)==1 && L(0x0c)==1 && F(0x10)==3.0f && n_create==0 && n_kill==0 && nstage()==4, __LINE__);
    setup(true,true,1); F(0x10)=3.0f; L(0x34)=2; g_fx_null=true; g_parent[0x91]=1; run();
    check(L(0x34)==1 && F(0x10)==3.0f && L(0x0c)==1, __LINE__); check(n_create==3 && n_push==3 && n_attach==0 && nstage()==4, __LINE__);
    // dead new effects: pushed, not attached; 1.125/3 = 0.375 / 0.75 = 0.5 -> 2 stages from 1
    setup(true,true,1); F(0x10)=1.125f; L(0x34)=1; g_fx_dead=true; run(); check(n_create==1 && g_create_ids[0]==12 && n_push==1 && n_attach==0 && nstage()==2, __LINE__);
    // unchanged count
    setup(true,true,2); F(0x10)=1.125f; L(0x34)=1; run(); check(n_create==0 && n_push==0 && n_kill==0 && nstage()==2, __LINE__);
    // shrink 3 -> 1 (0.5625/3 = 0.1875/0.75 = 0.25 -> 1): kill the live back effects, pop; slot 1 dead: not killed
    setup(true,true,3); F(0x10)=0.5625f; L(0x34)=1; g_fx[5][0x91]=1; run();
    check(nstage()==1 && n_kill==1 && g_killed[0]==g_fx[6] && g_kill_arg, __LINE__);
    // recovery clamp: 0.25 - 0.5 -> 0 -> stage 0, shrink 2 -> 0
    setup(true,true,2); F(0x10)=0.25f; run(); check(F(0x10)==0.0f && nstage()==0 && n_kill==2, __LINE__);
    // negative turncoatedness (recovery timer on) -> interp clamped to 0: nothing to kill from 0
    setup(true,true,0); F(0x10)=-1.0f; L(0x34)=3; run(); check(nstage()==0 && n_kill==0 && n_create==0 && L(0x34)==2, __LINE__);
    // audit fix: the same-TU IsTurncoated 0x007C7890 on its own (not only behind FrameUpdate's IsTurncoatable early return):
    // turncoatedness at the resistance but not overridable / not turncoatable -> false; overridable + turncoatable -> true, just below -> false
    setup(false,true,0); F(0x10)=3.0f; check(!((CTCTurncoat*)g_tc)->IsTurncoated(), __LINE__);
    setup(true,false,0); F(0x10)=3.0f; check(!((CTCTurncoat*)g_tc)->IsTurncoated(), __LINE__);
    setup(true,true,0); F(0x10)=3.0f; check(((CTCTurncoat*)g_tc)->IsTurncoated() && ((CTCTurncoat*)g_tc)->IsTurncoatable(), __LINE__);
    setup(true,true,0); F(0x10)=2.75f; check(!((CTCTurncoat*)g_tc)->IsTurncoated(), __LINE__);
    // audit fix: max_stage comes from the def's stage list (2 entries here): 1.2/3 = 0.4 / (1 - 1/2) = 0.8 * 2 -> 1 stage (a 4-entry count gives 2)
    setup(true,true,0); *(long**)(g_def+0x44)=g_stages+2; F(0x10)=1.2f; L(0x34)=1; run(); check(n_create==1 && g_create_ids[0]==11 && nstage()==1, __LINE__);
    if (g_bad==0) printf("007c7c40_TEST PASS\n"); else printf("007c7c40_TEST FAIL %d\n", g_bad);
    return 0;
}
