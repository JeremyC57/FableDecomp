# Chunk 865 notes -- inline-budget parks landed under the owner's 2026-11-08 decision

Result: 12 of 20 RELOCATION_MATCH (tests PASS under g++ and the lane VC7.1), 8 parked. Every grade is in ATTEMPTS.log
(-> tmp/attempts.log; tags r1..r17). Grades with the `// Flags:` line are made through bin/G.sh (sets FABLE_HDR_OVERLAY=hdr).

## Devices per matched row (payload `flags` = the row's `// Flags:` line)
| Address | Function | Device | Matching band | Notes |
|---|---|---|---|---|
| 0x00CF18D0 | CQ_ArenaHoldingScriptScript::COakvaleFan::Main | flag /d2inlS15 | S15..S20 (S14 = default shape) | source untouched (chunk 804 true model) |
| 0x00CF2B70 | ...::CBowerstoneFan::Main | flag /d2inlS15 | S15..S20 | source untouched |
| 0x00CF3D20 | ...::CHookCoastFan::Main | flag /d2inlS15 | S15..S20 | source untouched |
| 0x00DAC760 | CQ_NewOakValeIntroScript::CNOVI_Guard::Main | flag /d2inlT72 | T63..T72 (T62, T73 differ) | source untouched; T72 = nearest the default T74 |
| 0x00E8EF80 | CV_GhostGrannyNecklaceScript::CGhostGrannySon::Main | flag /d2inlT72 | T55..T72 (T50, T73 differ) | source untouched |
| 0x00D17F20 | CQ_BountyHuntScript::CBH_GangDeputy1::Main | flag /d2inlS15 | S15 (S12/S14 default) | source untouched; hdr/engine/CQ_BountyHuntScript.h |
| 0x00D22F30 | CQ_BreakSiegeScript::CKG_Sheriff::Main | flag /d2inlS15 | S15 (S12/S14 default) | source untouched |
| 0x00D067A0 | CQ_BanditCampBossBattleScript::CBCGameMaster::Main | flag /d2inlS15 | S15, S20 (S12/S14 default) | source untouched; hdr/engine/CQ_BanditCampBossBattleScript.h |
| 0x00EF4B40 | CPersonalScript_EmptyScript::CTestPerson::Main | flag /d2inlT36 | T28..T36 (T27, T37, T38 differ; S2..S20 no effect) | STB ctor made implicit (the OUTOFLINE_CTOR opt-in dropped) |
| 0x00E799F0 | CV_ExposeMayorScript::CEM_Rhodri::Main | `#pragma inline_depth(0)` ... `()` round the row-local `inline ~CIsThingConscious() { }` | no flag: S5/S15/S20, T30/T40/T55/T60/T65/T70/T80/T100 all differ | device 3; hdr/engine/CV_ExposeMayorScript.h |
| 0x00D19BD0 | CQ_BountyHuntScript::CBH_Hostage1::Main | flag /d2inlS15 + row-local `__forceinline` STB operator= (`FABLE_INLINE_BUDGET_FORCEINLINE`, `inline` outside _MSC_VER) | operator= called under S15..S30, T60..T150, S15+T60/80/90/100/120/150 | device 2+3; OUTOFLINE_ASSIGN opt-in declares it; hdr CQ_BountyHuntScript.h |
| 0x0083F650 | LHNetworkLib::LHHttpHeaders::GetHeader(const string&) | flag /d2inlS11 + the landed __find_if 0x00841900 body in the TU (auto_inline(off)) | S11..S20 (S10 default), also T60 (T50/T70 not) | model = the landed LHHttp2::GetHeader TU 0x00840B60 (which already carries this body); LEDGER MIS-CUT, see below |

Same retail TU, same N: the three arena fans (CQ_ArenaHoldingScriptScript) S15; the BountyHunt TU rows 0x00D17F20 / 0x00D19BD0 S15
(the parked 0x00D18DF0 is also S15 up to its one jne); CQ_NewOakValeIntroScript: 0x00DAC760 T72 (the parked Init 0x00DAADD0 of the
same TU is moved by no knob). Every other matched row is alone in its TU. Per EXTRA_RULES the flag is per row (catalog CompilerFlags);
`tu_flag_check.py` was not run (the brief says no TU check is needed) -- the coordinator may want to confirm S15 against the landed
rows of the arena / BountyHunt TUs before choosing one N per TU.

Choice of N: for /d2inlS (default 9) the smallest matching value; for /d2inlT (default 74) the value nearest the default. Any value
inside a band is byte-identical.

