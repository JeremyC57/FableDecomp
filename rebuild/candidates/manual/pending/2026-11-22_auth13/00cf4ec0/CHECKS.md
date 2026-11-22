# AUTH13 0x00CF4EC0 CFanMarker::Main: the four checks of the 2026-11-22 advisor ruling (AUTH12's escalation)
Base text: AUTH12 p6d (`$W/p6d.cpp` = pending/2026-11-22_auth12/00cf4ec0/00cf4ec0.cpp), overlay `$W/hdr` = AUTH12's (CTimer opt-in).
Coordinator 2026-11-22 (owner "have advisor decide"): any OWNER_PACKET.md of the brief is written as ESCALATE_ADVISOR.md instead.

## Check 1: the 5-B Ego_d misfit in the fan hit blocks
Tools: `tools/odv.sh` / `tools/both.sh` (VC10 SP1 /Od via odall10, fablelib preset, lane headers + $W/hdr), `tools/fit.py` (Ego_d line
boundaries our cumulative offsets do not meet). Ego_d lines: AUTH12 `egod_lines.txt`; sibling `od/bf_egod.txt` (eglines.py).
- Where the movie-dtor sites are. Ego_d 740/741/742 (541 B each, loop 1) and 887/888/889 (561 B each, loop 2) are one macro line each:
  `if (GetHealth(seh.GetScriptThing()) > 0.0f) { Speak; while (IsPerformingScriptTask()) { NewScriptFrame; if (T) return; } if (T) return; }`.
  Each holds two returns inside the movie's scope. The S9 readout (AUTH12 inl/p6s9.json, sites.py) lists the loop-2 movie-dtor
  occurrences k8..k14 with scores 99/798, 131/798, 177/798, 74: three (in-loop return, after-loop return) pairs = the six returns of
  the three loop-2 fan hit blocks, and k14 (74) = the normal scope end of the hit arm (Ego_d 898, outside the fan blocks).
  **So the fan hit blocks hold 6 of the 7 called movie-dtor sites** (loop 1 likewise holds 6 of its 7 expanded ones).
- Ego_d S_BLOCK32 records (eglocals.py): loop-1 movie block 0x51b+1785 (starts at line 738 `InHitCutsceneAlready = true`), loop-2 block
  0x1cec+2135 (starts at 883, the first StartScriptingEntity): both start after the arm's `if (T) return`, consistent with the text.
