// CAIStateGroup_HoverWanderAroundThing::IsRunnable() -- runnable while the thing being wandered around is alive; otherwise gather the
//   objects and the dead creatures in the 20x20 box round the creature and latch a random dead creature (else a random object) as
//   the new thing; false when there is neither
// Leaf: ?IsRunnable@CAIStateGroup_HoverWanderAroundThing@@UAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00959800, 595 bytes, thiscall `ret`; vtable 0x0128D1BC slot 11. Ego_r.pdb 0x0097DF80 / FableWin.pdb
//   0x017E945B ?IsRunnable@CAIStateGroup_HoverWanderAroundThing@@UAE_NXZ; member PThing (CIntelligentPointer<const CThingGameObject>) +0x1c.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (the
//   CIntelligentPointer<const CThingGameObject> temporary, vtable 0x0125C29C), CThing::GetPos 0x004C73D0, _ftol 0x00BFEA70,
//   CAIStateGroupBase::GetWorld 0x008FCFC0, CThingSearchTools::GetAllThingsInRoughArea<const CThingObject, vector<const CThingObject*>,
//   CThingFilter_None> 0x00959380 and <const CThingDeadCreature, vector<const CThingDeadCreature*>, CThingFilter_None> 0x009595C0,
//   CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890, GFRandom 0x004CDAE0, free 0x00BFEA14.
// Shape: the two rough-area instances are compiled straight before this function (0x00959380 / 0x009595C0, Ego_r keeps the same
//   order), so their bodies are visible (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY, noinline): the dead-creature vector's start
//   stays in edi across the later calls and the pointer temporary reuses its frame words. GFRandom (?GFRandom@@YIKKPAK@Z, the
//   landed 0x004CDAE0 body, shared header engine/GFRandom.h) is visible as a plain inline: VC7.1 expands the first call site and calls the out-of-line copy at
//   the second, exactly as retail does (inline at 0x00959922, `call 0x004CDAE0` at 0x009599CC).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_HOVERWANDERAROUNDTHING
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_C2DBOXI
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingObject.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_None.h"
#include "engine/CWorld.h"
#include "stlport/fable_containers.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/CAIStateGroup_HoverWanderAroundThing.h"


bool CAIStateGroup_HoverWanderAroundThing::IsRunnable()
{
    if (IS_THING_ALIVE(PThing))
        return true;
    std::vector<const CThingObject*> objects;
    std::vector<const CThingDeadCreature*> deadCreatures;
    {
        const C3DVector& pos = GetCreature().GetPos();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI box(x - 10, y - 10, x + 10, y + 10);
        GetWorld().GetThingSearchTools().GetAllThingsInRoughArea<const CThingObject>(box, CThingFilter_None(), objects);
    }
    {
        const C3DVector& pos = GetCreature().GetPos();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI box(x - 10, y - 10, x + 10, y + 10);
        GetWorld().GetThingSearchTools().GetAllThingsInRoughArea<const CThingDeadCreature>(box, CThingFilter_None(), deadCreatures);
    }
    if (!deadCreatures.empty()) {
        PThing = CIntelligentPointer<const CThingGameObject>(deadCreatures[GFRandom(deadCreatures.size(), GetWorld().GetWorldSeed())]);
        return true;
    }
    if (!objects.empty()) {
        PThing = CIntelligentPointer<const CThingGameObject>(objects[GFRandom(objects.size(), GetWorld().GetWorldSeed())]);
        return true;
    }
    return false;
}