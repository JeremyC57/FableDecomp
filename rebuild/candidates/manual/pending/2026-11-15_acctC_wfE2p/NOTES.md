# wfE2 NOTES (acctC lane, 2026-11-14)

## 0x00D0F640 NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre -- parked (first pass)
- Identity: Ego_r 0x00D13E10 + reloc_bindings 0x00D0F640 `?CheckForSecondAreaMassacre@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ`
  (thread spawned by the landed Main 0x00D038E0). Extent 2101 confirmed (last insn `jmp 0xd0fe48` at 0x00D0FE73).
- Anniversary PDB locals: followers (one vector for both lists), KilledCreatures, AllBandits, NewLeader, seh_hero, speech_movie, mymap.
- Best: differ/00d0f640.cpp = earlier draft (2131v2101, fitted folded operands `&& true` / `|| false`).
- Natural shape (TRetVoid, `for(;!false;NSF){ if(T) return; ...}` as sibling 0x00D0A830) = 2424 (tmp/d0_v1.cpp): the rotated
  loop-top T check IS retail's (retail's condition exit returns without a 2nd T call: 0x00D0F9B1; the draft's `!T && true` condition
  calls T twice there -- wrong control flow). for(;;NSF) 2456, while(true) 2424, kill loops as `while` 1622 (everything called),
  bandit loop as `while` 2177.
- Inline-decision analysis (tools/inl.sh + tools/dk.sh): retail per-site = V E,C,E,E,C,E,C | movie ctor E | map ctor E | ~CScriptThing C |
  map dtor C | movie dtor E | V C x4 | ~CScriptThing E,E | V C x2 | V E. Scores at equal f: map ctor < ~CScriptThing < ~vector.
  With T in (71,76], map ctor E at the cut-scene block (f_c) and ~CScriptThing E / ~vector C in the leader block (f_l) need
  f_l/f_c <= ~1.7; a `for` bandit loop gives 2.5, a `while` 0.25 -> no uniform-threshold source with either loop spelling.
  The draft has every decision right except the map ctor (called, sc 81 at f 0.1717).
- Next: find a shape with f_l/f_c in (1, 1.7) (e.g. leader block under a real condition), or remove `&& true` (f_c 0.34: map ctor E)
  and outline the bandit-loop / final-return vector dtors with scoped inline_depth(0) (they share the final return with the S E sites:
  needs the 0x00D0A830 goto-Terminate split).

