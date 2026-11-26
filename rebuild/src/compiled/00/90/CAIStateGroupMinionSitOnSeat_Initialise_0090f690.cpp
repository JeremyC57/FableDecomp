// CAIStateGroup_MinionSitOnSeat::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// Leaf: ?Initialise@CAIStateGroup_MinionSitOnSeat@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// -- the base initialisation, then a random sit-down frame within the next 80 seconds.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F690, length 125 bytes (+ int3 padding), `ret 14h`. Vtable slot 1 of
// CAIStateGroup_MinionSitOnSeat (0x0128CFBC) and of CAIStateGroup_MinionSitOutside (0x0128D00C), its only retail references.
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (the five arguments forwarded from the frame), CAIStateGroupBase::
// GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890 (__fastcall), CWorld::GetFrame @ 0x0049D870 (__fastcall).
// Global: the frame-rate long @ 0x01375550, read AFTER the base call (`fps*80` = `lea esi,[esi+esi*4] ; shl esi,4`).
// Shape: the landed random-frames step (seed*0x24A1+0x24DF, GFROR13, `range ? seed % range : 0` with an unsigned
// `div`; 0x00934090) then `FrameToSitDown = GFGetCurrentFrame() + r` (+0x3c, engine header name).
// Identity (NF476, T3 nf374 binder): Ego_r.pdb / FableWin.pdb ?Initialise@CAIStateGroup_MinionSitOnSeat@@UAE...JW4EScriptAIPriority@@...@Z
//   (was ...JJ...: the HEAD row's local `long` priority). The class is now the header's polymorphic retail view
//   (engine/CAIStateGroup_MinionSitOnSeat.h FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEAT_VIRTUAL_MODEL, the defines of the landed sibling
//   0x009021DA), so the base call binds the landed self ?Initialise@CAIStateGroupBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
//   (was the made-up ?Initialise@CAIStateGroupBaseI@@QAE..JJ..@Z). The local CAIStateGroupBase / CAIStateGroupBaseI / CAIBrain /
//   CBaseIntelligentPointer / CWorld / CMainGameComponent models are gone: engine/CWorld.h, engine/CMainGameComponent.h, engine/GFRandom.h.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_USESSEAT_VIRTUALS_PASS27
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEATBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEAT_VIRTUAL_MODEL
#include "engine/CAIStateGroup_MinionSitOnSeat.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"

void CAIStateGroup_MinionSitOnSeat::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    long framesPerSecond = CMainGameComponent::GetConstantFPS();
    unsigned long* seed = GetPWorldSeed();
    unsigned long range = framesPerSecond * 80;
    unsigned long next = *seed * 0x24A1U + 0x24DFU;
    *seed = next;
    unsigned long rotated = GFROR13(next);
    *seed = rotated;
    unsigned long r;
    if (range != 0)
        r = rotated % range;
    else
        r = 0;
    FrameToSitDown = CWorld::GetFrame() + r;
}
