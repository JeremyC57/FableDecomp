// CThingPhysical::ConstructFromParams(long defIndex, const C3DVector& pos, long player, unsigned long a4, CThingPhysical* originalCreator, CThingPhysical* lastCreator, bool addToMap) -- construct the CThing on the map under the position, then reset scale, health (from the def), creators and the hit flags
// Leaf: ?ConstructFromParams@CThingPhysical@@IAE_NJABVC3DVector@@JKPAV1@1_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A5950, length 289 bytes, `ret 1Ch`. Identity: Ego_r.pdb
//   ?ConstructFromParams@CThingPhysical@@IAE_NJABVC3DVector@@JKPAV1@1_N@Z (protected; the position by const reference).
// Callees: the world map's GetMapNumberAt(const C3DVector&) (virtual slot 16, +0x40) on CThing::GetPCurrentThingManager() @ 0x013B8A1C
// (WorldMap +0x28), CThing::ConstructFromParams(long, long, long, bool) @ 0x004CA010 (protected), CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (the temporaries' vtable 0x01244DCC is
// CIntelligentPointer<CThingPhysical>).
// Caller: CThingGameObject::ConstructFromParams 0x008388D0 (landed; NAME_FIXES for its binding).
// Shape: `Health = MaxHealth = def->Health` (MaxHealth stored first, through the FPU); each creator is assigned through
// a CIntelligentPointer<CThingPhysical>(T*) temporary (inline ctor: base ctor, vtable, SetPItem) and the inline
// `operator=` (SetPItem(other.GetPItem())), destroyed by the base dtor; `if (base) {...; return true;} return false;`
// keeps the false exit last.
// NF62 (2026-11-17, round 7): ported onto the shared CThing / CThingPhysical / CThingBaseDef / CThingManager / CWorldMap /
//   CIntelligentPointer headers (the local CWorldMap / CThingManager / CThingBaseDef / CBaseIntelligentPointer / CIntelligentPointer /
//   CThing / CThingPhysical models are gone); 0x004CA010 now binds ?ConstructFromParams@CThing@@IAE_NJJJ_N@Z. Overlay opt-ins:
//   FABLE_RETAIL_CTHING_CONSTRUCTFROMPARAMS, FABLE_RETAIL_CTHINGPHYSICAL_CONSTRUCTFROMPARAMS, FABLE_RETAIL_CTHINGMANAGER_WORLDMAP (the
//   FableWin.pdb inline GetWorldMap accessor).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_CONSTRUCTFROMPARAMS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_CONSTRUCTFROMPARAMS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_WORLDMAP
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingManager.h"
#include "engine/CWorldMap.h"
#include "engine/C3DVector.h"

bool CThingPhysical::ConstructFromParams(long defIndex, const C3DVector& pos, long player, unsigned long a4,
                                         CThingPhysical* originalCreator, CThingPhysical* lastCreator, bool addToMap)
{
    long mapNumber = CThing::GetPCurrentThingManager()->GetWorldMap().GetMapNumberAt(pos);
    if (CThing::ConstructFromParams(defIndex, player, mapNumber, addToMap)) {
        Scale = 1.0f;
        Health = MaxHealth = PBaseDef->Health;
        CanComeBetweenCameraAndObservedThing = -1;
        PLastCreator = CIntelligentPointer<CThingPhysical>(lastCreator);
        POriginalCreator = CIntelligentPointer<CThingPhysical>(originalCreator);
        Drawable = true;
        EvadeAllHits = false;
        NegateAllHits = false;
        Killable = true;
        RespondToHits = true;
        Damageable = PBaseDef->Damageable;
        AbleToBeEngagedInCombat = true;
        return true;
    }
    return false;
}
