# Hand chunk 872 (CodaGetErrorString .. CAudioSource::ApplyHardware): 12 of 14 byte-exact

All 12 are RELOCATION_MATCH under qq.py/gall.sh (two at their true length, below), behaviour tests PASS under g++ and VC7.1
(tall.sh / tall.py), call / data relocation targets checked with calls.py. No `__asm`, no header overlay (engine/CString.h
already carries the FABLE_RETAIL_CSTRING_* opt-ins these rows use), no inline-budget flags.

## What matched (12)
- **GameCODA errors** -- 0x00C3CBD0 `CodaGetErrorString(long)`: switch over 0..-26 returning the C_* names (first grade).
- **CAmbientCube** -- 0x00C45990 `Dump()`: header + eight "   Apex (x,y,z) = d" CString trace lines. SHAPE: the any-apex test
  is a loop with an early exit (`for (i = 0; i < 8 && !any; ++i) any = d != 0.0f;`); retail keeps the up-counter next to the
  apex pointer. Without the early exit VC7.1 fully unrolls for/while forms (1543 bytes) or turns do-while into a down-counter.
  Now uses engine/CString.h (was a local CString model in the earlier differ file).
- **PKCBufferedReader** -- 0x00C4A3B0 `Refill()`: window move / claim / per-run ReadRemote with seven traces. SHAPES (23 grades):
  `long current = pos / blockSize; long move = current - lookBehind;` (sub in eax, copy to the callee-saved move); `if (m_blockCount
  > count)`; the claim count computed BEFORE the file-block total (blockCount lives across the div -> edi); a NAMED `total` local
  for the EOF recompute plus a named `block = firstBlock + blockCount` (fixes both the blockSize/blockCount register swap and the
  scratch rotation); the flush is `dest` first, then `bytes = GetAvailableReadSize(offset, bytes)` (retail stores the clamped size
  back into bytes); in the run loop `if (slot == 0) { flush } else { extend; continue; }` puts the extend block after the
  start-run block.
- **CEAXManager** -- 0x00C4E730 `GetListenerProperty`, 0x00C4EED0 `GetVoiceBufferProperty` (result accumulator `long result = -N;
  if (...) switch { ... result = 0; break; } return result;`), 0x00C51A20 `QueueCommit` (direct member expressions, not reference
  locals), 0x00C4EA30 `ApplyVoiceBufferProperties` (new): clamps the voice's EAX30BUFFERPROPERTIES into its applied block and
  the owner object. SHAPE: the clamp is `(v <= hi && v >= lo) ? v : (v > hi ? hi : lo)` (jp / test ah,1 order); every value
  kept in two places is an if/else storing both copies in each arm (in-range arm: raw bit copy; out-of-range: x87 select for
  the first, constant stores with a re-test for the second); `if (!id || data) { ... }` (not an early return, which folds into
  the null-owner exit); `long` result (bool keeps it in bl); 3D path one checked downcast, 2D path one per call.
- **CDirectSoundDevice** -- 0x00C53340 `DetectSpeakerConfig()`: GetSpeakerConfig switch in source order (first grade).
- **CAudioContext** -- 0x00C56A40 `Open` (/GS; the landed Initialise body inline in the TU).
- **CVtable_012b8ed4** -- 0x00C57800 `CreateAndPlay` (first grade).
- **CSoftwareStreamVoice** -- 0x00C5C360 `MixRingBlock` (/GS; four if-else channel arms, first grade).
- **CAudioSource** -- 0x00C880A0 `ApplyHardware` (new): EAX direct / room, AL gain, pitch retry, position. SHAPES: inline
  CAudioContext wrappers (EAXSet, Sourcef, Sourcefv, GetError) with CAudioContext::GetProcAddress's landed body in the TU: VC7.1
  itself expands it only inside the pitch retry loop, exactly as retail (no flag needed); AmplitudeToMillibels expanded inline
  with the zero test first (`if (a == 0) r = -10000; else { r = FtoL(...); if (r < -10000) r = -10000; }`) -- the landed
  out-of-line form lays the blocks out differently; `gain *= a; gain *= b;` (one product sorts as a*b first); the next pitch
  `pitch *= 0.8f` written AFTER `error = GetError()` (retail schedules the multiply before the call); `x*x + (y*y + z*z)`.

