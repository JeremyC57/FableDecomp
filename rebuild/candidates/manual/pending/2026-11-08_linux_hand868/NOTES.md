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

Round 2 (coordinator 2026-11-08r: both matches landed; sources/tests moved to landed/, so finish.py emits only new matches -- none).
Every differ row re-graded against the final hdr/ overlay (attempts tagged `hand`/`hand2`), grades as stated below.

- 0x00D45060 CQ_GuildTrainingScript::CheckFriendlyAttacks -- plain best stays the chunk-865 base md 375 (3402v3439, frame slot order);
  23 grades this chunk: inline ~CScriptThing body (v1 md 422), dropping the after-second-loop T check (v2 532), lever-4 head (v3), do-while(0)
  on each T check (v4/v5/dwall/dwsel), in-class / non-inline dtor definitions (v9/v10 = v1 bytes), for(;;)+break loops (v11/v12), an
  `if (!creatures.empty())` guard on the second loop (v8a 417, v8b size()>0 436). Diagnostic /d2inlT: on v2 T60 265 / T50 418 / T40 490 /
  T30 487; on v8a T60 164, T56 103 (the differ: 3396v3439, only 43 bytes short); on v8b T58 182, T56 121. The T56 residue is zero-register
  homing (`xor edi,edi` once v `xor ebp,ebp` per site, `push 0` v `push ebp`, `test` v `cmp ecx,ebp`, `inc [esi+50h]` v load/inc/store) plus
  the guard's `cmp ecx,eax ; je` which retail lacks -- so the inline sites T56 flips are the plain-C++ quantity (next idea in the differ).
  Meter (tmp/d45/meter.py): marker-call frequencies of each T site were measured; none isolates the release copies.
- 0x00ECE7F0 CWitch::Main -- DIFFER(4086v4086) md 4 unchanged: the hidden-return `mov eax,esp ; push eax` v `push esp` at 0x00ECF317.
  8 more spellings of `pGlobalScriptDef->EndPotion` (functional / static_cast CCharString: md 71 with the conversion through a named
  temporary; a `const CDefString*` local: md 8; `.operator CCharString()` direct / `(*p).EndPotion`: md 4; const-ref / named CCharString
  locals: 71 / 93; a `const CScriptDef&` local: md 9). 10 grades. Unique site in the exe; no retail sibling shows which spelling folds.
- 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate -- shared-header rebuild (11 overlay headers/opt-ins, hdr/README.md);
  DIFFER(2712v2840) md 426 (r5: the AddTC component name as a named CCharString local builds the temporary before the pushes, 490 -> 426).
  Prologue right apart from `xor ebp,ebp` / `mov ebx,10h` register constants and frame 0xc4 v 0xbc (8 bytes: a CHitParameters / pair
  temporary). pack(1) is not a factor (r2 == r1); a named float local is worse (r4). 7 grades.
- 0x00A2F740 C3DMeshStats::InitialiseFromFileData -- rebuilt on the shared headers (priority 5): 8 new overlay headers (C3DMeshStats,
  C3DMeshFile, C3DHelperPoint, C3DDummyObject, C3DGeneratorObject, C3DMeshVolume, CPreAllocVectorMap, CCRC) and 12 additive opt-ins
  (FindFirst*/FindNext* finders, GetRoot/HasHelpers/GetHelpers, chunk getters, CCharString GetCRC inline / GetAsLowerCase, std::copy,
  C3DBoundingBox::SetCenterDiag). r1 md 1511 -> r10 DIFFER(3059v3051) md 720, 14 grades, 3 probes. Every callee and every inline decision
  now matches retail (vector<C3DGeneratorObject>::push_back and both std::__copy out of line; the pair push_backs, GetCRC = CCRC::Calc and
  ~C3DMeshVolume inline), `this` in ebp, bone/transform/bounding blocks right. Residue is pure allocation: the zero constant homed in ebx
  (retail) v esi (ours: the const-0 piece takes the first free callee-saved register in table order, so retail's esi was already taken by
  the dummy-name-address temp it CSEs once and reloads), frame 0x174 v 0x170 and the slot shift behind it, the bone load scheduled before
  the Transform `rep movsd`, dl/cl temp rotation. Learned: `while (!p.IsNull()) { ...; p = FindNext(p); }` reproduces retail's inline
  decisions where `for (...; p = FindNext(p))` over-inlines push_back/__copy (r3); a named `const CCharString&` reference wins ebp over
  `this` (r7/r9/r11 md 830 v r8/r10 749/720); a named `CCharString createMeshPrefix("createmesh ")` local merges the two null tests
  as retail (r10); a named root-chunk pointer (r13/r14) lowers md (645/661) but adds 8 frame bytes or the wrong register -- not the
  quantity; C3DDummyObject built by member assignments is byte-identical to the ctor form (r12).
