// CAIStateGroup_BalverineCornered::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit
//   ~CAIStateGroup_BalverineCornered, then operator delete when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_BalverineCornered_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF472 (0x008FD288 stand-in binder 0x00900533, callee-only respell): the call at +3 (retail 0x0090051A -> 0x00900533) now names the real implicit
//   destructor ??1CAIStateGroup_BalverineCornered@@UAE@XZ on the shared retail class (engine/CAIStateGroup_BalverineCornered.h; Ego_r /
//   FableWin type records flag ~CAIStateGroup_BalverineCornered compiler-generated), not the free stand-in CAIStateGroup_BalverineCornered_Destruct.
//   VC7.1 emits that destructor here as a pick-any COMDAT byte- and relocation-equal to retail 0x00900533 (PLastJumpTarget +0x1C, PTarget +0x14, then
//   ~CAIStateGroupBase 0x008FD288).
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUP_BALVERINECORNERED
class CThing; class CWorld; class CThingAICreature;
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_BalverineCornered.h"
void* __fastcall CAIStateGroup_BalverineCornered_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    static_cast<CAIStateGroup_BalverineCornered*>(stateGroup)->CAIStateGroup_BalverineCornered::~CAIStateGroup_BalverineCornered();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