True extents / extra function starts (catalog extents are next-manifest gaps):
- 0x00C51A20: 1164 bytes (+ tables); 0x00C51EB0..0x00C51EF9 is a separate function (`ret 14h`, calls 0x00C68FE0 then QueueCommit).
  gall.sh reports DIFFER(1164v1244) only because it grades the catalog extent.
- 0x00C53340: 180 bytes; 0x00C53400, 0x00C53410, 0x00C53420, 0x00C53430 are separate functions inside the catalog extent.
  gall.sh reports DIFFER(180v256) for the same reason.
- 0x00C4A3B0 1643, 0x00C4EA30 1182, 0x00C880A0 1307: single functions (the catalog tail is 0xCC pad).
- 0x00C51F50: one function, `ret 14h` at 0x00C527CA, 28-entry jump table 0x00C527D0..0x00C52840.

## Parked for Fable 5.1 (0)

## Skipped
- 0x00C51F50 (catalog "CDrawGuildSeal_SetProperty", wrong): the EAX 4.0 source-property setter of the voice block
  (`id & 0x7fffffff` <= 27 = DSPROPERTY_EAXSOURCE_*; ALLPARAMETERS memcmp/copy of 0x48 bytes, the obstruction / occlusion /
  exclusion groups, one change-tested store per long/float field into a `changed` byte, the send / occlusion-send /
  exclusion-send arrays (size / 24, / 32; VC unrolled by 4) and ACTIVEFXSLOTID; changed -> 0x00C51F00, the default path
  commits through the voice's +0x1F0 CEAXBuffer 0x00C68510 and QueueCommit 0x00C51A20; thiscall `ret 14h`). Not attempted:
  session budget. Retail's send loops store every element into the SAME member (+0xB8..+0xC4 / +0x138), reproduce that bug.
- 0x00C5B7C0 (gapscan): software-voice render step with __int64 positions (_allmul 0x00BFEB50, 0x00F3FC50), vtable slots
  +0x2C/+0x60/+0x6C, callees 0x00C5ACB0, 0x00C3C4A0, 0x00C5B480, 0x00C5B300, 0x00C5AF50, 0x00C6B720, 0x00C5AFC0, 0x00C6BAE0,
  0x00C6B9D0, 0x00C5CF90; `ret 0Ch`, ends 0x00C5BCF2. Not attempted: session budget.

## Label corrections
- 0x00C45990 "Render_CheckAmbientCubeLighting" -> CAmbientCube::Dump; 0x00C4A3B0 "PrimeBuffer" -> Refill;
  0x00C4EA30 "CTCLook_ProcessHeadTurn" -> CEAXManager::ApplyVoiceBufferProperties; 0x00C51A20 "PropertySerializer_WriteProperty"
  -> CEAXManager::QueueCommit; 0x00C51F50 "CDrawGuildSeal_SetProperty" -> EAX 4.0 source property setter;
  0x00C5C360 "Mesh_UpdateVertexColors" -> CSoftwareStreamVoice::MixRingBlock.
- Landed callee spellings proven wrong are in NAME_FIXES.tsv (0x00C415A0 / 0x00C415C0 / 0x00C41E40 / 0x00C44860 / 0x00C44880 are
  the 3D / 2D audio objects' EAX setters, not CTCLook / CCombatant / CRenderSetting; 0x00C58740 is CMixBuffer::MixQuad16;
  QueueCommit's first parameter; Refill's class; 0x012BEEA0 is the EAX 3.0 buffer GUID).
- CAudioObject vtable slot 1: CAudioObject::Open 0x00C42DD0 and this chunk call it Is3D; the landed ApplySound 0x00C72E00 calls it
  IsStreamed (it selects the 3D object for ApplyHardware, whose body reads the 3D members +0xFC..+0x150): Is3D.
