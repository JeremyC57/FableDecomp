# 2026-10-14_linux_hand42 — hand-authored chunk 42 of the class-labelled 17–96-byte pool

Coordinator-authored (no sub-agent) on the Linux VC7.1 lane. Chunk 42 (`mkchunk.py 42`, 40 rows, all
`CGameScriptInterface` script entry points): **34 of 40** landed as `RELOCATION_MATCH`/`MATCH` with behaviour tests.
The shared model: `CScriptThing` virtual slot 75 (+0x12c) `IsValid()` then slot 11 (+0x2c) `GetThing()`; the retail
CThing layout (+0x10 type byte: 1 = the player creature, 2 = an AI creature; +0x6c flags: bit 1 physical, bit 3
creature; bit-fields at +0x91/+0x92/+0xbc/+0xd4/+0x15c/+0x15d; the speeds at +0x190/+0x198/+0x1b8/+0x1c0); `CWorld`
slots 3/12/13 (thing manager, world map for edit, world map) and the PDB members (`PWorldMap_ptr` +0x14,
`PEnvironment_ptr` +0x18, `POpinionReactionManager_Data` +0x74, `PScriptConversationManager_ptr` +0x7c);
`CGameScriptInterface::World` +4, `DisplayEngine` +0xc, `PlayerManager` +0x14, `AllowScreenFadingIfAlreadyFaded` +0x2d.

## Lander fix (tools/decomp_pipeline/verify_and_land.py)
`objdump -d` collapses runs of zero bytes into `...`, which dropped the zero-filled entries of a switch's jump and
index tables (0x0088E980, 70 of 90 bytes extracted): the lander now disassembles with `-z`.

## Lessons (also in GOTCHAS, wave 24j)
- A member read as `this` for a call that is re-read from memory in every arm (`mov ecx,[esi+14h]` twice) is a POINTER member.
- A bit-field store from a bool argument is the `xor ; and mask ; xor` idiom; a second bit-field byte needs the first byte's
  bits padded to 8 or the fields pack into the same unit (+0x15c/+0x15d).
- The thing fetched BEFORE the argument is pushed = a local (`CThing* t = thing.GetThing(); t->Set(x)`); fetched after the
  push = the chained call. An object loaded straight into ecx before its argument = an inline accessor (`GetPhysics()`).
- A manager/player/interpolator loaded before the arguments are pushed = a local taken before the call.
- `if (a == b) { if (f()) return true; } return false;` gives the shared `xor al,al` tail with `test al,al ; je`; the
  `&&` expression form gives full-register 0/1, the `if (!f()) return false; return true;` form gives `setne`.
- A tail jump with argument-slot reuse (`mov [esp+4],1 ; jmp callee`) reproduces for a void call as the last statement.
- A `switch` over a dense case set with two outcomes compiles to the byte-indexed two-entry jump table; the out-of-range
  default is the `ja` target.

## Parked (6; sources in the lane's `differ/`)
- 0x0088E260 `IsThingWithThisUIDAlive`: retail `mov cl,[eax+91h] ; not cl ; test cl,1` with `mov al,1`; the lane gives
  `test byte ptr,1` or a full-register bool (bit-field, byte local and `&&` forms tried).
- 0x0088E540 `EntityTeleportToThing`: register choice for the flag/vtable (eax/edx v ecx/eax).
- 0x008900D0 / 0x00890110 `RadialBlurFadeTo`: retail hoists the World->environment loads above the eleven/seven argument
  loads and the `+0x1964` load below them; the lane evaluates the arguments first (local, accessor chain, reference tried).
- 0x00890820 `FadeScreenOut`: the colour's high word computed on both paths before the join (`shr edx,10h` / `mov dx,[esp+12h]`)
  and the four bytes stored into the pushed slot; closest 71 v 80 (two 16-bit halves + user copy constructor).
- 0x00890990 `EntityGetOpinionAttitudeToPlayer`: retail tail-jumps with the thing overwriting the argument slot; the lane
  calls and returns (if/else and enum-return forms tried).
