// CAIStateGroup_TavernServer::GetBar() -- gather the live tavern bars (TCI_TAVERN_BAR) the pub owns into a local list, clear PBar,
//   then pick one at random (GFRandom over the list size with the world seed) and remember it in PBar; false when the pub has none
// Leaf: ?GetBar@CAIStateGroup_TavernServer@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00966AF0, 502 bytes; thiscall, returns bool.
// Identity: Ego_r.pdb ?GetBar@CAIStateGroup_TavernServer@@QAE_NXZ (0x0098B360, between WanderWithinTavern and IsCarryingJug as in
//   retail 0x00966AD0 / 0x00966CF0) and FableWin.pdb (same mangled name); called by IsRunnable 0x00967650.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ctor 0x00A01B10 / dtor 0x00A01C10 (temporary
//   CIntelligentPointer<CThingObject>, vtable 0x0125A858), CVectorMap::LowerBound 0x0040F020 (GetTC<CTCThingOwner>, interface 0x21),
//   malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes), CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890
//   (the inline GFRandom), CIntelligentPointer<CThingObject>::CIntelligentPointer(CThingObject*) 0x0065E133 and operator= 0x007B82C0
//   and std::list<CThingObject*>::~list 0x00792240 (out of line inside the loop).
// Shape: the pick sits in a `while (true)` whose body returns on both paths; VC7.1 inlines the list destructor at the first,
//   out-of-loop return only and calls the COMDAT (and the folded IP ctor/operator=) at every return inside the loop, which is what
//   retail does (inlined self-link + free at 0x00966BDE, call 0x00792240 at the three later exits).
// Header opt-in: FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_GETBAR (declares GetBar in engine/CAIStateGroup_TavernServer.h).
// Flags: none
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_METHODS
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_GETBAR
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroup_TavernServer.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CThingBuilding.h"
#include "engine/ThingAlive.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCTavernTable.h"
#include "engine/GFRandom.h"
#include "stlport/fable_containers.h"

bool CAIStateGroup_TavernServer::GetBar()
{
    CTCThingOwner* owner;
    PPub->GetTC(&owner);
    std::list<CThingObject*> bars;
    for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it)
        // Retail tests the alive condition twice: two GetPItem-nonnull + [+0x91]&1 pairs at 0x00966B60-0x00966B97 before HasTC.
        if (IS_THING_ALIVE(*it) && IS_THING_ALIVE(*it) && (*it)->HasTC(TCI_TAVERN_BAR))
            bars.push_back((CThingObject*)(*it).Get());
    if (bars.empty())
        return false;
    PBar = 0;
    while (true) {
        if (!bars.empty()) {
            long index = GFRandom(bars.size(), GetPWorldSeed());
            for (std::list<CThingObject*>::iterator bar_it = bars.begin(); bar_it != bars.end(); ++bar_it) {
                if (index == 0) {
                    PBar = *bar_it;
                    break;
                }
                --index;
            }
            return true;
        }
        return false;
    }
}