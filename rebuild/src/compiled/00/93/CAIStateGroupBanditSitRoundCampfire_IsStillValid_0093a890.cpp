// CAIStateGroup_BanditSitRoundCampfire::IsStillValid() const -- the state group stays valid while its campfire exists and is not dead: IS_THING_ALIVE(PCampfire)
// Leaf: ?IsStillValid@CAIStateGroup_BanditSitRoundCampfire@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A890, oracle length 39 bytes, thiscall (reached through the vtable only).
// Retail bytes:  push esi ; lea esi,[ecx+0x14] ; mov ecx,esi ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ;
//                test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Name (NF404, T3 nf401 0x008D04B0): Ego_r.pdb 0x0095DCE0 + FableWin.pdb 0x02B7DC90 ?IsStillValid@CAIStateGroup_BanditSitRoundCampfire@@UBE_NXZ.
//   (1) retail RTTI vtable 0x0128B78C (.?AVCAIStateGroup_BanditSitRoundCampfire@@) slot 5 holds 0x0093A890; FableWin's vtable of the same class has ?IsStillValid@CAIStateGroup_BanditSitRoundCampfire@@UBE_NXZ in that slot
//       (same slot count, neighbours aligned).
//   (2) FableWin.pdb 0x02B7DC90 (/Od, 22 B): IS_THING_ALIVE<CIntelligentPointer<CThingObject>> on this+0x18 (the debug base is 4 bytes larger); Ego_r.pdb 0x0095DCE0.
//   The old self ?IsThingAlive_0093a890@@YI_NAAUCHolder@@@Z (a free __fastcall over a made-up CHolder) was a stand-in.
// Layout: the shared engine/CAIStateGroup_BanditSitRoundCampfire.h (PCampfire +0x14) with the new declaration-only opt-in FABLE_RETAIL_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_ISSTILLVALID; IS_THING_ALIVE from engine/ThingAlive.h (GetPItem 0x00A01B50 twice: the null test, then CThing::IsDead, bit 0 of +0x91).
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
#define FABLE_RETAIL_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_ISSTILLVALID
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_BanditSitRoundCampfire.h"
#include "engine/ThingAlive.h"

bool CAIStateGroup_BanditSitRoundCampfire::IsStillValid() const
{
    return IS_THING_ALIVE(PCampfire);
}
