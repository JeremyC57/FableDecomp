// CTCCreatureOpinionOfHero::FrameUpdate() -- per frame: kill a stale or hero-perceived unknown-threat source, run the transient opinion offsets (run in with peak capping, hold, run out, drop), periodically drop known deeds the hero's deed log no longer holds, and decay the times-hit count
// Leaf: ?FrameUpdate@CTCCreatureOpinionOfHero@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006823D0, 962 bytes, thiscall `ret`. Identity: CTCCreatureOpinionOfHero vtable slot 9 (header), Ego_r.pdb /
//   FableWin.pdb ?FrameUpdate@CTCCreatureOpinionOfHero@@UAEXXZ.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_THREATSOURCE
#define FABLE_RETAIL_COPINIONSET_INDEX
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "stlport/fable_map.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/ThingAlive.h"
#include "engine/GFLimit.h"       // both PDBs ??$GFLimit@MMM@@YIXAAMABM1@Z: the clamp address is taken once per clamp (`add ecx,edi` 0x00682593, `lea ecx,[edx+ecx*4]` 0x006825F8)
extern const float DELTA_NOTIONAL_ZERO;                                 // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)

inline float __fastcall GFFloatAbs(float f)                             // FableWin ?GFFloatAbs@@YIMM@Z (inline: `and [m],7FFFFFFFh`)
{
    *(unsigned long*)&f &= 0x7fffffff;
    return f;
}

void CTCCreatureOpinionOfHero::FrameUpdate()
{
    if (IS_THING_ALIVE(PUnknownThreatOpinionSource)) {
        const CTCPerceiveThing* perceive;
        CThingPlayerCreature* hero;
        if ((float)(GetWorld().GetFrame() - FrameLastUpdatedUnknownThreat) > (float)CMainGameComponent::GetConstantFPS() * 30.0f)
            PUnknownThreatOpinionSource->Kill(true);
        else if (ParentThing->PeekTC(&perceive) && (hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature()) != 0 &&
                 perceive->GetPerceivedThing() == hero && perceive->IsAwareOfThing() && perceive->IsThingIgnoreFOVLineOfSight())
            PUnknownThreatOpinionSource->Kill(true);
    }

    std::list<COpinionTransientOffset>::iterator end = TransientOffsets.end();
    for (std::list<COpinionTransientOffset>::iterator it = TransientOffsets.begin(), next = it; it != end; it = next) {
        ++next;
        COpinionTransientOffset& offset = *it;
        if (offset.FramesToRunIn > 0) {
            RelativeOpinion[offset.Opinion] += offset.OffsetPerFrameRunIn;
            if (GFFloatAbs(RelativeOpinion[offset.Opinion]) - 2.0f > DELTA_NOTIONAL_ZERO) {
                float over = GFFloatAbs(RelativeOpinion[offset.Opinion]) - 2.0f;
                if (GFFloatAbs(offset.OffsetPerFrameRunOut) > DELTA_NOTIONAL_ZERO) {
                    long frames = (long)GFFloatAbs(over / offset.OffsetPerFrameRunOut);
                    if (frames > offset.FramesToRunOut)
                        frames = offset.FramesToRunOut;
                    offset.FramesToRunOut -= frames;
                    offset.FramesOfCappedPeak += frames;
                }
                GFLimit(RelativeOpinion[offset.Opinion], -2.0f, 2.0f);
            }
            --offset.FramesToRunIn;
        } else if (offset.FramesOfCappedPeak > 0) {
            --offset.FramesOfCappedPeak;
        } else if (offset.FramesToRunOut > 0) {
            RelativeOpinion[offset.Opinion] += offset.OffsetPerFrameRunOut;
            GFLimit(RelativeOpinion[offset.Opinion], -2.0f, 2.0f);
            --offset.FramesToRunOut;
        } else {
            next = TransientOffsets.erase(it);
        }
    }

    long fps = CMainGameComponent::GetConstantFPS();
    if (((unsigned long)CWorld::GetFrame() + ParentThing->GetUniqueID()) % (unsigned long)(fps * 20) == 0) {
        CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (hero) {
            CTCHeroOpinionDeedLog* deedLog;
            hero->GetTC(&deedLog);
            for (long i = 0; i < 92; ++i) {
                std::list<long>& deeds = KnownDeeds[i];
                std::list<long>::iterator deedsEnd = deeds.end();
                for (std::list<long>::iterator deed = deeds.begin(), nextDeed = deed; deed != deedsEnd; deed = nextDeed) {
                    ++nextDeed;
                    if (!deedLog->GetDeedFromHandle(*deed))
                        nextDeed = deeds.erase(deed);
                }
            }
        }
    }

    if (NumberOfTimesHit != 0.0f) {
        long frames = CWorld::GetFrame() - FrameToDecayNumberOfTimesHit;
        if (frames > 30) {
            NumberOfTimesHit -= (float)frames / CMainGameComponent::GetConstantFPS() * (1.0f / 60.0f);
            if (NumberOfTimesHit < 0.0f)
                NumberOfTimesHit = 0.0f;
            FrameToDecayNumberOfTimesHit = CWorld::GetFrame() + 1;
        }
    }
}
