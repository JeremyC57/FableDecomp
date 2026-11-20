# 00e9e420 (p25B, Account B, 2026-11-20): NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E9E420 | CBriarRose::Main() | 5915 | RELOCATION_MATCH (no flags) | PASS (12 targeted + 20,000 random runs) | 56 live: 55 killed (47 by mut2, 8 relocation-only mutants by running the test on their objects), 1 link fail; fitted line reported separately |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, with one warning:
`WARN RELOC_ALIAS: _rand -> 00bfeb16 is the import thunk 'MSVCR71.DLL::rand'`.
- The binding is right. Retail calls rand through its import thunk 0x00BFEB16, as the landed GuildTraining Mains do (`int __cdecl rand(); // 0x00BFEB16`).
- The warning is a tool gap. `reloc_identity._crt_claim` reads `_rand` as a C symbol, but `CRT_TOKENS` has no `rand`, so it finds no claim and reports an alias.
- Suggested fix (not mine to make): add `rand` (and `srand`) to `CRT_TOKENS`.
  GOTCHAS line: "RELOC_ALIAS on `_rand` is a CRT_TOKENS gap, not a wrong binding."

Other properties:
- Purity: plain C++. No asm, no pragma, no flag.
- Line 2 `// Inline-budget:` names every inline device and opt-in, each with its ablation grade (below).
- One `// Inferred: redundant second read --` fitted statement, admitted by the coordinator's ruling of 2026-11-20 under the owner's delegation, on the 0x00E97070 terms.
- No header change and no newly flagged rows. engine/CV_IntroductionToTrophiesScript.h is in the repo, and the row needs nothing new from it.

## What the parked diagnosis missed
The parked chunk 828 draft graded DIFFER(6035v5915), with md 392. It had three separate causes.

1. **Loop frequency.**
   - The main loop must be the generated endless form `for (;!false;Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return ..; ... }`.
   - c2 scores each inline site as cost + k/f, where f is its block-frequency estimate.
   - Each `!false` test halves the frequency of every site in the loop. That moves ~CTimer, ~CScriptThing and the movie dtor at the exit pads to retail's split: expanded at some pads, called at others (0x00CD4470 / 0x004AA840 / 0x006E7B80).
   - With `for (;;..)` and everything else final: DIFFER(5945v5915).
2. **Prologue colouring.**
   - `AnimTimer=pGlobalScriptDef->TrophiesBriarRoseAnimTime+rand()%pGlobalScriptDef->TrophiesBriarRoseAnimTimeVar;`
   - VC evaluates the right operand first, so it loads Var before `rand()` and reloads pGlobalScriptDef after it, as retail does.
   - Written as `rand()%Var+Time`, both loads are CSE'd before the call. That recolours the prologue: DIFFER(5893v5915).
3. **The trophy test's register.** This is the fitted read, described next.

## The fitted read (mechanism; paragraph for docs/journal/FINDINGS_LOG.md)
2026-11-20 (p25B, 0x00E9E420) The register of a bool test's byte is a register-colour signal for how many reads it has.

Retail tests DoneBriarTrophySpeech in al (`mov al,[eax+49h]; test al,al; jne`, 0x00E9F3FE). The `mov ecx,esi` that both of its successors need is
hoisted all the way up, above the intro test's jne (0x00E9F234). We had `mov cl` / `test cl` and `mov ecx,esi` after each jne: DIFFER(5923v5915),
187 diff lines. Everything else in those lines is jump offsets.

How VC7.1 picks the byte register:
- **Read once.** The flag is a code-generation temporary. It takes the first free of eax, ecx, edx. The base register counts as held even at its
  last use. Here eax is the ParentClass pointer, a global candidate carried from the sniffy test, so the byte gets cl. Retail shows the same rule
  elsewhere: `mov al,[ecx+4Bh]` at 0x00E9EDA9, `mov al,[edx+4Dh]` at 0x00E9F718.
