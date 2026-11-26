// CAIStateGroup_SitInPub::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit ~CAIStateGroup_SitInPub expanded (its
//   CIntelligentPointer member +0x34, then the base ~CAIStateGroup_UsesSeat), then operator delete when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_SitInPub_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Retail: slot 0 of vftable 0x0128BF04 (RTTI .?AVCAIStateGroup_SitInPub@@ : CAIStateGroup_UsesSeat).
// NF471 (T3 nf303 0x008FD288, callee-only respell): the call at 0x00900228 (retail -> 0x008FD544) now names the real implicit base destructor
//   ??1CAIStateGroup_UsesSeat@@UAE@XZ on the shared retail class (engine/CAIStateGroup_UsesSeat.h; both PDBs flag it compiler-generated), not the
//   free stand-in CAIStateGroup_SitInPub_Destruct (a second name for 0x008FD544). VC7.1 also emits that destructor here as a pick-any COMDAT
//   byte- and relocation-equal to retail 0x008FD544; the row's own bytes are unchanged.
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
void* __fastcall CAIStateGroup_SitInPub_CompositeScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(stateGroup) + 52))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    static_cast<CAIStateGroup_UsesSeat*>(stateGroup)->CAIStateGroup_UsesSeat::~CAIStateGroup_UsesSeat();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
