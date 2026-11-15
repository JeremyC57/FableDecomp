# wfE1 NOTES (acctC; rows 0x00D13580, 0x00DF6010, 0x00DB97A0, 0x00CF68C0)

## Header overlay
- engine/CQ_BountyHuntScript.h: NEW opt-in FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_MAINTHREADS (see hdr/README.md); 6 asserts pinned.

## 0x00D13580 CQ_BountyHuntScript::Main (row 1) -- best DIFFER(6861v6917) 26 hunks, /d2inlT75 (differ/00d13580.cpp)
- Extent confirmed (6917 bytes, last `ret` 0x00D15084). Draft ported onto the per-entity repo opt-ins (GANGDEPUTY1, HOSTAGE1/2,
  GANGLEADER, BOASTS) + the new MAINTHREADS opt-in; GANGDEPUTY2 NOT defined (its declared ~TeleportData() forces calls).
- 6770 -> 6861: TeleportData dtor implicit (ct.py: retail's loop-top exit pad 0x00D13F24 expands it to 4 ~CCharString, the cold pad
  0x00D15032 calls COMDAT 0x00CD4550; with both called they cross-jumped into one). Loops: `for (;; Game->NewScriptFrame()) { if (T)
  return; ... }` for the region-entrance loop and the main quest loop (`if (QuestSucceeded || QuestFailed) break;` after the T check:
  retail's main-loop head is the QS/QF test reached by `jmp`, bottom `NSF; T; je head; jmp exit` = the rotated top T check).
- Left: cutscene-2 movie dtor (retail expands at 0x00D1460E; ours calls 0x006E7B80): c2 inline trace f 0.0005 (sc 5945) v cs1 0.0469
  (sc 74, expanded only with /d2inlT75). Every `if (T) return` halves the static frequency of the fall-through, so after loop 1 + the
  GreatwoodEntrance wait the cs2 block is ~100x colder than in retail. Tried (no change to cs2): loop 1 as for(;true;NSF) (6949, more
  expansions), while(true), a BountyHuntIntroComplete flag loop. The rest (frame slots +0xE8 / hostages, eax/edx picks at 0x00D14858..)
  follow from it. Anniversary PDB locals: tData, defName, scriptName, teleportTo, teleportMkr, regionName, sehHero, myMap, speech_movie,
  entrance, GangLeader, Female, Male, GangDeputy1/2 (CScriptThings), sehGangLeader, sehGangDeputy1/2, seh_Hero, seh_Male, seh_Female,
  csMap, thang/teleportMarker/tPos (teleport loop) -- the draft's structure differs (names only, not adopted yet).
- SUCCESSOR (wfE1b): 6861/26 -> 6892/10 hunks. cs2 movie dtor closed by a LOCAL frequency lever: the cast2 scope and the movie scope each
  `do { } while (0)` (x10 each: f 0.000458 -> 0.0458) + /d2inlT76 (sc 75.6). A global lever (for(;true;) loop 1, if/do-while waits) also
  expands it but moves pads / frame slots elsewhere (the surrounding code must stay cold: retail's main-loop pads are in the cold area).
  Left: CCharString==text non-null arm reloads (retail compares 2..6 reload the pointer from the frame, compare 1 does not).
  Reload residue measured with tools/c2prio_f.py (patched to print piece spill): the CSE candidates regionName.PStringData (#45,
  compares 1/3/5) and teleportTo.PStringData (#29, compares 2/4/6) have spill <= 0, are split into one 2-ref piece per compare
  (test + deref, spill 8) and every piece gets eax. Retail = compare 1's piece in a register, the others 1-ref pieces (spill 0,
  dropped -> reload at each use). Insensitive to: named bool / const-ref / no-paren / ==true spellings of the condition, implicit
  TeleportData::operator= (same bytes; the ctor must stay declared: implicit ctor is expanded at entry), Compare body visible (worse).
  Unexplored: what splits retail's pieces per reference (no allowed register left in their region? liveness edge?).

## 0x00DF6010 CQ_TraderConflictEvilScript::Main (row 2) -- DIFFER(6247v6493) 333 hunks (differ/00df6010.cpp), 1 grade
- NEW shared header engine/CQ_TraderConflictEvilScript.h (Ego_r layout, sizeof 0x70; hdr/README.md); the draft's local entity-class
  macro moved into it. Residue as the draft says: whole-function register colouring (retail ebx=this, ebp=0) and the ~CScriptThing
  expand/call split (retail 23 sites v ours 40). Not worked further (breadth first).

- SUCCESSOR (wfE1b): 333 -> 62 hunks (DIFFER(5922v6493), differ/00df6010.cpp). tools/ol.py (one-letter call outline, retail v ours,
  aligned) showed the draft's structure is close; the hunks were (1) vector<CScriptThing>::erase EXPANDED in ours, CALLED in retail
  (0x00DF6BA9 -> 0x00CD3152): repo opt-in FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE (333 -> 127); (2) ~CScriptThing: retail expands it at
  17 sites (PImp release + ~CBaseClass 0x0099A2E0) and calls 0x004AA840 only for the three early by-value temporaries (0x00DF6371 /
  63F9 / 6951, c2 f <= 0.38) and in the all-call pad 0x00DF75D7 (reached only from 0x00DF71B4): row-local `inline CScriptThing::~CScriptThing() { }`
  gives that split by itself (early sites r=0, loop sites r=1); (3) ~vector<CScriptThing>: retail EXPANDS it at most return pads
  (_Destroy loop `push 0; call [edx]`, then the shared free/epilogue tail 0x00DF7957, 6 jumps) and calls 0x008AC970 only at 0x00DF75C7 /
  0x00DF75F9. Ours with the inline dtor expands it everywhere without sharing the tail (8155 bytes); the differ source carries a
  DIAGNOSTIC declaration-only `template <> std::vector<CScriptThing>::~vector();` (all calls; NOT a hand-back device). (4) guard loop as
  `while (true) { if (!(cond)) break; ... }` (76 -> 62; do-while / for(;cond;) no better). Next: the return-pad map (which exits share the
  0x00DF7957 tail, which exit goes to the all-call pad: an inline_depth(0) region round the 0x00DF71B4 terminate check) with the vector
  dtor left inline.

- THIRD AUTHOR (wfE1c): 62 -> 62 hunks but three BEHAVIOUR fixes now in differ/00df6010.cpp (5902v6493): `MissionSucceeded = true;`
  after StopOverrideMusic (retail 0x00DF7630 `mov byte ptr [ebx+0x68],1`; ours had no store at all); the guard-loop count is signed
  (`test eax,eax; jg/jle` 0x00DF6CD2 / 0x00DF72F7 -> `(long)AllCreatures.size() - InitialNumberInRegion - NumberSpawned + 25 > 0`,
  as the landed CTC_GuardSpawnPoint::Main); the shopkeeper search (k loop) indexes AllCreatures[j] (0x00DF6AE9 `[ecx+esi]`, esi = j*12:
  an original script bug, kept). Retail return pads per T-check (tools/tj.py): 0xDF7962 (none), 0xDF75F5 (vector C), 0xDF75EC (intro C +
  vector C), 0xDF75E3 (+ follower), 0xDF75D7 (+ fighter); expanded pads 0xDF6D1D (end-of-j-body T), 0xDF73B9 (guard top), 0xDF745B
  (m top), 0xDF74F9 (n2 top: 3 ST EXPANDED + vector CALLED 0x00DF75C7), in-place post-guard / beds2 / post-beds2 / final; the SickChild T
  (0x00DF7851) destroys RegionName then jumps to the 0xDF75E3 called chain.
  Measured (tools/st.sh + tools/sites.py on t2/gj.cpp = inline ~vector): c2 regime 0, ~CScriptThing E iff f > 0.38, ~vector E iff
  f > ~0.467 (T 74, no caller-size penalty: a+g <= 6.7k). Static-frequency model tools/fm.py reproduces all 29 of our site f exactly
  (for: body 5x entry, after = body top + break flows; while with an inner loop = for; T-return 0.5 / fall 0.5; materialised || bool
  0.5; && 0.25). Retail's split is NOT reachable by any /d2inlT or site pragma on our frequencies: R13 (k top) C and R17 (guard top) E
  have equal f in our structure; R23 needs ST expanded and vector called (f in (0.38, 0.467)) while R15's vector (f 7.1, sc 45) is
  expanded. fm.py's only solutions put a x0.5 "guard" in front of every loop (beds, i, j, k, m, n/n2) + colder ifs, i.e. retail's loops
  have body ~2.5x entry in c2's model; `if (n > 0)` before a for does that but is NOT code-neutral (VC7.1 keeps both tests, t2/h1.cpp).
  The wait-loop forms while(true)+break / do-while in if are hotter (0.333 / 0.375 v 0.25). Open question: which plain loop spelling
  gives c2 a 2.5x body with a single guard test.

- wfE1c lead (measured, t2/h2.cpp): a GUARDED DO-WHILE `long b = 0; if (b < n) { do { ... } while (++b < n); }` compiles to retail's
  exact beds-loop bytes (one `cmp edi,ebp; jle`, rotated body: same as the `for`) but c2 gives its body NO loop multiplier (R3 f 0.625
  -> 0.0625, R4 -> 0.094; `for` = 5x, `if (n > 0) for` = 2.5x but keeps a second test). tools/fm2.py (structural interpreter, exact on
  our trace) with loop kinds for / do / while and if p .25/.5/.75 finds no assignment meeting all retail sites: best 2 left
  (beds=do, n=do: R13 + R23). Hand analysis: feasible only if the hero-if and fighter-if run at p 0.25 (materialised || bools give
  0.5 in ours) and the j/k region is ~2x colder relative to the guard loop than any for/do mix gives. Not closed; next: measure p of
  `bool x = ...; if (x)` / `if (!(...)) {} else` spellings and the k loop as do, with st.sh (16 s per trace).

- wfE1c BREAKTHROUGH: DIFFER(6493v6493), 16 hunks (differ/00df6010.cpp, /d2inlT72). Calls / length equal; left = register and SIB
  order. Steps (each measured with tools/st.sh): beds loop as guarded do-while (all early ~vector sites called); the hero-side n loop as
  `while (n < size()) { ...; ++n; }` (c2: x1 body, R20/R21 cold, n2 side unchanged); FABLE_FORCEINLINE on the row-local GetDataString
  (retail expands it at 0x00DF64D1; c1 34, needs f > 0.108, the do-while beds loop leaves 0.094); `#pragma inline_depth(0)` round the k-loop
  top T (retail 0x00DF6AD9 -> called pad 0x00DF75EC; sc 67); /d2inlT72 (or 73) for R23 (n2 top: three ~CScriptThing sc 68 expanded,
  ~vector sc 73 called 0x00DF75C7) while the guard-top / post-loop ~vector (sc 71) stay expanded; every called site has sc >= 76.
  Residue hunks: SIB [offset+base] in retail v ours [base+offset] at 0x00DF6839 (i loop CreateCreature AllCreatures[i]) and 6F0A / 6F1D /
  7157 / 7169 (n / n2 loops: AllCreatures[m], [n]); 0x00DF6B83 erase loop ecx v eax; 0x00DF6C03 sar order; R15 pad (0x00DF6D1D) eax/edx;
  0x00DF6F80 esi reload; fillers. Declaring i / m before their loops does not move the SIB order (s1/s4).

- wfE1c later: CScriptThing::operator== in the landed spelling `PImp->IsEqualTo(*other.PImp)` -> 13 hunks (6477v6493; the erase loop
  load order, the sar order and the R15 pad registers closed). Left (parked as allocator / numbering ties): six SIB base/index flips
  (0x00DF6839, 6B86, 6F0A, 6F1D, 7157, 7169: retail's order varies per occurrence, e.g. 0x00DF6F07 [eax+esi] matches but 0x00DF6F0A
  [edi+eax] does not) and the hero GiveThingBestEnemyTarget block 0x00DF6F6E..6F90 (retail keeps the Game vtable in edi and spills
  _M_start to [esp+58h], adding the n offset after GetHero; ours spills the vtable and keeps begin+offset in edi). Tried without
  success: FABLE_RETAIL_STLP_VECTOR_INDEX_START (32 hunks), i / m declared before their loops, k = 1..7 unused `int` locals (no
  change at all), a named `CScriptThing& hero = GetHero()` (18) or `const CScriptThing& guard = AllCreatures[n]` (21).

## 0x00CF68C0 CEntranceGuard::Main (row 4) -- DIFFER(8150v8070) 392 hunks (differ/00cf68c0.cpp), 2 grades
- New opt-in FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_ENTRANCEGUARD (class body; Ego_r 32 bytes). Draft residue unchanged (frame
  0x160 v 0x15C, ebp/ebx zero register; movie slots). Not worked further.

- SUCCESSOR (wfE1b): tools/ol.py shows the draft's call structure nearly equal to retail (T count 70 v 71; 8 movie-dtor sites v
  retail's 6, all expanded in retail (6 x 0x006E7AB0, no 0x006E7B80); ours calls the COMDAT at the two scope-end sites 0x00CF75AF
  (talk-1 movie +0x14, tail-merged with the hit-1 movie +0x48 at 0x00CF786D) at sc > 130 (T90..T130 sweeps only unmerge more pads);
  the seh_me exit pads are EXPANDED STB dtors in retail (0x00CF6F56, 0x00CF79CB with `xor esi,esi`, final) placed after the intro
  loop's ENTER block. do-while(0) round every movie scope (row-1 lever) makes it worse (8710). Not closing this session.

## 0x00DB97A0 CNOVI_Theresa::Main (row 3) -- MATCHED (RELOCATION_MATCH 7013v7013, vt PASS, vland clean, addmatch done)
- What closed it (from the H40j draft at 6859v7013): (1) main loop `for (; true; Game->NewScriptFrame())` (FITTED; replaces the draft's
  `if (T || false)`); (2) scoped `#pragma inline_depth(0)` round the intro loop's top terminate check (retail calls that exit copy,
  c2 score 73 v the expanded sites 56 / 65: no single /d2inlT); (3) a `{ }` block from seh_me's declaration to the end (FITTED) with
  `#pragma inline_depth(0)` before the post-loop `if (T) return` and `#pragma inline_depth()` after the block's `}`: retail calls the
  seh_me / outro_trigger dtors at the exit block (score 47 at f 2.5 after the `true` loop). Without the block the region would end
  at the function's brace (function-wide); without the post-loop return seh_me's frame weight ties the cond-temp flags word (frame
  packer: density w/size, tie -> flags first). Locals renamed from the Anniversary PDB (byte-neutral).
- Opt-ins: repo only (draft set: CPREDICATEBASE_NOVTABLE, CSCRIPTTHING_IMPLICIT_ASSIGNMENT, MOVIEBASE_OUTOFLINE_CTOR/DTOR,
  STB_OUTOFLINE_ASSIGN, BARRELMAN_THERESA, ...). No flags line. Grades used ~60.

## Hand-back summary (wfE1)
- 0x00DB97A0 matched with REPO headers only (g++ -M: the only overlay files it reads are hdr/stlport/fable_containers.h and
  fable_map.h, and it grades RELOCATION_MATCH without them too). No `// Flags:`; two scoped inline_depth regions (Inline-budget line).
- Row 1 extra probe: loop 1 as `for (; true; Game->NewScriptFrame())` raises the cs2 movie dtor site to f 0.0295 (sc 108, from 0.0005),
  the right direction, but moves the loop-1 exit pad (0x00D13F24) into a merged copy and adds a pad at the main loop bottom
  (DIFFER(6949v6917) 28 hunks; `!false` / `1` spellings identical). Next: with that loop form, find which exits retail sends to
  private v merged pads (tools/jt.py) and what lifts cs2 from f 0.03 to >= 0.047 (cs1's).
- Descriptive names: outro_trigger (0x00DB97A0; the Anniversary PDB has no name for that CScriptThing). Everything else PDB-spelled.
- Probe probe/00d13580_for_true_loop1.cpp: at /d2inlT74 DIFFER(6917v6917) 35 hunks (equal length by coincidence: cs1 + cs2 movie dtors called, loop-1 exit pad merged, an extra all-call pad at the main loop bottom 0x00D14B60); T109 6997, T120 7689. Grades on row 1 this session: ~25.

## Hand-back summary (wfE1c, third author)
- 0x00DB97A0 unchanged (matched by wfE1a; src/test/payload untouched).
- 0x00DF6010: 62 hunks / 5902 v 6493 (diagnostic) -> 13 hunks / 6477 v 6493 with NO diagnostic device, calls equal
  (differ/00df6010.cpp, `// Flags: /d2inlT72`). Devices to decide: guarded do-while beds loop + `while (...) { ...; ++n; }` hero loop
  (FITTED, byte-neutral frequency levers), scoped inline_depth(0) at the k-loop top T, FABLE_FORCEINLINE row-local GetDataString,
  /d2inlT72 (73 equivalent). Left: 6 SIB base/index flips + one allocator block (GiveThingBestEnemyTarget, 0x00DF6F6E). Behaviour
  fixes vs every earlier draft: MissionSucceeded = true; signed guard count; AllCreatures[j] in the k loop. ~75 grades + ~20 traces.
- 0x00D13580: two probes only (guarded do-while teleport loop: byte-identical, residue unchanged -> the reload pieces do not depend on
  static frequency; c2prio priorities use loop depth w = 1 << depth, not frequency). Unchanged differ.
- 0x00CF68C0: not worked by wfE1c (draft is macro-built `#define T / NSF / EG_*`; needs a full hand re-author: frame 0x160 v 0x15C,
  seh_me at local +0x14 in retail v +0x04 in ours, zero register ebp v ebx).
- No header changes by wfE1c; no new names (MissionSucceeded / CounterID / NumberSpawned / InitialNumberInRegion are the existing
  Ego_r members of engine/CQ_TraderConflictEvilScript.h in hdr/).

## wfE1d (fourth author) -- 0x00DF6010 RELOCATION_MATCH with device c (src/00df6010.cpp, test PASS, addmatch, vland clean)
- The 13-hunk residue of differ/00df6010.cpp (six SIB flips + GiveThing block 0x00DF6F6E) is a c2 record-count position, not a
  statement fact: sweeping k initialised unused `int` locals at the top of Main (e4/kin.py + e4/sw.sh, 1 s a grade, e4/sweep_i.log)
  moves each flip at its own threshold (66b8 period 32; 7157 right from k 14; 6F84 block right at 60..63 / 93..108 / 155+; 6839 only
  from 212). MATCH at exactly k = 224 (212..219: 6E3E/6F07 + label 6AC9; 220..223: 6F0A; 225: 6F07). The earlier "k = 1..7, no change"
  was true but far too short a sweep. Ints placed in another function above Main do nothing (ids are per function).
- Device d measured as an alternative: one dead FABLE_RETAIL_ASSERT holding one inline CCharString text compare = 35..38 records
  (GetName on a named object 35, on a GetHero() temp 35, AllCreatures[i].GetName 36, GetDataString 37, GetDefName 38; a plain int
  compare 0, `i < size()` 1, a second `||` compare +3). By-value CCharString temporaries in TWO asserts break additivity (6E3E/6F07
  stay wrong for every k: a second quantity); GetName (const ref) asserts are additive (2 x 35 = 70). So ~6 asserts + a few locals could
  replace the 224 locals; not built (device c is preferred by WF_RULES order).
- Delivered: src/00df6010.cpp = differ source + one `int fid000..fid223 = 0` block (14 lines, `// Fidelity:` header line + inline
  comment). Decision for the coordinator: accept a 224-local ruler (smallest k that matches) or keep the row parked.

## wfE1d -- 0x00D13580 (row 1): unchanged differ (DIFFER(6892v6917), 10 hunks), ~75 grades this session
- k-sweep of initialised unused locals (device c): k = 0..64 step 1-32 and 70..400 step 6: no change below k 166, MORE hunks
  (15) from k 172; the reload residue is not a record-count position.
- c2prio (e1/prio0.txt): regionName #45 prio 20 spill -8 refs 6, teleport.teleportTo #29 prio 8 spill -16 refs 6, both split into
  three eax pieces (allowed only a....bp: ecx/edx = Compare's fastcall registers, esi/edi = repe cmpsb). Retail facts re-read from
  the bytes: compare 2's deref reloads straight into ECX (no candidate at all there), compares 3..6 reload into EAX and their null
  arms use `xor edx,edx` (eax busy), compare 1 keeps eax. Ours: no reloads; null arms of compares 4/6 use eax.
- Probe (e1/pA..pC, diagnostic `#define protected public` + a row-local copy of operator==(const char*)): the header text
  reproduces the 10 hunks exactly (control); `if (p) return p->Data==text; ...` and the ?: form change the layout (80+ hunks), so the
  header text is retail's. Not closing; parked again.

## wfE1d -- 0x00CF68C0 (row 4): DIFFER(8150v8070)/392 -> DIFFER(7956v8070)/185 (differ/00cf68c0.cpp)
- Macros expanded by hand (cpp + clang-format; byte-identical) and `// Flags: /d2inlS15` from the three landed same-TU fan Mains
  (EVIDENCED by siblings, Inline-budget: caller IL-size cap for the implicit MovieBase dtor): frame now 0x15C = retail.
- Census (e3/calls.sh v retail): T 70 v 71, ~vector 9 v 10, delete 1 v 2, ~CBaseObject 6 v 5, everything else equal; retail has 3
  epilogues (0x00CF6F9E after the wait-loop pad 0x00CF6F56, 0x00CF7A15, end), ours 2: identical seh_me pads cross-jump in ours.
- First wrong decision (e3/prio_tr.txt): region 1 colouring. Retail: const 0 = ebx (prologue..0x00CF6AB4, wait-loop pad), &Me
  (`lea ebp,[esi+8]`) = ebp until 0x00CF6A6D, a NEW &Me piece in edi inside the intro loop (0x00CF6A2D), the MsgIsTalkedToBy bool in
  frame byte [esp+13h], Game cached in ebx at 0x00CF6AB4. Ours: const 0 piece #1 = ebp (prio -153, only ebp left), &Me piece #240
  (prio 924, 34 refs, allowed only ebx) = ebx, the bools (prio 24, byte reg across a call -> ebx only) in bl, Game in ebp.
  Retail's intro-loop head 0x00CF6A00 is ALIGNED (jmp + lea ecx,[ecx]); ours 0x00CF69F4 is not (frequency meter: our intro loop is
  colder?). Tried: do{}while(0) round the intro loop (280 hunks), `for (; !IntroDone; )` (identical), `while (true) { if (IntroDone)
  break;` (262). Next: find why retail's &Me piece ends at the intro loop (piece formation) / why retail's const 0 piece outranks the
  bools; --blocks of c2prio for the intro-loop blocks.

## Hand-back summary (wfE1d, fourth author)
- 0x00DF6010 MATCHED: RELOCATION_MATCH 6493v6493 with /d2inlT72 + device c (224 unused initialised `int` locals, `// Fidelity:`),
  test/00df6010.cpp PASS (vt.py; three mutants fail), addmatch done, read-only vland clean (both rows RELOCATION_MATCH PASS).
  DECISION: the ruler is far outside the k = 0..7 the prompts name; it is the smallest k that matches (sweep 0..400). Alternative
  (device d, not built): ~4-6 dead asserts holding inline CCharString compares (35-38 records each).
- 0x00CF68C0: 392 -> 185 hunks (frame = retail) with the same-TU siblings' /d2inlS15 and hand-expanded macros; parked on the region-1
  register swap (see the section above). Insensitive to k = 0..300 and /d2inlS16..30 (S12/S14 = old 392).
- 0x00D13580: unchanged (10 hunks); parked again (k-sweep, T77..90, operator== text probes: nothing moves the reload pieces).
- No header changes (hdr/fable_assert.h was copied for probes and removed again); no new names; LABELS / NAME_FIXES unchanged.

## wfE1e (fifth author) -- both rows re-measured; neither closes (differ sources unchanged)
### 0x00CF68C0 (DIFFER(7956v8070), 185 hunks) -- causal chain found, first wrong decision = c2 split-pass order
- Every structural item of the census is an RA consequence, not a statement fact:
  * T 70 v 71: retail keeps has_followers in frame byte [esp+13h] (0x00CF6E29 `mov al,[esp+13h]`), so the `if (T) return;` at the top
    of BOTH arms of `if (has_followers)` stay (0x00CF6E37 / 0x00CF6F9F); ours keeps it in bl and c2 HOISTS the common T call above
    `test bl,bl` (ours 0x00CF6DFA). Same for the talked-to / health bools ([esp+13h] in retail, bl in ours).
  * delete 1 v 2 / ~CBaseObject: retail's seh_me exit pads A 0x00CF6F56 (loop-1 T, zero = ebx) and B 0x00CF79CB (intro-top T +
    post-intro T, `xor esi,esi`) differ only in the zero register, so they are not cross-jumped; inline trace (tmp/inl_f4a.txt):
    ours EXPANDS the same two sites (#3 f 0.5 sc 68, #9 f 0.625 sc 65; T-ret after loop 1 #6 sc 85 called = retail) but both pads use
    the same zero register and merge. Inline decisions are already right.
  * Loop-head alignment (retail 11 pads, ours 8; intro head 0x00CF69FD `jmp; lea ecx,[ecx]`): NOT a frequency meter here: x10
    do-while(0) round the intro loop, loop-1 `while(true){if(..)break;` and removing the post-loop-1 T (diagnostic) never align it.
    It is position: the pad is emitted only when few bytes are needed (ours head at ..F4 would need 12; retail ..FB needs 5): our
    region 1 is 7 bytes shorter (`push ebp` v retail `push 0` x6). Alignment differences follow the register colouring.
- Root (tools/c2prio_f.py on f4/a.cpp -> f4/prio_a.txt): retail keeps the FE conditional-temporary FLAGS WORD (home [esp+34h], entry
  store 0x00CF68D2, bits 1/2/4 hit-1, 8/0x10/0x20 hit-2, 0x40 DoneArena) in EBX in pieces (prologue..pad A = also retail's "zero",
  `or ebx,1 / test bl,4 / and ebx,-5` 0x00CF75DC.., reload `mov ebx,[esp+34h]` at the intro-loop bottom 0x00CF7872). Hence ebx is
  busy over the intro loop -> the byte bools (bl-only across a call) go to memory, &Me takes ebp (region 1) / edi (intro loop), Game
  goes to ebx only for 4 refs. Ours: flags word #87 prio -1155 `split, memory` (or/test/and through eax/edx/[esp+34h]); in the split
  pass &Me's piece #240 (prio 924, 34 refs, allowed only ebx because ebp/edi are held by main-pass vtable temps over its range) is
  coloured FIRST and takes ebx; the bools (main pass, prio 24..88) already hold bl. So retail colours flags-word pieces before
  &Me's region-1 piece (or forms &Me's piece shorter, ending at the intro-loop head where retail re-leas `lea edi,[esi+8]`
  0x00CF6A2D). Unknown: which source fact makes retail's flags pieces outrank / &Me's piece shorter (split-pass piece formation is
  not documented in C2_ALLOCATOR_PROBE.md). Next: instrument piece extents (blocks per piece) in c2prio, then compare variants.
- Probes this session (tools/g4.sh: grade + hunks + our pad addresses): decl-order/loop-shape variants v1/v3/v4 (f4/): v3 identical,
  v1 280 hunks, v4 (diagnostic) 261. ~6 grades + 2 traces.
### 0x00D13580 (DIFFER(6892v6917), 10 hunks) -- unchanged
- New probes (f1/): Anniversary declaration order (regionName before teleportMkr) 16 hunks; TeleportData last 18; spawn-flag
  `else if` chain = byte-identical to the || condition (10 hunks; IL normalised); three separate `if (...) spawn = true;` 195 hunks;
  no-paren && / || = identical. The reloads do not depend on the condition's statement shape. Anniversary PDB lists tData /
  defName / scriptName / teleportTo / teleportMkr / regionName all at function scope (no blocks), as ours. ~7 grades.
- Reading of retail (for the next author): compare 2's deref reload goes straight to ecx (`mov ecx,[esp+58h]; mov ecx,[ecx]`),
  compares 3..6 reload into eax; null arms 1/3/4/5/6 use edx as the strcmp scratch (eax busy), null arm 2 uses eax. Ours: null arms
  2/4/6 use eax. A byte-class candidate (the inline operator== return temp homed at [esp+1Fh], stored at every compare) may be
  coloured before the integer CSE pieces; not measured (c2prio prints integer candidates only).
- 0x00CF68C0 extra measurements: first split pass (f4/prio_a.txt line 413..494): flags-word piece #239 (of #87) prio -521 v &Me piece
  #240 prio 924 -> &Me first, flags piece left with no register and re-split to memory. &Me `lea [esi+8]` census: retail 16
  (690D ebp = region 1, 6A2D edi = intro-loop head, 78B6 edi = second-loop head, 870F edi, 77D2/861A ebx, ten eax transients), ours
  15 (690D ebx spanning region 1 + intro loop, no re-lea at either loop head; 7551/7761/838A/85BC ebx). Retail's &Me pieces end
  at the loop heads; ours does not. A lever must either shorten &Me's first piece (piece formation) or lift the flags piece above it.

## wfE1f (sixth author) -- neither row closes; 0x00D13580 source cleaned (same bytes, no fitted levers)
### 0x00D13580 (DIFFER(6892v6917), 10 hunks) -- differ/00d13580.cpp now `// Flags: /d2inlS15`, no do-while(0), two opt-ins fewer
- NEW: the landed same-TU siblings CBH_GangDeputy1 / CBH_GangDeputy2 / CBH_GangLeader ::Main (0x00D17F20 / 0x00D18DF0 / 0x00D1B200)
  all carry `// Flags: /d2inlS15`; that flag alone gives the SAME 10 hunks as /d2inlT76 (and S15+T76 the same), and with it both
  FITTED `do { } while (0)` levers round the cutscene-2 cast / movie scopes are unnecessary (removing either or both: identical).
  Under S15 the opt-ins CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR / CENTITYSCRIPTBINDING_FORCEINLINE_CTOR are byte-neutral and
  were dropped; MOVIEBASE_FORCEINLINE_CTOR (53 hunks without), CCHARSTRING_FORCEINLINE_TEXT_EQ (338), CBASICSTRING_TEXT_EQ_INLINE (100)
  stay. S20 178 hunks, S12 33. So the differ source is now flag-evidenced and has no FITTED item; residue unchanged.
- Re-measured under S15 (no change, 10 hunks each): k = 1..9 unused int locals; the old f1/ probes (decl order 16, TeleportData
  last 18, else-if chain 10, three ifs 195); row-local operator== probes with the inner CBasicString compare spelled `?:`, `==0`, `!`
  (all identical: IL normalised); TeleportData ctor and / or operator= bodies defined in the TU (ctor visible = inlined, 6917v6917
  15 hunks, reloads unchanged; operator= visible = identical).
- c2prio --trace (g1/tr_a.txt): whole candidates #45 regionName (refs 6) / #29 teleport.teleportTo (refs 6) skip (spill -8 / -16),
  the split pass makes one 2-ref piece per compare (test + deref), prio 40 for compare 1 (its test shares the loop-top block: larger
  K) and 24 / 16 for the rest, allowed `ac.si..`, all -> eax. Retail's register use reads as: compare 1 piece {test, null arm, deref}
  (null arm scratch edx); compare 2 piece {test} only (null arm eax, deref a fresh load straight into ecx); compares 3..6 piece
  {test, null arm} (null arm edx) with the deref outside the piece (reload into eax). So retail's split pass did NOT grow the
  compare 2..6 pieces into their deref blocks; ours does everywhere. What forbids it there and not at compare 1 is unknown (no
  candidate in our trace occupies those blocks); a mini TU (mini/m1..m3.cpp, `mini/run.sh` prints the chain signature in ~3 s)
  reproduces OURS exactly, i.e. the plain loop + six compares is not enough to get retail's shape.
- ~35 grades + 2 traces this session. Parked again.
### 0x00CF68C0 (DIFFER(7956v8070), 185 hunks) -- unchanged
- Probes (all 185 hunks): dropping the WIDESCREEN / MOVIEBASE forceinline opt-ins (7956 / 7951 / 7951), named `bool hit = ...` /
  `bool talked = ...` for the hit and talked-to conditions (IL-identical). Same-TU fan siblings carry the same opt-in set and flag.
- Reading of retail hit-1 (0x00CF75C5..): &Me in edi (`mov eax,[edi]; mov ecx,edi`), flags in ebx (`or ebx,1/2/4`, `test bl,4`,
  `and ebx,-5`, each followed by the home store); ours `lea ebx,[esi+8]` (new &Me piece 0x00CF7551), flags first in edi then memory
  (`test byte ptr [esp+34h],4`). ~5 grades. Parked again.

## wfE1 h1 (fifth pass, auditor fix) -- 0x00DF6010: device c (224 unused locals) REPLACED by device d (6 dead asserts), still RELOCATION_MATCH
- Auditor flag: `int fid000..fid223 = 0` (k = 224) is far outside device c's k = 0..7. Removed. src/00df6010.cpp now has NO unused
  locals; instead 6 dead FABLE_RETAIL_ASSERTs (overlay hdr/fable_assert.h = the staged acctC NEW_fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM),
  each one `x.GetName() == "<literal>"` (const-ref GetName: no class temporary, no cond-temp flag bit), placed: after the fighter
  lookup, before both GiveThingBestEnemyTarget calls (guard name), at the end of each guard-loop frame (follower), after
  RemoveQuestInfoElement (follower), before EntityUnsetAsOpinionSource (hero). Conditions INFERRED/FITTED, count + region EVIDENCED by grading.
- Measurements (h1/, ~1 s a grade, tools h1/mkv.py + h1/sw.sh + h1/subs.sh, logs h1/lg_*.log, h1/subs.log, h1/subs56.log):
  one assert placed before the guard loop = 35 records (36 with AllCreatures[x].GetName, IsEmpty 2, IsNull 0, `||` second compare +3);
  placed from the fighter lookup on = 32 (single-assert window sweeps: match at k = 192 instead of 189; some post-loop sites match at
  both k = 192 and 224). Every subset of the 7 late candidate sites x k = 0..7: sizes 1..5 never match (5: 3-7 hunks), 6 matches at
  k = 0 for 5 of 7 six-subsets, 7 and 8 also at k = 0. Three early + three late asserts need k = 23 (S1). So 6 is the smallest count.
- Inline controls: all three still needed with the asserts in place: no FORCEINLINE on GetDataString -> 6477v6493 (T72/T73);
  no inline_depth region -> 6605v6493 (T73) / 6653v6493 (T72); neither -> 6589v6493; T 50..100 and /d2inlS 8..30 replace neither;
  no flag 6541v6493; plain `for` beds loop (to drop the FORCEINLINE need) 8237v6493. Inline-budget line kept, now states these numbers.
- Local `enum EHeroAbility` replaced by `#include "engine/EHeroAbility.h"` (repo header; byte-neutral: the 224-local source also matched with it).
- Real-text first: not re-opened (four earlier passes worked statement order / named temporaries / scoping for ~300 grades; the
  residue moves only with record counts). Grades this pass: ~1,150 cheap sweep grades (mostly the subset search) + ~60 targeted.
- Re-done: q.py RELOCATION_MATCH (/d2inlT72), vt.py PASS, addmatch (payload source has 6 asserts, no fid), read-only vland: both rows
  RELOCATION_MATCH PASS, 0 RELOC_IDENTITY. The old 224-local source is kept as h1/orig224.cpp (reference only).

## wfE1g (seventh author) -- 0x00CF68C0 RELOCATION_MATCH (src/00cf68c0.cpp, test PASS, addmatch, vland clean)
- The 185-hunk "register allocation" residue was a STATEMENT fact every earlier draft missed: in retail the talk block's end jumps
  PAST the hit test (intro loop 0x00CF75C0 `jmp 0x00CF786D` = shared movie dtor -> ebx reload -> IntroDone loop test; second loop
  0x00CF83E2 `jmp 0x00CF8699` -> DoneArena test). Hit-1 0x00CF75C5 has a single predecessor (the talked `je` 0x00CF6A4D). So both
  loops are `if (Me.MsgIsTalkedToBy(..)) { ... } else if (hit) { ... }` (EVIDENCED; `continue` is wrong for the second loop: it
  would skip the DoneArena test). Intro else-if alone: 185 -> 57 hunks (zero / flags word in ebx = retail); both: 0 masked hunks
  with ONE real byte difference: 0x00CF84D6 (hit-2's first `if (T) return;`) jumps to the out-of-line ~seh_me exit 0x00CF8832 in
  retail, to the expanded pad 0x00CF8740 in ours = one inline decision (q.py masks jump targets: found by a byte compare of
  non-relocation bytes, tool k4/rd.sh).
- Inline control (device b, WF_RULES order 2: per-row flag): /d2inlS15 (landed same-TU fan Mains) + /d2inlT69. Sweep with S15:
  T50/60..68 far off, T69..T73 RELOCATION_MATCH, T74..80 the one site, T85+ far off; without S15 no T matches. T69 = smallest.
- Census items of earlier notes (T 70 v 71, ~vector, delete, ~CBaseObject, 3 v 2 epilogues, loop-head alignment, bools in
  [esp+13h]) all closed with the else-if: they were consequences, as wfE1e found, but of the CFG, not of c2's split order.
- Test: state model (lane convention for script Mains, like the landed fan tests) incl. the talk-frame-skips-hit rule; the mutant
  "hit tested after the talk" fails. No header changes; no new names. ~75 grades (sweeps) this session.

## wfE1g -- 0x00D13580 (row 1): unchanged differ (DIFFER(6892v6917)), one NEW residue item found, ~45 grades + ~60 mini compiles
- NEW (masked by q.py / fg, which hide jump targets): besides the five compare-chain reloads there is ONE real jump-target
  difference: 0x00D13DE8 (the `if (IsActiveThreadTerminating()) return;` inside `while (!Game->MsgOnRegionLoaded(regionName))` of
  the first region loop) jumps to the FINAL epilogue chain 0x00D15032 in retail (with the post-while T 0x00D13E08 and the later
  returns), but to the near copy 0x00D13F24 (the for-loop top T exit, fallthrough after the rotated bottom test) in ours. Both
  copies are byte-identical (~cast, ~seh_hero, five ~CCharString, TeleportData dtor), so this is a cross-jump (tail-merge) target
  choice, not an inline decision: /d2inlS15 + T60..90, S16..20 and k = 1..32 unused locals do not flip it; outer-loop reshapes
  (pre-loop T + bottom NSF/T; NSF at the body end) are worse (6863 / 6876). Tool: k1/jt.py (instruction diff with jump targets as
  relative instruction indices) + k1/r1.sh (grade + `de8_differs`).
- Mini TU (mini/k/: sig.sh prints per-compare null-arm scratch / deref keep-or-reload; retail = edx/K eax/Rc edx/Ra edx/Ra edx/Ra
  edx/Ra, ours = edx/K eax/K edx/K eax/K edx/K eax/K): 25 variants, ALL give ours: CCharString::operator== texts (if(P==0), ?:,
  bool r, local SD* p before / after the test, else-return), CBasicString == texts (==0, named long), free-function / static-helper
  operator==, forceinline inner ==, flags /d2inlS15 /d2inlT76 /Ob1 /Ot (/Os all eax), declaration order, escaped addresses
  (global = &region / &t), extra loop depth, no outer loop, no element clears. Diagnostic volatile loads: deref-volatile = all Ra
  with ours' null-arm alternation; test-volatile = all edx/Ra and loses the null-arm jump threading. So retail's mixed shape is not
  "separate loads" either. If/else-if with the spawn body duplicated three times: 7884 bytes (no tail merge) -> not that.
- Open: what makes c2 cut the compare 2..6 pieces at the deref block (retail) -- and which source fact moves the 0x00D13DE8 merge.
- 0x00CF68C0 follow-up: the row-local `enum ETextGroupSelectionMethod` / `enum EHeroAbility` replaced by the repo headers
  engine/ETextGroupSelectionMethod.h / engine/EHeroAbility.h (byte-neutral, still RELOCATION_MATCH /d2inlS15 /d2inlT69); EScriptAIPriority
  stays row-local like the landed fan siblings (no standalone header; it lives inside CThingAICreature.h etc.). Re-done: vt PASS,
  addmatch (payload holds this source), read-only vland: all three rows RELOCATION_MATCH PASS, 0 RELOC_IDENTITY.