- **Read twice, with no call or store between.** The flag is a global candidate. The pointer dies at the load, so eax is allowed, and the flag gets
  al. With cl free, `mov ecx,esi` is scheduled above the load. Both successors of the intro test then begin with it, so it is hoisted again.

What the micro TUs show (probe/):
- `m1_base.cpp` gives cl.
- `m1_eqtrue_survives.cpp` gives al, but leaves a `cmp al,1`.
- `m1_dup_arms.cpp` gives al, and the second test is gone: retail's exact shape.
- `m1_folded_else_if.cpp` gives cl. A second read that c2 can fold early (`else if (b)` after `!b`, `!b&&!b`, `if(b){}`) has no effect.

So the read must sit before the first call in the blather arm, and its two outcomes must merge so that it leaves no code. The two identical arms of
`ParentClass.DoneBriarTrophySpeech==true ? IsActiveThreadTerminating() : IsActiveThreadTerminating()` are cross-jumped, so the second test is removed.

Other forms graded in the full row:

| form | grade |
|---|---|
| the whole blather arm duplicated (`probe/row_dup_whole_arm.cpp`) | DIFFER(6195v5915): its loops stop the merge |
| only the T-check duplicated as an if/else (`probe/row_dup_tcheck_ifelse.cpp`) | DIFFER(5915v5915), 104 lines: two return pads, no merge |
| the ternary in the trophy arm instead | cl kept |
| `else if (b==true || true)` / `else if (b || true)` | cl kept |
| `T() || (b==true && false)` | cl kept |
| `else if (b==true || !b)` | DIFFER(5931v5915): the test survives |

**Fitted statement** (src line 273, the blather arm's first terminate check):
`if (ParentClass.DoneBriarTrophySpeech==true ? IsActiveThreadTerminating() : IsActiveThreadTerminating()) return _Fail_Val.Return();`
- It uses only a member the function already reads and a call it already makes.
- **The bytes fix the read's position (before the blather arm's first call) and the merge of its outcomes, not its spelling.**
- Spellings, per the ruling:

  | spelling | grade |
  |---|---|
  | bare `ParentClass.DoneBriarTrophySpeech ? ..` | DIFFER(5923v5915): c2 knows the flag is non-zero there and folds the test |
  | `==true` | RELOCATION_MATCH: the comparison the generated scripts use (landed 0x00ECF7F0 `LaughingWomanKilled==true`, 0x00D58490 `MeleeRepeating == true`) |
  | `>0` | RELOCATION_MATCH |

- **Ego_d: retail-only, not in Ego_d.**
  - Ego_d.pdb's line table for this proc has 141 records, with statements merged on macro lines; t1 has about 240 lines. It is a later revision.
  - At VC7.1 /Od the fitted line is 134 bytes, against 87 for a plain terminate check in the talk block (`odl2.py`): a surplus of 47 bytes.
  - No Ego_d line in the talk region (688..761) is a terminate-check line with that surplus.
  - Ego_d does not veto the read (ruling); it gives no second source for it.

## Ladder and per-device ablation (ablation/, each removed in turn from the final source; grades.txt)