- 0x00D11930 CAssassinMarker::Main -- DIFFER(4056v4072) md 24 from md 263 (two scoped `inline_depth(0)` devices: round the
  `~CIsThingAlive` definition and round the final loop's `if (T) return;`). Residue: one cold cut-scene MovieBase dtor site 0x00D1284D
  expanded in retail, a call in ours. S8..S15 all permute this row's frame slots (worse); w1/w2/all1/dsingle/dattack fail. 64 grades (round 1).
- 0x00DCFA60 CQ_OrchardFarmRaidScript::DoCutsceneIfRequired -- DIFFER(2451v2412) md 41. The post-region-wait `if (T) return;`
  (0x00DCFC14) expands the CScriptThing release of `spawn`; retail calls 0x004AA840 via the shared tail. Every other outer/inner loop
  form (p7b, q1, q2, q7, q8, q10) permutes the frame; q11/p5/p6 change nothing. Round 2: diagnostic /d2inlT sweep on the base -- T72 md 41
  (unchanged), T66 480, T60 482, T56 228, T50 748: other sites flip before the post-wait release, so device 2 cannot isolate it. 20 grades.
- 0x00B92300 CEngineSceneGridCell::CreateOcclusionGroup -- DIFFER(335v335) md 4, prologue order only. c2prio probe: allocation is
  retail in every spelling (layer 41 > handle 32, this split); a5 gives the load-first order but swaps the allocation (handle 40 >
  layer 33). Residue is the entry-block pre-schedule order, which the probe does not expose. 18 grades.
- 0x00C52BB0 CCpuInfo::GetAMDModelName -- DIFFER(140v140) md 4: `cmp eax,3 ; ja` v `cmp eax,4 ; jae`. Probe: colouring order
  `this` v the K5 Model temp; compare constants create no const records but the switch `sub` does and a compare links to it. 18 grades.

## Names
LABELS.tsv: 0x00D18DF0, 0x00ECBDF0, 0x007A1B20, 0x007A0400 StartLoopingSound, 0x0085F120 IsUnderground, 0x0085FAA0 IsLightningable,
0x00811830 CEventHitBySpecialAbility ctor (two sources each). NAME_FIXES.tsv: one local-model member spelling in the landed C3DMeshStats::AllocateBuffers row (PackedNames / PackedNamesSize, PDB).
No new LABELS rows for 0x00A2F740: every callee it names is already landed under its PDB name (finders 0x00AAA270.., GetRoot 0x00A32620,
HasHelpers 0x00A9B2C0, GetHelpers 0x00A9B3D0, ??0C3DMeshVolume 0x00A2E9A0); g_CRCTable 0x0129A168 has no PDB name (CCRC.h comments it).

## Files
landed/src + landed/test (the two 2026-11-08r landings, kept out of finish.py), src/ empty -> payload.json holds no rows, oracle.tsv (finish.py),
differ/ (8, all re-graded against hdr/), hdr/ (+README.md: 8 new headers, 14 opt-in'd repo headers, one identical lane copy), STATUS.tsv,
ATTEMPTS.log, LABELS.tsv, NAME_FIXES.tsv, LESSONS.md, variant dirs tmp/{b92,c52,d11,d18,dcf,ecb,ece,7a1,d45,a2f}/, probes probe/,
tools tmp/a2f/calls.py (call sequence of a .obj function with symbol names) and tmp/a2f/ndiff.py (register/offset-normalised structural diff).