## Coordinator must know
- **0x0083F650 ledger mis-cut**: the catalog / chunk oracle row is 442 bytes, but VC7.1 emits an `int3` after the trailing noreturn
  `call abort` and retail has `cc` at 0x0083F80A: the function is 443 bytes. oracle.tsv carries the 443-byte row (finish.py appends the
  byte); against the 442-byte ledger the grader reports DIFFER(443v442) with the int3 as the only line. The catalog row needs the
  one-byte extension before vland can land it. The catalog name "ProcessTextWithRegex" is descriptive (NAME_FIXES.tsv).
- **Header overlay** (hdr/engine/, all opt-in blocks only, copied unchanged from the chunks that wrote them; default paths identical to
  rebuild/include): CQ_BountyHuntScript.h (chunk 833: GANGDEPUTY1 / HOSTAGE1 / HOSTAGE2 opt-ins; GANGDEPUTY2 is already in the repo copy)
  for 0x00D17F20 / 0x00D19BD0 (and the parked 0x00D18DF0); CV_ExposeMayorScript.h (chunk 836 via 856) for 0x00E799F0;
  CQ_BanditCampBossBattleScript.h (chunk 852) for 0x00D067A0; NUISystem_CComponent.h (chunk 862) for the parked 0x00531EC0.
  gall.sh / q.py without FABLE_HDR_OVERLAY report SRC_FAIL on those four rows.
- 0x00EF4B40 no longer defines FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR (the implicit ctor is retail's;
  the flag then calls seh_hero's copy and expands seh_me's).
- 0x00D19BD0 defines FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN (existing opt-in) and gives the declaration
  a row-local body under a `_MSC_VER`-guarded forceinline macro; outside MSVC it is a plain `inline` definition.
- Catalog names fixed in NAME_FIXES.tsv: 0x00DAADD0, 0x00D067A0, 0x00531EC0, 0x0083F650 (plus the NScript namespace on four old rows).
- Tests: test/<addr>.cpp for all 12 (0x0083F650's is new), each prints `<addr>_TEST PASS`; tall.sh (g++) and tall.py (VC7.1) both PASS.

## Parked rows (differ/<addr>.cpp, one line each; details in STATUS.tsv)
- 0x00D18DF0 CBH_GangDeputy2::Main -- under /d2inlS15 byte-identical except ONE jne displacement (0x00D195CF -> retail 0x00D19977, ours
  0x00D19A8B: two identical ~deputy_pos+~seh_me tails). A c2 tail-merge pick: our compiler sends every conditional return to the last
  identical tail, retail sent this one to the copy inside the speech pad. Not an inline decision; do-while(0), brace scopes,
  `if (!T) { ... }`, dropping R1 or duplicating R2 leave it. 14 grades. If the coordinator accepts a 2-byte relocation-like residue it
  is otherwise landable with S15 + the FORCEINLINE_CTOR / implicit-dtor model (the differ copy).
- 0x00531EC0 NUISystem::CComponent::Update -- /d2inlT52..T56 gives retail's ConvertVector 8-call / 4-expand split, but md rises
  (654: frame 0x84 v 0x94, list-head spill); the called ~_List_base 0x0042ABCA for the THIS_INSTANCE CDeletion local is reached by no
  S/T value, and an in-body inline_depth(1)/(2) region is a no-op (0 outlines the whole dtor). Two residues; 9 grades.
- 0x00DAADD0 CQ_NewOakValeIntroScript::Init -- _Construct<CCharString> expanded at all 42 push_backs (retail 22 + 20 calls): nlw,
  fcsw75..400 (75+ outlines every site), flcsw100..400, fcsa100/400 unchanged; in-body inline_depth(1)/(2) round sites 23..42 no-op,
  (0) outlines push_back too. Behaves like a per-callee expansion count that no allowed device controls. 16 grades.
- 0x00E03F70 CMagicBarrier::Main -- /d2inlT60 removes the region-2 ~CScriptThing split (md 120 -> 68) but seven inline-angle `fstp`
  slots sunk past the next null test remain (IR order). Old local-model source. 3 grades.
- 0x004CE1B0, 0x00DC08C0, 0x00E2EAC0, 0x00CDDCB0 -- baseline regraded only: their residues are register roles / frame slots / a dead
  store / un-merged cleanup copies, not inline decisions, and the sources are old local-model files with inline_depth regions and
  forceinline helpers; they belong to a register/frame chunk, not to this one.

## Grades
167 logged grade lines in ATTEMPTS.log (a few are duplicates of the same probe); per row in STATUS.tsv. No address was graded twice at once through q.py after the one race noted in LESSONS.
