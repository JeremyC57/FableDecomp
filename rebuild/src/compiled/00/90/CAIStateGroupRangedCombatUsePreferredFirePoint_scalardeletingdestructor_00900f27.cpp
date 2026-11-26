// CAIStateGroup_RangedCombatUsePreferredFirePoint::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit ~CAIStateGroup_RangedCombatUsePreferredFirePoint, then
//   operator delete when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_RangedCombatUsePreferredFirePoint_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF471 (T3 nf303 0x008FD288, callee-only respell, ruling r470a): the call at +3 (retail 0x00900F2A -> 0x00900F43) now names the real implicit
//   destructor ??1CAIStateGroup_RangedCombatUsePreferredFirePoint@@UAE@XZ on the shared retail class (engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h, its add-only members view
//   FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT_MEMBERS_ONLY: the full view's STLport include would add /DEFAULTLIB:"libcp"; Ego_r / FableWin type records flag
//   ~CAIStateGroup_RangedCombatUsePreferredFirePoint compiler-generated), not the free stand-in CAIStateGroup_RangedCombatUsePreferredFirePoint_Destruct. VC7.1 emits that destructor here as a
//   pick-any COMDAT byte- and relocation-equal to retail 0x00900F43 (PFirePoint +0x1c, PEnemy +0x14, then ~CAIStateGroupBase 0x008FD288).
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT_MEMBERS_ONLY

class CThing; class CWorld; class CThingAICreature; class CThingObject; class CThingCreatureBase; class CCreatureActionBase;
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h"
void* __fastcall CAIStateGroup_RangedCombatUsePreferredFirePoint_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    static_cast<CAIStateGroup_RangedCombatUsePreferredFirePoint*>(stateGroup)->CAIStateGroup_RangedCombatUsePreferredFirePoint::~CAIStateGroup_RangedCombatUsePreferredFirePoint();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