- The misfit is a property of the shared macro, not of this row: sibling 0x00CF2B70 BowerstoneFan (landed text with Ego_d's `TRetVoid
  _Fail_Val` returns, `od/bfe.cpp`) gives 385/385/385/385/391/397/397 for its seven same-macro lines where Ego_d has
  390/390/390/390/396/402/402: +5 at every one of the 13 blocks of the two functions, whether the block's internal jumps are short
  (sibling) or near (this row). So the instruction is distance-independent: not a jump whose length varies.
- Spellings measured (both functions, every block; d = ours - Ego_d; fm = this row x6, bf = sibling x7):

| spelling | fm d | bf d |
|---|---|---|
| p6d text (baseline) | -5 | -5 |
| `continue;` at the end of the wait-loop body | **0** (all six; row total 11940 = Ego_d 11940) | -3 (short jmp, +2) |
| `(... > 0.0f) == true` | +1 | +1 |
| `while (true == seh.IsPerformingScriptTask())` | -4 | -4 |
| `for (;;) { if (!...) break; ... }` | -4 | -3 |
| `if (!(... > 0.0f)) {} else {...}` | -4 | -4 |
| `if (true) {block}` / `if (true)` round the loop / before the tail | +6 / +10 / +8,+6 | +6 / +6 / +4 |
| `do {block} while (false);` | +3,+1 | +3 |
| `do { if (!...) break; ... } while (true);` | +4,+2 | +4 |
| `... else (void)0;`, `; ;` after the loop | -5 | -5 |
| AUTH12's six (`> 0`, `== true` loop test, `!(<= 0.0f)`, `> 0.0`, `0.0f <`, `!= false`) | -5 / -4 | (not run) |
| `(&seh)->Speak`, NSF after the test, `else {}` round the inner return, scoped Speak/loop | -5 / -3,-7 / -5 / -5 | (fm only) |
- Ego_d types: Speak has two 6-argument virtual overloads (`const char*` / `unsigned long` text), the call shape our text has.
- **Result:** the instruction is not identified. `continue;` fits this row's whole line table (only two unmet boundaries left, 764 / 898,
  both `}` / `else if` line attribution with equal sums), but the sibling's Ego_d contradicts it as the macro's spelling (+2, not +5),
  so it is NOT adopted (Ego_d evidence from one function only, contradicted by the second). A distance-independent 5-B /Od instruction
  that VC7.1 /O2 removes (retail bytes there already equal): an argument-less call of an empty inline (fastcall default -Gr), a
  `mov r32,imm32`, or a far jump. Text unchanged.

## Check 2: is the movie class's dtor compiler-generated? Sizes of the three dtors (odall10, `cal/dtA.cpp`, `cal/dtB.cpp`)
- PDB method lists (llvm-pdbutil on the 4k copies; Ego_d types.txt):
  - `~CScriptGameResourceObjectMovieBase`: **public virtual compiler-generated** in Ego_d (field list 0xAFB0), Ego_r (0x2867) and
    FableWin (0xAFB0). Its ctors (default, copy) and operator= are compiler-generated too.
  - `~CScriptGameResourceObjectBase`: public virtual, user-declared (all three).
  - `~CCountedPointer<CScriptGameResourceObjectMovieBase>`: public, user-declared (all three); `ReleaseData` is a separate member
    (FableWin has its own S_GPROC32, code size 265, not "marked inline"; Ego_d 265 B, 14 lines).
- Sizes (Ego_d PROC records / FableWin code size v VC10 SP1 /Od on the repo headers):

| function | Ego_d | FableWin | VC10, repo default header | VC10, FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA |
|---|---|---|---|---|
| ~CScriptGameResourceObjectMovieBase (implicit) | 75 (no line table) | 75 (has EH) | 75 | 75 |
| ~CScriptGameResourceObjectBase | 28 (1 line) | 28 | 28 | 28 |
| ~CCountedPointer<MovieBase> | **19** (line 71) | 19 | **112** (full release in the dtor) | **19** (`{ ReleaseData(); }`) |
| ReleaseData | 265 | 265 | (none) | 112 (Ego_d adds four 35-B lines: debug checks) |
- So the implicit movie dtor and the GROBase dtor match as modelled; the PImp dtor's original spelling is `~CCountedPointer() { ReleaseData(); }`
  (Ego_d / FableWin 19 B = the existing RELEASEDATA opt-in byte for byte; the repo default puts the release in the dtor, 112 B).

## Check 3: the row's movie-dtor COMDAT v retail 0x006E7B80 (pk.py on the reproduced p6d object `obj/p6d.obj`, DIFFER(6180v6164))
- `??1CScriptGameResourceObjectMovieBase@@UAE@XZ` 25 B v 0x006E7B80 66 B: **NEQ**. Ours: `call ??1?$CCountedPointer<MovieBase>` (the row's
  declared-only specialization, 0x006E7AB0) + GROBase vtable store + `jmp ~CBaseObject`.
- Retail 0x006E7B80 **carries the release** (`mov eax,[esi+0xC]; test; je; dec [eax]; ...; call [eax+4]; push; call 0x00BFE9BC`, then
  `[esi+8]=0, [esi+0xC]=0`, GROBase vtable 0x0126008C, `jmp 0x0099A430`): no call to ReleaseData or to 0x006E7AB0. 0x006E7AB0 likewise
  carries the release (Info-first, then Data=0, Info=0). So the TU whose copy the linker kept had the PImp dtor's body visible and inlined,
  while this row's TU (retail) calls 0x006E7AB0 from its expanded movie dtors.
- Also seen (not this check): the row's `??1CIsThingAlive@NScript@@UAE@XZ` copy is 14 B v owner 0x00E241B0 84 B (NEQ): the inline_depth(0)
  predicate device leaves a pick-any copy that differs from its owner.

## Check 4: falsify "cost <= 13"
- `tools/xrefall.py 0x006E7B80` (xref.py without the 30 cap): **1132 call sites** (`xref_6e7b80.txt`), mapped to rows in `xref_rows.tsv`:
  985 in landed rows; 414 of those in **100 landed rows with stock inline flags and no movie device** (no `// Flags:`, no inline_depth,
  no MOVIEBASE_OUTOFLINE_DTOR / _FORCEINLINE).
- Example: 0x00D53521 in landed 0x00D52E90 CQ_GuildTrainingPreMeleeScript::CTheRealGuildmaster::Main (stock flags, the same declared-only
  `CCountedPointer<MovieBase>` dtor, no EH frame: "thiscall, no EH frame"): `lea ecx,[esp+0x78]; call 0x006E7B80; lea ecx,[esp+0x60];
  call 0x00CDBFB0; ...`, a local movie's scope end in straight-line code. Others: 0x00E44EA0 CBordelloClient::Main (19 sites),
  0x00E64FB0 CChickenMaster::Main (15), 0x00E40E80 CMagicman::Main (14), 0x00DB09E0 CNOVI_AffairMan::Main (12).
