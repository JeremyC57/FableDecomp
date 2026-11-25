// CAIStateGroup_WorkSawingWood::IsStillValid() const -- the state group stays valid while its sawhorse exists and is not dead: IS_THING_ALIVE(PSawhorse)
// Leaf: ?IsStillValid@CAIStateGroup_WorkSawingWood@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009445C0, oracle length 39 bytes, thiscall (reached through the vtable only).
// Retail bytes:  push esi ; lea esi,[ecx+0x15] ; mov ecx,esi ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ;
//                test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Name (NF404, T3 nf401 0x008D04B0): Ego_r.pdb 0x009683C0 + FableWin.pdb 0x02BAC230 ?IsStillValid@CAIStateGroup_WorkSawingWood@@MBE_NXZ (protected virtual const).
//   (1) retail RTTI vtable 0x0128BCBC (.?AVCAIStateGroup_WorkSawingWood@@) slot 5 holds 0x009445C0; FableWin's vtable of the same class has ?IsStillValid@CAIStateGroup_WorkSawingWood@@MBE_NXZ in that slot
//       (14 slots both, neighbours AllowPriorityOverlap / GetDebugText of CAIStateGroupBase aligned).
//   (2) FableWin.pdb 0x02BAC230 (/Od, 22 B): IS_THING_ALIVE<CIntelligentPointer<CThingObject>> on this+0x19 (the debug base is 4 bytes
//       larger); Ego_r.pdb 0x009683C0.
//   The old self ?IsThingAlive_009445c0@@YI_NAAUCHolder@@@Z (a free __fastcall over a made-up CHolder) was a stand-in.
// Layout: the shared engine/CAIStateGroup_WorkSawingWood.h with the new declaration-only class branch FABLE_RETAIL_CAISTATEGROUP_WORKSAWINGWOOD_CLASS (the flattened struct stays the
//   default): CAIStateGroupBase +0, TookActionPoint +0x14, PSawhorse +0x15 (packed); IS_THING_ALIVE from engine/ThingAlive.h (GetPItem
//   0x00A01B50 twice: the null test, then CThing::IsDead, bit 0 of +0x91).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_WORKSAWINGWOOD_CLASS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_WorkSawingWood.h"
#include "engine/ThingAlive.h"

bool CAIStateGroup_WorkSawingWood::IsStillValid() const
{
    return IS_THING_ALIVE(PSawhorse);
}
