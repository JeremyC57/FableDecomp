# NOTES -- chunk 868 (agent868) retry of ten parked near misses

Result: 2 of 10 matched (0x00D18DF0, 0x00ECBDF0), 8 parked. Both matched rows pass their behaviour tests under the lane
(`python3 $A/tall.py`: cl /Od + link + wine) and under g++. All grades are in ATTEMPTS.log (134 lines; per row: d11930 64,
c52bb0 16, dcfa60 15, b92300 13, d18df0 10, ecbdf0 7, 7a1b20 6, ece7f0 2, d45060 1, a2f740 0). Nothing written under the repo;
no git. Coordinator instruction honoured: check_extents.py never run (extents via `$S/w17/xdis.py`).

## Matched

### 0x00D18DF0 NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main -- RELOCATION_MATCH(3321v3321)
- Start residue (865): one `jne` (0x00D195CF) joined the end tail 0x00D19A8B where retail joins the ~deputy_pos/~seh_me tail of the
  speech pad (0x00D19977).
- Device: the follow loop written with its terminate check at the loop top; VC7.1 inverts the loop and peels that check in front,
  and the peeled copy is the one return that cross-jumps to the speech-pad tail while the 13 others share the end tail.
- Inline budget: `// Flags: /d2inlS15` with the `// Inline-budget:` evidence line -- the implicit ~CScriptGameResourceObjectMovieBase
  at the two speech-pad sites (0x00D1995D, 0x00D19A44) is a `??1..@@UAE@XZ` call in the plain build (3287 bytes, md 6); S15 is the
  smallest N that expands both (S14, S12, S10, S8 graded, all 3287); the FABLE_RETAIL_..._FORCEINLINE_CTOR user dtor adds a vtable
  store per site (chunk 797) and no scoped pragma raises inlining. Explicit specialization declaration of
  `CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()` (0x006E7AB0) keeps the call out of line.

### 0x00ECBDF0 NScript::CV_SickChildScript::CTalkingTrader1::Main -- RELOCATION_MATCH(2928v2928), plain C++
- Start residue: ~CTimer split -- retail expands it at the main-loop exit (0x00ECC866) and calls 0x00CD4470 at the hit-block
  speech returns (0x00ECC92C); `while (!IsActiveThreadTerminating())` gave both the same frequency.
- Device: lever 4 loop-head macro on the main loop (`for (;;) { do { if (T) return; } while (0); ...`) plus the talk block's
  existing `do { if (T) return; } while (0);` before `CCharString TalkString;` (macro alone: 2972 bytes, md 25). Precedent: landed
  0x00D99A30 CQ_HobbeCaveScript::CheckIfGotTreasure.
- Test fix: the lane build printed FAIL while g++ passed -- VC7.1 `World w = World();` leaves the scalar members of a class with an
  implicit ctor and vector members unset; the model now has an explicit zeroing ctor.

## Parked (differ/<addr>.cpp line 2 = residue + probe quantity, line 3 = next idea)

- 0x00D11930 CAssassinMarker::Main -- DIFFER(4056v4072) md 24 from md 263 (two scoped `inline_depth(0)` devices: round the
  `~CIsThingAlive` definition and round the final loop's `if (T) return;`). Residue: one cold cut-scene MovieBase dtor site 0x00D1284D
  expanded in retail, a call in ours. S8..S15 all permute this row's frame slots (worse); w1/w2/all1/dsingle/dattack fail. 64 grades.
- 0x00DCFA60 CQ_OrchardFarmRaidScript::DoCutsceneIfRequired -- DIFFER(2451v2412) md 41. The post-region-wait `if (T) return;`
  (0x00DCFC14) expands the CScriptThing release of `spawn`; retail calls 0x004AA840 via the shared tail. Every other outer/inner loop
  form (p7b, q1, q2, q7, q8, q10) permutes the frame; q11/p5/p6 change nothing. 15 grades.
- 0x00B92300 CEngineSceneGridCell::CreateOcclusionGroup -- DIFFER(335v335) md 4, prologue order only. c2prio probe: allocation is
  retail in every spelling (layer 41 > handle 32, this split); a5 gives the load-first order but swaps the allocation (handle 40 >
  layer 33). Residue is the entry-block pre-schedule order, which the probe does not expose. 13 grades (12 spellings).
- 0x00C52BB0 CCpuInfo::GetAMDModelName -- DIFFER(140v140) md 4: `cmp eax,3 ; ja` v `cmp eax,4 ; jae`. Probe: colouring order
  `this` v the K5 Model temp; compare constants create no const records but the switch `sub` does and a compare links to it. 16 grades.
- 0x00ECE7F0 CWitch::Main -- DIFFER(4086v4086) md 4: 0x00ECF317 `mov eax,esp ; push eax` (hidden return slot of
  CDefString::operator CCharString) v ours `push esp`; unique site in the exe. 2 grades (rotation probe).
- 0x00D45060 CQ_GuildTrainingScript::CheckFriendlyAttacks -- DIFFER(3402v3439) md 375 frame-slot order; baseline only.
- 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate -- rebuilt on shared headers (11 overlay headers/opt-ins, see hdr/README.md);
  DIFFER(2712v2840) md 490. Prologue right apart from `xor ebp,ebp` / `mov ebx,0x10` register constants and frame 0xc4 v 0xbc (8 bytes:
  a CHitParameters / pair temporary). pack(1) is not a factor (r2 == r1); a named float local is worse (r4). 6 grades.
- 0x00A2F740 C3DMeshStats::InitialiseFromFileData -- NOT rebuilt: the shared C3DMeshFile*Chunk headers carry no FindFirst*/FindNext*/
  GetRoot finders and there are no C3DMeshStats / C3DHelperPoint / C3DDummyObject / C3DGeneratorObject / C3DMeshVolume headers. The
  draft stands as received (DIFFER(2613v3051) md 1333). Still owed.

## Names
LABELS.tsv: 0x00D18DF0, 0x00ECBDF0, 0x007A1B20, 0x007A0400 StartLoopingSound, 0x0085F120 IsUnderground, 0x0085FAA0 IsLightningable,
0x00811830 CEventHitBySpecialAbility ctor (two sources each). NAME_FIXES.tsv: none.

## Files
src/00d18df0.cpp, src/00ecbdf0.cpp (+ test/), payload.json + oracle.tsv (finish.py), differ/ (8), hdr/ (+README.md), STATUS.tsv,
ATTEMPTS.log, LABELS.tsv, NAME_FIXES.tsv, LESSONS.md, variant dirs tmp/{b92,c52,d11,d18,dcf,ecb,ece,7a1}/, probes probe/.