- The by-script count above screens only Flags / inline_depth / two movie defines. Stricter screen (`clean_rows.tsv`: includes the shared
  `engine/CScriptGameResourceObjectMovieBase.h`, no Flags, no #pragma, no local movie class, no Destroy/DestroyInline device, no FORCEINLINE
  dtor, no noinline, no movie/CCountedPointer/GROBase opt-in other than _FORCEINLINE_CTOR): **55 landed rows** still call 0x006E7B80.
  (Many small "stock" rows, e.g. 0x00DEB000 / 0x00EFC500 / 0x00EB15C0 / 0x00E0AD40, use explicit Destroy / DestroyInline devices and do
  not count.) Hand-verified, all stock, shared header, implicit dtor, same declared-only PImp dtor, no EH frame:
  - 0x00EB40C0 CMurderTwistAssassin::DoWatchingForTrouble (1601 B, a quarter of this row): retail EXPANDS the movie dtor at 0x00EB418D
    (`lea ecx,[esp+0x34]; call 0x006E7AB0`, GROBase store, ~CBaseObject) and CALLS 0x006E7B80 at 0x00EB429A / 0x00EB42C1 (exit paths
    before `pop edi`): one callee, both decisions in one stock function = scored, not always-inlined.
  - 0x00D44CB0 CQ_GuildTrainingScript::RunArrivalCutscene (944 B): `lea ecx,[esp+0x50]; call 0x006E7B80` at 0x00D4501D.
  - 0x00EEBDF0 CExpression_StealScript::Main (1240 B, "default flags"): calls at 0x00EEC256 / 0x00EEC28A.
  - 0x00D52E90 (11.9 KB) as above.
- **(a) is killed**: a movie dtor whose cost passes the always-inline test at S9 (c1 - 4 <= 9) would be expanded at every site of a stock TU;
  retail calls the COMDAT in straight-line code of stock rows of the same TU (below).
- One TU, three PDBs: Ego_d's compiland `...\FableScripting\Debug\register_scripts.obj` (S_OBJNAME at raw 0x161006, next compiland
  register_defs.obj at 0x7C0806) holds the PROCs of CFanMarker::Main (0x1A350B), CBowerstoneFan::Main, CHookCoastFan::Main,
  CQ_GuildTrainingPreMeleeScript::CTheRealGuildmaster::Main (0x28CE6B), CBordelloClient::Main, CGhostFisherman::Main. FableWin: all in
  Mod 0011 `Debug\register_scripts.obj` (FableScripting, -Od -Gr). Ego_r: Mod 0012 `.\Release\register_scripts.obj`. So every quest
  script Main is one compiland, compiled with one S.
- Landed rows carrying /d2inlS and using CScriptGameResourceObjectMovieBase (all in that TU), 16:
  S15: 0x00CF18D0 OakvaleFan, 0x00CF2B70 BowerstoneFan, 0x00CF3D20 HookCoastFan, 0x00CF68C0 EntranceGuard (+/d2inlT69), 0x00D067A0
  CBCGameMaster, 0x00D13580 CQ_BountyHuntScript::Main, 0x00D17F20 GangDeputy1, 0x00D18DF0 GangDeputy2, 0x00D19BD0 Hostage1,
  0x00D1A740 Hostage2, 0x00D1B200 GangLeader, 0x00D22F30 CKGSheriff, 0x00D2D930 CEndGameJackOfBlades;
  S13: 0x00DE8370 CRansommer (+/d2inlT29), 0x00E17840 CKGChief; S25: 0x00D30030 JackOfBladesBossBattle.
  With one TU and one S, these per-row S values are fits, not retail's flag: queue them as debt (ruling: "either way").

## Forced oracle (readout, NOT a grade; reproduced)
`oracle/`: AUTH10 fv.sh on p6d (stock flags, overlay $W/hdr) forcing only `??1CScriptGameResourceObjectMovieBase` occurrences 8..14 to
expand (`oracle/fm.json`, 7 forced): 6164v6164, mdiff 54 masked (relocation artefacts / jump tables, 13 hunks); gobj.py on
`oracle/p6d.f.obj`: **RELOCATION_MATCH**. Same as AUTH12's.

## Outcome
Check 4 kills (a) by the brief's own test. Step 5 NOT run (no candidate list, no readout, no grade). Per the coordinator's 2026-11-22
message, the packet is `ESCALATE_ADVISOR.md` (not OWNER_PACKET.md).
