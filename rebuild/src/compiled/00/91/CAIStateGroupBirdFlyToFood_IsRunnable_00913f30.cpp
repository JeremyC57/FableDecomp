// CAIStateGroup_BirdFlyToFood::IsRunnable() -- runnable for a flying bird that has not eaten for 20 seconds, on a 74% roll: fly to the nearest dead creature (64% roll) or else the nearest BIRD_FOOD thing, if a flight position near it is found
// Leaf: ?IsRunnable@CAIStateGroup_BirdFlyToFood@@UAE_NXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Model: the landed sibling CAIStateGroup_BirdGroundWander::IsRunnable 0x009143E0 (same TU: interface 7 lookup, mode getter,
//   frame-rate local) and GetRandomFlightPos 0x00912700; layout: Ego_r.pdb FoodPos +0x24 (C3DVector), FrameLastFoundFood +0x30.
// Shape: the two GetPBestThing<Filter, Compare, T> instances are compiled in this TU (CThingSearchTools.h BESTTHING_BODY, noinline):
//   their visible bodies show that neither the out pointer nor the comparator temporary escapes, so VC7.1 stores the out
//   pointer's zero after loading the creature and loads the whole position before the stores (as retail).
// NF141 (2026-11-20, q4994 / q5140): ported onto the shared headers. PeekThingSearchTools 0x008FD000 now binds the PDB
//   ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ (engine/CAIStateGroupBase.h _PEEK), was the pointer stand-in. The
//   two searches 0x009142B0 / 0x00914330 are the real same-TU noinline GetPBestThing body of engine/CThingSearchTools.h (BESTTHING_BODY;
//   retail compiled both in this TU, right after this function) under their PDB template spellings
//   ??$GetPBestThing@VCThingFilter_None@@VCThingCompare_Nearest@@VCThingDeadCreature@@@CThingSearchTools@@QBE... (Ego_r.pdb 0x00960DD0 +
//   FableWin.pdb 0x02B8CA30) and ??$GetPBestThing@VCThingFilter_HasScriptName@@VCThingCompare_Nearest@@VCThingMarker@@@CThingSearchTools@@QBE...
//   (Ego_r.pdb 0x00960E50 + FableWin.pdb 0x02B8CBC0; filter 0x006A7E30 = the CThingFilter_DefName stand-in), were QAE stand-ins over made-up
//   FindBest_* helpers. Local CMainGameComponent / CWorld / CThingPhysical / CThingFilter_None / CThingCompare_Nearest / CThingSearchTools /
//   CAIBrain / CAIStateGroupBase / CAIStateGroup_FlyingThingBase models dropped for their shared headers; the row's own class (no header)
//   now derives from engine/CAIStateGroup_FlyingThingBase.h, members in place with their Ego_r.pdb names (FoodPos +0x24,
//   FrameLastFoundFood +0x30).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGDEADCREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGMARKER_STATTYPEFLAGS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GETFRAMESPERSECOND
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CThingMarker.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CAIStateGroup_FlyingThingBase.h"
extern const unsigned long NAVIGATOR_TYPE_A_STAR;   // 0x0129CBA4 (NF95 q11442: FableWin public ?NAVIGATOR_TYPE_A_STAR@@3KB, value 3; was g_flyingMode)

long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

struct CAIStateGroup_BirdFlyToFood : CAIStateGroup_FlyingThingBase {   // no shared header: the row's own class (Ego_r.pdb layout 0x34)
    C3DVector FoodPos;                          // +0x24
    unsigned long FrameLastFoundFood;           // +0x30
    virtual bool IsRunnable();                  // NF59 (q10505): Ego_r.pdb 0x00960BC0 + FableWin.pdb 0x02B8C310 ?IsRunnable@CAIStateGroup_BirdFlyToFood@@UAE_NXZ
};

bool CAIStateGroup_BirdFlyToFood::IsRunnable()
{
    CTCCreatureNavigation* navigation;
    GetCreature().GetTC(TCI_NAVIGATION, &navigation);
    if (navigation->GetNavigatorType() == NAVIGATOR_TYPE_A_STAR) {
        long framesPerSecond;
        if ((FrameLastFoundFood == 0
             || (framesPerSecond = PeekWorld().GetFramesPerSecond(),
                 CWorld::GetFrame() - FrameLastFoundFood > (unsigned long)(framesPerSecond * 20)))
            && WorldRandomRange(*GetPWorldSeed(), 100) < 74) {
            if (WorldRandomRange(*GetPWorldSeed(), 100) < 64) {
                CThingAICreature& creature = GetCreature();
                CThingDeadCreature* corpse = 0;
                if (PeekThingSearchTools().GetPBestThing(CThingFilter_None(), CThingCompare_Nearest(creature.PeekPos()), &corpse)
                    && GetRandomFlightPos(corpse->PeekPos(), 0.5f, FoodPos, true))
                    return true;
            }
            CThingMarker* food = 0;
            bool found;
            {
                CCharString name("BIRD_FOOD");
                found = PeekThingSearchTools().GetPBestThing(CThingFilter_HasScriptName(name), CThingCompare_Nearest(GetCreature().PeekPos()), &food);
            }
            if (found && GetRandomFlightPos(food->PeekPos(), 1.5f, FoodPos, true))
                return true;
        }
    }
    return false;
}
