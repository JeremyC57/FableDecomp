// NCreatureMode::IsTransmogrified(const CThing& thing) -- true for a thing of type 2 (creature, byte +0x10) whose def name is not "CREATURE_HERO"
// Leaf: ?IsTransmogrified@NCreatureMode@@YI_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085EE10, length 41 bytes (+7 int3 padding).
// Callees: CThing::GetDefName @ 0x004C7CC0 (CDefString through the hidden pointer, `ret 4`) and
// CDefString::operator!=(const char*) const @ 0x005FA740 (landed); the literal 0x01231C88 is "CREATURE_HERO".
// Name (NF93, round 8, q11270): Ego_r.pdb ?IsTransmogrified@NCreatureMode@@YI_NABVCThing@@@Z at 0x00833030 heads the run IsTransmogrified /
//   IsUnconscious / GetStunnedEffectCreationPos / IsPlayHitResponse / IsRolling = retail 0x0085EE10 / 0x0085EE40 / 0x0085EE50 / 0x0085EE90 /
//   0x0085EEB0 (FableWin.pdb 0x02831D60 the same symbol; Ego_d record NCreatureMode::IsTransmogrified(thing)); the shared
//   engine/NCreatureMode.h declares it (FABLE_RETAIL_NCREATUREMODE_ISTRANSMOGRIFIED) and the landed 0x00805600 binds it. Was the made-up
//   global `IsCreatureButNotHero(CThing*)` (the manifest "CTCScriptedControl::IsActive" is a propagated label). The callers are
//   CTCTargetingPlayer::PostProcessTargetedThing (tail jump at 0x00803B4B with ecx = the parent thing) and CTCTargetingPlayer::FrameUpdate
//   (0x00805877). The local CDefString model replaced by the shared engine/CDefString.h.
// Shape: the 4-byte CDefString temporary lives in the `push ecx` slot; the literal is pushed before the GetDefName call
// (right-to-left evaluation of `GetDefName() != "..."`); `if (a && b) return true; return false;` gives the two
// `mov al,1` / `xor al,al` exits.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_NOTEQUAL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_NCREATUREMODE_ISTRANSMOGRIFIED
#include "engine/CDefString.h"
#include "engine/CThing.h"
#include "engine/NCreatureMode.h"

bool __fastcall NCreatureMode::IsTransmogrified(const CThing& thing)
{
    if (thing.GetType() == 2 && thing.GetDefName() != "CREATURE_HERO")
        return true;
    return false;
}
