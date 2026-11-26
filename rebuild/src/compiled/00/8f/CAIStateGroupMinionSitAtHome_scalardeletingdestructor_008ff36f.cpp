// CAIStateGroup_MinionSitAtHome::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit ~CAIStateGroup_MinionSitAtHome (inlined: no members of its own, so it is the base ~CAIStateGroup_MinionSitOnSeatBase), then operator delete
//   when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_MinionSitAtHome_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF472 (0x008FD56A self respell, callee-only here): the call at +3 (retail 0x008FF372 -> 0x008FD56A, vftable 0x0128B6C4 slot 0) now names the real implicit
//   destructor ??1CAIStateGroup_MinionSitOnSeatBase@@UAE@XZ on the shared retail class (engine/CAIStateGroup_MinionSitOnSeatBase.h, FableWin.pdb name;
//   Ego_r / FableWin type records flag it compiler-generated), not the free stand-in CAIStateGroup_MinionSitAtHome_Destruct. VC7.1 emits that destructor here as a
//   pick-any COMDAT byte- and relocation-equal to retail 0x008FD56A (POwner +0x34, then ~CAIStateGroup_UsesSeat 0x008FD544).
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEATBASE
class CThing; class CWorld; class CThingAICreature;
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_MinionSitOnSeatBase.h"
void* __fastcall CAIStateGroup_MinionSitAtHome_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    static_cast<CAIStateGroup_MinionSitOnSeatBase*>(stateGroup)->CAIStateGroup_MinionSitOnSeatBase::~CAIStateGroup_MinionSitOnSeatBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
