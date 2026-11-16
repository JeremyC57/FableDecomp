// CThingFilter_IsOpinionSourceWithFreshDeed::operator()(const CThing& thing) const -- an opinion source passes when it carries a virtual deed that is not the one last seen from it, and the creature is aware of it (always-aware sources, else the nonstop awareness or the sight test)
// Leaf: ??RCThingFilter_IsOpinionSourceWithFreshDeed@@QBE_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00960FC0, length 188 bytes, `ret 4`, bool in al.
// Identity: Ego_r.pdb 0x00985680 + FableWin.pdb 0x02C29470 ??RCThingFilter_IsOpinionSourceWithFreshDeed@@QBE_NABVCThing@@@Z (const CThing&);
//   engine/CThingFilter_IsOpinionSourceWithFreshDeed.h declares it; the IsRunnable donor 0x009609B0 calls it as filter(*thing).
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x50 = CTCOpinionSource, CThing::PeekTC expanded), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50, NCreatureAI::IsAwareOfThingIndirectly @ 0x00864700 and NCreatureAI::IsAwareOfThing @ 0x008645E0 (both `__fastcall`, `ret 4`).
// Layout: engine/CThingFilter_IsOpinionSourceWithFreshDeed.h (PCreature +0, LastVDeedID +8, LastSourceUID +0x10), engine/CTCOpinionSource.h
//   (UseNavLineOfSight +0x18, AlwaysAwareOf +0x75, ReactNonstopFlag +0x77, VirtualDeed +0x80, VirtualDeedID +0x84), CThing UniqueID +0x18.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCOPINIONSOURCE
#define FABLE_RETAIL_CTCOPINIONSOURCE_AWARENESS_PEEKS
#define FABLE_RETAIL_NCREATUREAI_AWARE_INDIRECTLY
#define FABLE_RETAIL_CTHINGFILTER_ISOPINIONSOURCEWITHFRESHDEED
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCOpinionSource.h"
#include "engine/NCreatureAI.h"
#include "engine/CThingFilter_IsOpinionSourceWithFreshDeed.h"

bool CThingFilter_IsOpinionSourceWithFreshDeed::operator()(const CThing& thing) const
{
    const CTCOpinionSource* source;
    if (thing.PeekTC(&source)) {
        if (source->PeekVirtualDeed()) {
            if (!(LastVDeedID == source->PeekVirtualDeedID() && LastSourceUID == thing.GetUniqueID())) {
                if (source->IsAlwaysAwareOf())
                    return true;
                bool aware;
                if (source->IsReactNonstop())
                    aware = NCreatureAI::IsAwareOfThingIndirectly(PCreature.Get(), &thing, source->PeekUseNavLineOfSight());
                else
                    aware = NCreatureAI::IsAwareOfThing(PCreature.Get(), &thing, source->PeekUseNavLineOfSight());
                if (aware)
                    return true;
            }
        }
    }
    return false;
}
