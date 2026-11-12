// CTCVillage::UpdateMirroredCrimeData() -- refresh the hero's mirrored crime record for the current region from this village's crimes
// Leaf: ?UpdateMirroredCrimeData@CTCVillage@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065C5BB, 421 bytes, thiscall `ret`, `leave`: frame-pointer size TU (the CTCVillage TU:
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)`, as the landed RemoveCrimeIfApologisable 0x0065C1FC).
// Identity: Ego_r.pdb ?UpdateMirroredCrimeData@CTCVillage@@QAEXXZ (the catalog name).
// What it does: the main player's controlled creature, when it has its CTCHeroStats, gets the OutstandingCrimes record of the
//   current region (map<CCharString, CTCHeroStats::MirroredCriminalStatus>) copied out, stamped with this village's
//   FramePlayerLastSeenByGuard, its ten crime counts cleared, then one count per crime deed in PlayersCrimes (a deed type below 10;
//   a deed type whose effects def is OnlyGetFinedForOneInstance counts once), and the record written back.
// Callees: CWorld::PeekMyPlayer() const 0x0049D9D0, CPlayer::GetPControlledCreature() const 0x00487DC0, std::bitset<274>::test
//   0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the two expanded GetTC<T>), CTCBase::PeekWorldMap()
//   const 0x00686D20 (twice), CWorldMap::GetLoadedRegion() const 0x004FB150, CWorldMap::GetRegion(long) const 0x004FC180,
//   CCharString copy ctor 0x0099EC30 / dtor 0x0099EAE0, map<CCharString,MirroredCriminalStatus>::operator[] 0x005812DF (twice),
//   CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0, CTCHeroOpinionDeedLog::PeekPDeedEffectsDef 0x006C5630, the set iterator's
//   _Rb_global<bool>::_M_increment 0x004292C0; the def's slot 1 on the last release.
// Shape: the effects-def temporary lives in the `||`'s first operand (retail's `or [ebp-4],1` destruction flag); the deed log's
//   unchecked out-parameter GetTC; the record is a 0x44-byte local copied in and out (`rep movsd`, 0x11 dwords).
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
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CDefPointer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCVillage.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

void CTCVillage::UpdateMirroredCrimeData()
{
    CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
    if (!hero)
        return;
    CTCHeroStats* hero_stats;
    if (!hero->GetTC(&hero_stats))
        return;
    CCharString region_name = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetName();
    CTCHeroStats::MirroredCriminalStatus status = hero_stats->OutstandingCrimes[region_name];
    status.LastTimeSeenByGuard = FramePlayerLastSeenByGuard;
    for (long n = 0; n < 10; ++n)
        status.Crimes[n] = 0;
    CTCHeroOpinionDeedLog* deed_log;
    hero->GetTC(&deed_log);
    std::set<long>::iterator end = PlayersCrimes.end();
    for (std::set<long>::iterator it = PlayersCrimes.begin(); it != end; ++it) {
        const COpinionDeed* deed = deed_log->GetDeedFromHandle(*it);
        if (deed && deed->DeedType < 10) {
            if (!deed_log->PeekPDeedEffectsDef(deed->DeedType)->OnlyGetFinedForOneInstance || status.Crimes[deed->DeedType] == 0)
                ++status.Crimes[deed->DeedType];
        }
    }
    hero_stats->OutstandingCrimes[region_name] = status;
}