| device removed | grade without it |
|---|---|
| the fitted read (`// Inferred:`), as a plain T-check | DIFFER(5923v5915), 187 diff lines |
| the fitted read, bare `?:` (no `==true`) | DIFFER(5923v5915) |
| the fitted read, `>0` for `==true` | RELOCATION_MATCH (spelling not constrained) |
| FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR | DIFFER(5859v5915) |
| FABLE_RETAIL_STLP_MAP | DIFFER(5947v5915) |
| row-local `inline CTimer::CTimer()` | DIFFER(5804v5915) |
| row-local `inline CTimer::~CTimer()` | DIFFER(5656v5915) |
| row-local `FABLE_FORCEINLINE CTimer::operator=(long)`, as plain inline | DIFFER(5901v5915): the AnimTimer site is called |
| the same, dropped | DIFFER(5811v5915) |
| row-local `inline CTimer::GetTime()` | DIFFER(5842v5915); `FABLE_FORCEINLINE` gives the same bytes, so the plain form is kept |
| row-local `inline CScriptThing::CScriptThing()` | DIFFER(5881v5915) |
| row-local `inline CScriptThing::~CScriptThing()` | DIFFER(5769v5915) |
| row-local `inline CScriptThing::IsAlive()` | DIFFER(5925v5915) |
| `inline CIsThingAlive` ctor (the landed DemonDoors / Witch Mains' contract body) | DIFFER(5891v5915) |
| `for (;!false;..)` -> `for (;;..)` (text, not a device) | DIFFER(5945v5915) |
| AnimTimer operand order `rand()%Var+Time` (text) | DIFFER(5893v5915) |

Dropped from the draft because they change no byte (ablation/e9e_plus_assign.cpp adds them back: RELOCATION_MATCH):
- the ScriptedThingBase assign OUTOFLINE opt-in;
- the movie ctor / dtor OUTOFLINE opt-ins;
- the two `template<> std::map<..>::map()` / `~map()` out-of-line declarations.

No flag and no pragma are used. The row matches at default flags, so the flag step of the ladder had nothing to replace.

## Source shape
The chunk 828 draft's statement order, plus the three changes above.

Names from Ego_d.pdb's locals (egod_locals_all.py, `NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main`):
- function scope: deedTimer (was OpinionDeedTimer), standStillTimer (UnusedTimer), sehMe (seh_me), waypoint (UnusedThing), _Fail_Val, LookAtMe;
- blocks:
  - {speech_movie, PauseMyEntitiesPlease};
  - {animIdx} (was animation);
  - {villager (watcher), {HLStyleConversationIndex (conversation)}} x2;
  - {speech_movie, PauseMyEntitiesPlease, {seh_hero, mymap} x2};
  - {speech_movie, PauseMyEntitiesPlease}.
- The renames change no byte.
- Ego_d also has `counter`, `initialise` and a {position_name} block. Retail's code has no use for them; Ego_d is a later revision.
- The declaration order deedTimer, waypoint, standStillTimer is retail's frame and prologue.

## Headers
None changed. These existing shared headers carry the layout:
- engine/CV_IntroductionToTrophiesScript.h (in the repo):
  - ParentClass +0x14;
  - AnimTimer +0x1C;
  - the parent flags +0x48..+0x62.
- engine/CScriptDef.h through pGlobalScriptDef: +0x69C, +0x6A8, +0x6AC, +0x6C4, +0x6C8, +0x6D4.

The test runs the row against those offsets. BriarRoseOpinion stays the header's default empty enum; the row writes it through casts, as the draft did.
No newly flagged rows.

## Test (test/00e9e420.cpp)
Links the real row object. These run as compiled:
- the row body;
- the expanded CTimer / CScriptThing / CIsThingAlive bodies;
- the sehMe / seh_hero / movie / pause bodies;
- the ScriptedThingBase and movie dtor COMDATs;
- the cast map's ctor, operator[] (the lower_bound walk) and dtor (_M_erase);
- the handle assignment.

How the externals are bound:
- Every external goes by /alternatename to a __fastcall stub, or else to a trap (the binding block is generated from the object's undefined symbols).
- The map's `insert_unique` is a stub that links a real node into the row's own tree. The row's `_M_erase` destroys and frees it.
- Things carry real counted-pointer info blocks, and their implementations a vtable with IsAlive at slot 75.
- pDSTGame's timers sit on a separate object.

What the inputs script:
- terminate positions;
- IsAlive per thing (LookAtMe, hero, villager);
- StartScriptingEntity results for sehMe and the hero;
- IsPerformingScriptTask;
- health;
- GetTimer values (0 / 7 / -3);
- rand values (all 8 animation arms, and the anim timer);
- the two distance tests;
- talked-to;
- every initial ParentClass flag, the hit count and the opinion states;
- GDefaultToUsingPhysicsInCutsceneAnimations.

What the test checks:
- each run, event by event, against an independent model (Model()), including:
  - the evaluation and destruction order of every string temporary;
  - the cast map's key temporaries, inserted pairs, assignment and erase order;
  - the destruction order of every live local on each early return;
- every argument, including:
  - the 7 PlayAnimation bools (the OUT animations' `true`) and the console flag;
  - the conversation index and its things;
  - the cast map's contents at RunCutsceneMacro ({HERO: seh_hero, ROSE: sehMe}), with CutsceneStarted set;
  - the deed type 56;
- the final value of every ParentClass field, and no other store;
- the AnimTimer / deedTimer values set;
- Me and the hero untouched;
- every string, thing, info block, handle and timer released, and the pause balanced.

One order is unspecified by C++. VC builds AddLineToConversation's CCharString argument before its `Game->GetHero()` argument, and the model follows VC.

What the 20,000 random runs reach:

| path | runs |
|---|---|
| opinion source switches | 9,843 |
| hit warning | 2,241 |
| animation arms 0..7 | 4,420 / 4,333 / 4,355 / 4,320 / 4,277 / 4,357 / 4,340 / 4,355 |
| approach 1 / 2 (conversation started) | 2,281 / 2,296 (2,301) |
| sniffy | 12,830 |
| intro cut scene | 3,737 |
| trophy cut scene | 3,052 |
| blather, flag byte 1 | 11,137 |
| blather, flag byte 2 | 2,070 |
| attack speech | 3,430 |
| opinion deed | 67,931 |
| sehMe release | 39,414 |

**The fitted read's arms.**
- With a canonical bool (0 / 1) the blather head is only reached with DoneBriarTrophySpeech == 1, so only the `==true` arm runs. The `:` arm is dead
  for every value the game writes.
- The test also drives the non-canonical flag byte 2 (2,070 runs). There the second arm runs, and it behaves the same, as retail's merged bytes do.

## Mutants (mutants/)
**Live code.** 56 one-decision mutants (mutants/m9e.tsv, m9e.log). mut2 killed 47.
- SAME_CODE (8): literal swaps that change only a relocation target (mut2 compares bytes):
  - M_TrophiesLookAtMe;
  - LOUD -> QUIET;
  - WARNING / BLATHER texts;
  - ST_IDLE_SUBTLE, TALK_IDLE_03;
  - APPROACH_01;
  - CS_TROPHIES_INTRO -> TROPHY.

  All 8 were compiled and run against the test (mutants/sc/: sources, list.txt, results.txt), and all 8 FAIL. Every one is RELOCATION_MATCH by
  bytes, so only the test can catch them.
- LINK_FAIL (1): deleting `SetPredicate(CIsThingAlive(Me));` stops the object from emitting the CIsThingAlive vtable that the test binds (as in
  0x00E72630 / 0x00ECE7F0). The test's SetPred check covers this statement.

**Fitted line** (mutants/m9edead.tsv, m9edead.log; line 273):

| mutant | behaviour test | byte grade |
|---|---|---|
| first arm -> `false` | KILLED (flag byte 1) | — |
| second arm -> `false` | KILLED, only by the flag-byte-2 runs. Under canonical bools this arm is unreachable, and the mutant would survive the test as unreachable, not killed | DIFFER(5931v5915) |
| the whole read removed (plain T-check) | SURVIVED (same behaviour) | DIFFER(5923v5915) |
| `==true` -> `>0` | SAME_CODE (RELOCATION_MATCH: spelling not constrained) | — |
| DoneBriarTrophySpeech -> DoneBriarIntroSpeech | SURVIVED (both arms identical) | DIFFER(5931v5915): `mov dl` |

## NAME_FIXES
None.
- Ego_r.pdb / FableWin.pdb confirm the leaf and the Game / pDSTGame slots named in the header comments.
- rand is the import thunk 0x00BFEB16.
