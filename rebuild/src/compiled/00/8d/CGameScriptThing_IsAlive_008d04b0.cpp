// CGameScriptThing::IsAlive() const -- the script thing is alive while its thing exists and is not dead: IS_THING_ALIVE(PThing)
// Leaf: ?IsAlive@CGameScriptThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D04B0, oracle length 39 bytes, thiscall (reached through the vtable only).
// Retail bytes:  push esi ; lea esi,[ecx+0xc] ; mov ecx,esi ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ;
//                test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Name (NF404, T3 nf401 0x008D04B0): Ego_r.pdb 0x00907510 + FableWin.pdb 0x02B00770 ?IsAlive@CGameScriptThing@@UBE_NXZ.
//   (1) retail RTTI vtable 0x012852CC (.?AVCGameScriptThing@@) slot 75 holds 0x008D04B0; FableWin's vtable of the same class has ?IsAlive@CGameScriptThing@@UBE_NXZ in that slot
//       (same slot count, neighbours aligned).
//   (2) FableWin.pdb 0x02B00770 (/Od): the debug-only ProcessBreathe() call, then IS_THING_ALIVE<CIntelligentPointer<CThing>> 0x01CCA150 on this+0x0C; Ego_r.pdb 0x00907510 is this retail body instruction for instruction (`lea esi,[ecx+0Ch]`, GetPItem twice, the dead bit at Ego_r's +0xA1).
//   The old self ?IsThingAlive_008d04b0@@YI_NAAUCHolder@@@Z (a free __fastcall over a made-up CHolder) was a stand-in.
// Layout: the shared engine/CGameScriptThing.h class branch (PThing +0x0C) with the new declaration-only opt-in FABLE_RETAIL_CGAMESCRIPTTHING_ISALIVE; IS_THING_ALIVE from engine/ThingAlive.h (GetPItem 0x00A01B50 twice: the null test, then CThing::IsDead, bit 0 of +0x91).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTTHING
#define FABLE_RETAIL_CGAMESCRIPTTHING_ISALIVE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameScriptThing.h"
#include "engine/ThingAlive.h"

bool CGameScriptThing::IsAlive() const
{
    return IS_THING_ALIVE(PThing);
}
