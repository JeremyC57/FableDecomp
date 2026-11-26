// Retail 0x008fd56a: CAIStateGroup_MinionSitOnSeatBase::~CAIStateGroup_MinionSitOnSeatBase (catalog name; the implicit destructor: the
//   CIntelligentPointer member +0x34, then the base ~CAIStateGroup_UsesSeat by tail jump). Self symbol unchanged (free stand-in; NAME_FIXES.tsv).
// Leaf: @_Dest_val_pair_long_CRegisteredMusicEntry@4
// push esi; mov esi,ecx; lea ecx,[esi+0x34]; call 0xa01c10; mov ecx,esi; pop esi; jmp 0x8fd544
// NF471 (T3 nf303 0x008FD288, callee-only respell): the tail jump (retail 0x008FD578 -> 0x008FD544) now names the real implicit destructor
//   ??1CAIStateGroup_UsesSeat@@UAE@XZ on the shared retail class (engine/CAIStateGroup_UsesSeat.h), not the free stand-in tail_8fd544 (a fourth
//   name for 0x008FD544). VC7.1 also emits that destructor here as a pick-any COMDAT byte- and relocation-equal to retail 0x008FD544.
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

extern "C" void __fastcall _Dest_val_pair_long_CRegisteredMusicEntry(void* self)
{
    ((CBaseIntelligentPointer*)((char*)self + 0x34))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    static_cast<CAIStateGroup_UsesSeat*>(self)->CAIStateGroup_UsesSeat::~CAIStateGroup_UsesSeat();
}
