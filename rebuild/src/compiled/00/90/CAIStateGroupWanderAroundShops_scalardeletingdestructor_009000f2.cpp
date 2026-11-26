// CAIStateGroup_WanderAroundShops::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit ~CAIStateGroup_WanderAroundShops, then
//   operator delete when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_WanderAroundShops_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF471 (T3 nf303 0x008FD288, callee-only respell, ruling r470a): the call at +3 (retail 0x009000F5 -> 0x0090010E) now names the real implicit
//   destructor ??1CAIStateGroup_WanderAroundShops@@UAE@XZ on the shared retail class (engine/CAIStateGroup_WanderAroundShops.h, the r470e retail opt-in FABLE_RETAIL_CAISTATEGROUP_WANDERAROUNDSHOPS on CAIStateGroup_WorkBase<CAIStateGroupBase> (retail RTTI 0x0128BE4C); Ego_r / FableWin type records flag
//   ~CAIStateGroup_WanderAroundShops compiler-generated), not the free stand-in CAIStateGroup_WanderAroundShops_Destruct. VC7.1 emits that destructor here as a
//   pick-any COMDAT byte- and relocation-equal to retail 0x0090010E (PrettyThing +0x1c, Shop +0x14, then ~CAIStateGroupBase 0x008FD288).
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUP_WANDERAROUNDSHOPS

class CThing; class CWorld; class CThingAICreature; class CThingObject; class CThingCreatureBase; class CCreatureActionBase;
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_WanderAroundShops.h"
void* __fastcall CAIStateGroup_WanderAroundShops_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    static_cast<CAIStateGroup_WanderAroundShops*>(stateGroup)->CAIStateGroup_WanderAroundShops::~CAIStateGroup_WanderAroundShops();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