## 0x00E33EA0 NScript::CV_ArcheryCompetitionScript::CAC_Owner::Main -- MATCHED (RELOCATION_MATCH, vland WIN, test PASS)
- Identity: vtable slot .rdata 0x012E30FC (only reference); Ego_r 0x00E83BB0 + FableWin 0x039381B0 ?Main@CAC_Owner@...@@EAEXXZ. Extent 9228 confirmed.
- From the earlier draft (9303v9228, 323 lines) in four steps (6 grades): (1) header overlay opt-in FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_OWNERMAIN
  (today's header lacks ResetRandomSpeechTime / WatchForAttack / DoTimesUp / GetRandomSpeech; SRC_FAIL without it) + repo DOCOUNTDOWN;
  (2) landed predicate + spawn idiom: PREDICATEBASE_NOVTABLE + CSCRIPTTHING_IMPLICIT_COPY, inline_depth(0) round the row-local
  CIsThingAlive ctor and a CSpawnedFunc1<CV_ArcheryCompetitionScript,CScriptThing> ctor specialization (as 0x00E2FF00): 9268, calls equal;
  (3) the five random prizes as `{ unsigned long idx = rand() % X.size(); GiveHeroObject(X[idx]...); }` (retail reloads pGlobalScriptDef
  after rand, 0x00E35BF0; inline indexing keeps it in ebp): 9228v9228, 2 hunks; (4) EntitySetFacingAngleTowardsThing(Game->GetHero(), Me,
  false) at the two talk/speech sites (the draft had the arguments swapped -- a behaviour bug, push order at 0x00E34299 / 0x00E3472E).
- No flags. Inline-budget line for the inline_depth(0) region. FITTED: the idx locals (spelling of a retail-evidenced evaluation order).
- vland warnings only: RELOC_NAME CDefString::operator CCharString 0x00415D70 label, RELOC_ALIAS _rand import thunk.

## 0x00E5FD40 NScript::CV_BodyGuardScript::CBodyGuard::Main -- near miss DIFFER(9381v9381), 8 hunks (differ/00e5fd40.cpp)
- Extent 9381 confirmed (ret 0x00E621E4, int3 filler, SetHenchmanAsHired 0x00E621F0 follows). Name: draft's proof (vtable / Ego_r).
- Draft 9471v9381; `for (; !false; Game->NewScriptFrame()) { BG_T; ...}` (0x00D0A830 idiom, retail top T 0x00E5FE40 / bottom T 0x00E61F6B
  sharing exit 0x00E61F78) -> 9381v9381 with 8 hunks. `for(;!T;NSF)` 9813, `for(;;NSF){T}` 9843.
- Residue = one cross-jump direction in the anger-warning chain (see differ line 2). Playbook: the LAST edge sharing a tail absorbs the
  others; retail behaves as if LOW's edge were last (reverse source order). Tried: bool local, nested else, empty else (no change);
  MEDIUM moved last 9365 (5 hunks: lever confirmed, other layout moves). Next: find a natural spelling that orders LOW's join edge last.

## 0x00D973D0 NScript::CQ_HobbeCaveScript::CBanditSacrifice::Main -- parked DIFFER(9964v9821), 73 hunks (differ/00d973d0.cpp)
- Extent 9821 confirmed (ret 0x00D99A2C, int3 filler). Name from the draft (vtable / Ego_r).
- Key retail fact: ~CTimer is never expanded (0x00CD4470 called twice, both in the shared final chain 0x00D999FE/0x00D99A10 that
  26 + 14 exits jump to); ours expands it at f=1.0 return sites (inline trace: returns inside the Nymph `for(;;)` loop and the
  draft's `if (!T) continue; return;` bottom are scored at f=1.0, sc 14) and then the pads cannot cross-jump into the final chain.
- Best so far: draft + `#pragma inline_depth(0)` round the Nymph-bottom `return` = 9964 (73 hunks). Natural `while (!Nymph.IsDead())`
  removes every expanded timer and gives ONE ret (9915) but lays the `if (!sacrificed)` block out cold -- the most promising lead:
  find why the while form makes that arm cold (retail keeps it inline at 0x00D98596), e.g. sacrificed tested via a different spelling.
- Also: retail CALLS the by-value CScriptThing copy for SetIsPushableByHero(Me, true) at 0x00D98BDD (per-site inline decision).
- (later) bodyguard: anger chain fully reversed (`if (>=20) { if (>=45) { if (<70) HIGH } else MEDIUM } else LOW`) 9365 / 7 hunks: the
  tests get laid out consecutively -- not retail. The playbook edge-order model predicts retail's merge from a nested-label join
  (inner joins first, LOW's edge last), but VC7.1 gives ours source-order edges for every brace spelling tried.

## Grades used (approx.): 0x00D0F640 9 + 3 traces; 0x00E33EA0 7; 0x00E5FD40 10; 0x00D973D0 9 + 3 traces.
## Header changes: one overlay opt-in (engine/CV_ArcheryCompetitionScript.h FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_OWNERMAIN, asserts pinned).
## Names: all four row names proven (Ego_r + FableWin / vtable / reloc_bindings); no descriptive names introduced.

## (successor) 0x00E5FD40 anger chain -- mini-probe findings (probe/v_*.cpp, tools/pc.py compiles a probe with BASE_FLAGS)
- A 30-line probe (T = `if (term()) return`, S temp with dtor, same chain + 3-arm health block in a for(;;)) reproduces OUR result
  exactly (HIGH keeps `T; st(gt()+30)`, LOW/MED `jmp` into it, LOW/MED calls not merged). Invariant under: bool locals per arm,
  `if (!cm) {} else`, do-while(0) round any arm or the chain, `break` chains, empty else, `else if (>= 70) ; else HIGH`, nested else.
- ONLY a reversed chain (LOW lexically last: `if (>= 20) {...} else LOW`, also `switch` with LOW as case 0 + default) gives retail's
  MERGE pattern (LOW keeps, MED jumps into LOW's dtor call, HIGH into LOW's tail) -- but then LOW is laid out last (`jl LOW`), full
  function 9365 (6 hunks, tmp/bgr_r5.cpp). Retail = that merge + base layout (LOW falls after test20, M pulled up after LOW, MED/HIGH after
  the crit arm's jmp): consistent with "base IL order + LOW keeps, then M pulled after LOW because HIGH no longer falls into it".
- `if (hit) {...} else { h9(); }` (all arms end in jmp): LOW and MED keep separate tails and HIGH merges into MED -- the absorber is not
  simply "last edge"; the rule depends on which pred falls through into the join.
- Scheduling `mov bl,al ; lea ecx` (retail LOW/MED) v `lea ; mov bl,al` (HIGH, ours) is a CONSEQUENCE of the merge (scheduler runs on
  the split block), not a source fact.

## (successor) 0x00D973D0 -- near miss DIFFER(9821v9821), every instruction equal after masking, 43 frame-slot hunks (differ/00d973d0.cpp)
- 9964 -> 9805 by ONE change: the Nymph loop in the landed-sibling idiom `HC_T; bool sacrificed = false; for (; !false;
  Game->NewScriptFrame()) { HC_T; if (Game->GetThingWithScriptName("Nymph").IsDead()) break; ... }` (c2 emits the body-top T once
  before the loop, 0x00D9851F, and again after the NSF at the bottom, 0x00D98B99 `je top ; jmp exit`): no T return is at f=1.0, no
  ~CTimer is expanded, all inline_depth(0) regions in the body became unnecessary (removed; only the CIsThingAlive ctor/dtor ones remain).
- Then three source facts from the normalised diff (9805 -> 9821, 0 normalised lines): `while (!Game->IsRegionLoaded("GreatwoodCaves"))`
  (retail materialises !result with neg/sbb/inc across the temp dtor, 0x00D98DE3 / 0x00D98E2C; the draft looped while the region WAS
  loaded -- behaviour bug), `sacrificed = true;` before `RemoveQuestInfoElement` (store scheduled before the call, 0x00D985AE),
  `if (PreviousAIState != CurrentAIState && ...)` (cmp [esi+0x20],eax 0x00D9839E).
- Left: frame slots. Packer probe (hand885 tools/pk/pk.py via tools/pkw.sh): size-1 order sacrificed w4 (opens slot0), takeShare w4,
  six HIT temps w3 -> slot0, Nymph-loop HIT temp w3 (conflicts with sacrificed) opens slot1, letOut w2 joins slot1, IsDead temp w2 joins
  slot1. Retail has letOut in slot0 (F13; init 0x00D9787D [esp+0x1F] at push depth 12, reload 0x00D97B9A) and slot1 = the two Nymph temps
  (F3F, above seh_me F2C). So retail's letOut is ordered before the Nymph HIT temp: IL weight >= 3 (ours 2 = init store + reload).
  Tried (no change, weight stays 2): declaration moved (before InCutscene / after FadeScreenIn / at function top + reset), `letOut == false`,
  `if (!letOut) while`, `if (!letOut) do..while`, `for(;;){if (letOut) break;` (9822), explicit else `letOut = letOut` (optimised away).
  Next: find which IL reference retail had that is removed after packing (cross-jumped duplicate or post-RA peephole).
- Locals renamed to the Anniversary PDB names (Trapped, AIComments, LookingForHobbeMainChamber, TempEffect, SacrificePos, MyJames,
  seh_Hero, seh_James, speech_movie, ExitMarker, local_position); byte-identical.

## (successor) 0x00D0F640 CheckForSecondAreaMassacre -- MATCHED (RELOCATION_MATCH, vland WIN, test PASS), src/00d0f640.cpp, flags /d2inlT59
- Started from the NATURAL shape (TRetVoid, `for (; !false; Game->NewScriptFrame()) { if (T) return; ...}`, plain `if (Gate3Open) break;`,
  no folded operands), not the fitted draft. With natural frequencies every retail-EXPANDED site is expanded; the residue was only
  sites retail CALLS. Closed with the owner's inline-budget devices (EXTRA_RULES 2026-11-08), in steps measured by grade:
  1. ~CScriptThing: FABLE_FORCEINLINE on the row-local definition (both leader sites E at any usable threshold) + `#pragma inline_depth(0)`
     round the one cut-scene statement `Game->RemoveThing(Game->GetThingWithScriptName("Gate3Guard"), false, true);` (called, 0x00D0FC0B);
     a forceinline callee inside an inline_depth(0) region is called.
  2. bandit loop spelled `unsigned long i = 0; while (i < AllBandits.size()) { ...; i++; }` (FITTED; same rotated code as `for`, but the
     post-loop block frequency drops from 2.06 to ~0.3 so the leader-block ~vector<CScriptThing> dtors are called while ~vector<unsigned long>
     stays expanded -- they share each return, so an inline_depth(0) region there cannot split them).
  3. `/d2inlT59` (smallest N: T58 calls the followers-loop exits sc 58 that retail expands; T62 expands the cast-map dtor sc 61): calls
     the summoned-loop inner T exit and the cut-scene entry T exit (f 0.824, sc 65) and the map dtor, as retail (shared chain 0x00D0FE39).
  Intermediate: inline_depth(0) on those T exits also called ~vector<unsigned long> (wrong); inline_depth(1..3) behaves like no pragma.
  A nested scope round the leader creation changed the frame (12 bytes); without it the frame is retail's 0x74.
- Locals renamed to the Anniversary PDB names. No header changes. vland warning only RELOC_NAME (Rb_tree dtor v label CCutsceneCastMap::Destroy 0x00CDBFB0).
- (successor, full function) first-test spellings `!(t >= 20)`, `20 > t`, both tests negated: byte-identical (8 hunks). Reversed chains
  r2 / r5 9365 (6 hunks), r3 9365 (7). Two models fit retail's layout and neither was produced: (A) MED/HIGH placed out of line BEFORE
  cross-jumping (then LOW is M's fall-through pred and keeps the tail, MED merges at the dtor call, HIGH at the tail -- the probe shows
  this merge is exactly what a fall-through LOW gets); (B) LOW keeps by edge order, then M is pulled up after LOW. Next: find what makes
  c2 lay a branch out of line (the D973D0 `while` variant laid a loop body after the exit code) -- e.g. the hit test or the hired loop
  spelled so the `t >= 20` arm is colder.

## Successor totals (2026-11-15)
- Grades: 0x00D0F640 ~40 full grades + 2 inline traces + 1 packer probe (MATCHED); 0x00D973D0 ~25 + 2 packer probes (near miss, frame
  slots only); 0x00E5FD40 ~12 full + ~25 mini probes (near miss, one cross-jump). 0x00E33EA0 untouched (first agent's match).
- Header changes: none (overlay unchanged). Flags: 0x00D0F640 /d2inlT59. New devices: FABLE_FORCEINLINE row-local ~CScriptThing and one
  scoped inline_depth(0) statement in 0x00D0F640 (Inline-budget line on line 2).
- Names: no new callee / global names; locals of 0x00D0F640 and 0x00D973D0 renamed to the Anniversary PDB local names.
- Tools added: tools/g.sh (fg wrapper, absolute paths), tools/pc.py (mini-probe compile + objdump), tools/pkw.sh / pka.sh (hand885
  frame-packer probe: item order, weights, slot joins), tools/rr.py (retail address-range dump).

## (third author) 0x00E5FD40 CBodyGuard::Main -- MATCHED (RELOCATION_MATCH, vland WIN, test PASS), src/00e5fd40.cpp, no flags
- The "cross-jump direction" was a SOURCE fact: the original has ONE `T; AngerTimer += 30` behind a shared flag, not three copies.
  `bool commented = false; if (<20) { T; commented = COMMENT(LOW); } else if (<45) { T; commented = COMMENT(MEDIUM); } else if (<70)
  { T; if (COMMENT(HIGH)) { T; SetHenchmanAsHired(false); commented = true; } } if (commented) { T; AngerTimer += 30; }`.
  Jump threading then gives exactly retail: MEDIUM jumps into LOW's ~CCharString call (0x00E61176), HIGH's constant-true flag threads
  to 0x00E610AF, >=70 threads to the join. The scheduling tell: LOW/MEDIUM `mov bl,al ; lea ecx` (store to the variable) v HIGH
  `lea ecx ; mov bl,al` (a plain if-temp). Probe probe/v_flag.cpp reproduced it on the first compile; the full row matched on 1 grade.
- Bare `__forceinline` on the row-local CTimer / CScriptThing helpers replaced by FABLE_FORCEINLINE (vland NOT_PORTABLE otherwise).
- Test test/00e5fd40.cpp: state model of the hired loop (anger chain with spoken / unspoken warnings, health states, summoners,
  state group comments, redundancy break).

## (third author) 0x00D973D0 -- still near miss DIFFER(9821v9821), 43 frame-slot hunks (differ/00d973d0.cpp; bare __forceinline -> FABLE_FORCEINLINE, unchanged)
- Interference sets (pk.py --iset, tools/pkw.sh fixed for unnamed items): iset(letOut) = {Trapped, LookingForHobbeMainChamber, t12463,
  seh_me}; letOut conflicts with NO slot0 member and not with t12415 (Nymph HIT temp). hand890 TRACE.md: join scans slots from the most
  recent down, so letOut (w2, placed after t12415 w3) always joins slot1. Retail slot0 => letOut was placed BEFORE t12415: at packing
  either weight(letOut) >= 3 (ties go by creation order, letOut first) or weight(t12415) <= 2. Final retail code has exactly 2 letOut
  frame refs (0x00D9787D, 0x00D97B9A) and 3 t12415 refs, like ours, so retail's extra ref must vanish after packing.
- Weight = memory refs after RA: an extra register read (`GiveHeroMorality(... + letOut)`, probe only) leaves w2. A mini probe
  (probe/lo/m2.cpp, tools/pkm.sh) with all callee-saved registers busy gives letOut w3 in the memory-phi form (`mov [m],1` in the arm,
  reload after the scope) -- different code from retail's `mov bl,1 ; jmp past reload`.
- Spellings measured (weight stays 2, grade 9821/43 unless noted): loop arm `break` / `continue` after `letOut = true`, `while (letOut ==
  false)`, `while (true) { if (letOut) break; ...}` (9822/55), loop `letOut = answer == 1; if (letOut) HC_T;` (9821/46), cut-scene
  `letOut = answer == 1; if (letOut) HC_T;` (9805/12: letOut no longer a frame item), cut-scene `else letOut = false;` (9805/11), cut-scene
  `letOut = true;` before HC_T (9821/49).
- Next level if resumed: find the c2 pass that runs after the frame packer and can delete a frame store/reload (store-to-jump-to-reload
  forwarding that would turn retail's packed `mov [m],1 ; reload` into `mov bl,1 ; jmp past reload`), then a source shape that leaves
  the arm's store in memory at RA (answer still live in ebx at the arm, or letOut live into the while loop until RA).

## (fourth author) 0x00D973D0 CBanditSacrifice::Main -- MATCHED (RELOCATION_MATCH, vland WIN, test PASS), src/00d973d0.cpp, no flags
- The 43 frame-slot hunks were ONE source fact: the let-out flag and the share flag are the SAME bool (named `saidYes`, descriptive:
  bool register locals appear in no PDB; the Anniversary PDB lists only stack objects for this function). c2 packs byte items by
  memory-reference weight (hand885: refs counted before cross-jumping); separate bools give letOut w2 (init 0x00D9787D + reload
  0x00D97B9A) < Nymph HIT temp w3, so letOut joined the Nymph slot. One symbol = w6 (2 + takeShare's 4: 0x00D98E56, 0x00D9934B,
  0x00D993F7 + one) -> processed first, opens slot0, sacrificed / HIT temps join, the Nymph temps get their own slot above seh_me.
  43 -> 5 hunks in one grade. Spelling: `bool saidYes = false;` before FadeScreenOut, and `saidYes = false;` where `bool takeShare`
  stood (the reset is behaviour: the flag is still true from the let-out question).
- The last 5 hunks: AddQuestInfoBarHealth colour is CRGBColour(255, 0, 0, 255) (red; R byte +2 = 0xFF at 0x00D97FAD / 0x00D98D3E),
  the draft had (0, 0, 255, 255) -- a behaviour bug hidden in the frame noise. Fixed at both sites -> RELOCATION_MATCH.
- FITTED: sacrificed kept as its own bool (folding sacrificed into saidYes too is also RELOCATION_MATCH, tmp/e/m3.cpp); the merge of
  letOut + takeShare is EVIDENCED by the retail slot partition (no two-variable spelling reaches it: ~45 earlier grades + k = 1..7
  unused locals measured here, unchanged).
- Local enum EScriptEntityMoveType replaced by the shared engine/EScriptEntityMoveType.h (byte-identical). EScriptAIPriority stays
  local as in the matched siblings (the repo has only empty forward enums for it).
- Measured on the way (no change, weight stays 2): k = 1..7 unused bool / float / long locals (function top, before the flag), arm
  spellings `letOut = !letOut` / `= answer == 1` / `= answer != 0`, flag declared at function top / near Trapped (worse, init store
  moves). Mini probe probe/fw/m4.cpp + tools/mv2.sh reproduce the split shape (memory init + reload, `mov bl,1; jmp` arm) in 5 s.
- Test test/00d973d0.cpp: state model (trapped calls, let-out question + repeat on talk, follow loop, sacrifice / cage / hit count,
  rescue, share question with the flag reset, red bar); mutants without the reset or with the blue bar fail.
