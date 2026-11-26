// CAIStateGroup_MinionReactToCriminal::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit
//   ~CAIStateGroup_MinionReactToCriminal, then operator delete when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_MinionReactToCriminal_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF472 (0x008FD288 stand-in binder 0x008FECEC, callee-only respell): the call at +3 (retail 0x008FECD3 -> 0x008FECEC) now names the real implicit
//   destructor ??1CAIStateGroup_MinionReactToCriminal@@UAE@XZ on the shared retail class (engine/CAIStateGroup_MinionReactToCriminal.h; Ego_r /
//   FableWin type records flag ~CAIStateGroup_MinionReactToCriminal compiler-generated), not the free stand-in CAIStateGroup_MinionReactToCriminal_Destruct.
//   VC7.1 emits that destructor here as a pick-any COMDAT byte- and relocation-equal to retail 0x008FECEC (PHome +0x1C, PHero +0x14, then
//   ~CAIStateGroupBase 0x008FD288).
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL
class CThing; class CWorld; class CThingAICreature;
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_MinionReactToCriminal.h"
void* __fastcall CAIStateGroup_MinionReactToCriminal_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    static_cast<CAIStateGroup_MinionReactToCriminal*>(stateGroup)->CAIStateGroup_MinionReactToCriminal::~CAIStateGroup_MinionReactToCriminal();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
