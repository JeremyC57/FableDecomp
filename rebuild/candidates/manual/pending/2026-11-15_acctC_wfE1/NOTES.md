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
