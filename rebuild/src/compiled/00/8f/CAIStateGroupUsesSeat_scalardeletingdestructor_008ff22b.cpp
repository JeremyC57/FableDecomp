// CAIStateGroup_UsesSeat::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit ~CAIStateGroup_UsesSeat, then
//   operator delete when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_UsesSeat_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF471 (T3 nf303 0x008FD288, callee-only respell, ruling r470a): the call at +3 (retail 0x008FF22E -> 0x008FD544) now names the real implicit
//   destructor ??1CAIStateGroup_UsesSeat@@UAE@XZ on the shared retail class (engine/CAIStateGroup_UsesSeat.h; Ego_r / FableWin type records flag
//   ~CAIStateGroup_UsesSeat compiler-generated), not the free stand-in CAIStateGroup_UsesSeat_Destruct. VC7.1 emits that destructor here as a
//   pick-any COMDAT byte- and relocation-equal to retail 0x008FD544 (PSeat +0x14, then ~CAIStateGroupBase 0x008FD288).
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
class CThing; class CWorld; class CThingAICreature;
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_UsesSeat.h"
void* __fastcall CAIStateGroup_UsesSeat_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    static_cast<CAIStateGroup_UsesSeat*>(stateGroup)->CAIStateGroup_UsesSeat::~CAIStateGroup_UsesSeat();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
