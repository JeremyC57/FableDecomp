// CAIStateGroup_OpinionReactionRoot::IsStillValid() const -- the reaction stays valid while the thing it reacts to exists and is not dead: IS_THING_ALIVE(ThingReactingTo)
// Leaf: ?IsStillValid@CAIStateGroup_OpinionReactionRoot@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095EE40, oracle length 39 bytes, thiscall (reached through the vtable only).
// Retail bytes:  push esi ; lea esi,[ecx+0x6b] ; mov ecx,esi ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ;
//                test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Name (NF404, T3 nf401 0x008D04B0): Ego_r.pdb 0x009833D0 + FableWin.pdb 0x02C276B0 ?IsStillValid@CAIStateGroup_OpinionReactionRoot@@UBE_NXZ.
//   (1) retail RTTI vtable 0x0128C984 (.?AVCAIStateGroup_OpinionReactionRoot@@) slot 5 holds 0x0095EE40; FableWin's vtable of the same class has ?IsStillValid@CAIStateGroup_OpinionReactionRoot@@UBE_NXZ in that slot
//       (same slot count, neighbours aligned).
//   (2) FableWin.pdb 0x02C276B0 (/Od, 22 B): IS_THING_ALIVE<CIntelligentPointer<CThing>> 0x01CCA150 on this+0x6F (the debug base is 4 bytes larger); Ego_r.pdb 0x009833D0.
//   The old self ?IsThingAlive_0095ee40@@YI_NAAUCHolder@@@Z (a free __fastcall over a made-up CHolder) was a stand-in.
// Layout: the shared engine/CAIStateGroup_OpinionReactionRoot.h class branch (_WIFEUPDATE + _THINGREACTINGTO: ThingReactingTo +0x6B) with the new nested declaration-only opt-in FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_ISSTILLVALID; IS_THING_ALIVE from engine/ThingAlive.h (GetPItem 0x00A01B50 twice: the null test, then CThing::IsDead, bit 0 of +0x91).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_ISSTILLVALID
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"

bool CAIStateGroup_OpinionReactionRoot::IsStillValid() const
{
    return IS_THING_ALIVE(ThingReactingTo);
}
