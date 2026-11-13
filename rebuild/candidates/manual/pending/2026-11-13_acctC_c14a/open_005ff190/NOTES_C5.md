# agentC5 (acctC lane) -- NOTES (continues agentC4; C4's files in prev_C4/)

## Row 1: 0x005FF190 CTCInventoryMap::CreateMapMarkers -- DIFFER(4096v4176), best differ/005ff190.cpp
Length 4,176 (code to `ret 8` 0x006001CB, `mov edi,edi`, jump table 0x006001D0..0x006001DF), oracle in batch/agentC5_oracle.tsv.
Best = C4's r2b + `/ 2.0f` in the path midpoint (FableWin.pdb emits ??KC2DVector (operator/) as the first COMDAT after
CreateMapMarkers 0x02099AF0 -> the source divides; same bytes as `* 0.5f`). Header line 1 corrected (C4's file said ConstructPCMapList).
Tools added: tools/tv.py (template variants `<<G@@alt0@@alt1>>`), tools/vs.py (grade a list of choice sets -> tmp/board.tsv),
tools/g.sh, tools/first.py (first differing instruction), tools/reg.py (retail-only instructions per region), tools/fq.py (inline
trace site list), tools/rep.py (exact text replace). Template: probe/r2/T.cpp (= T_head.txt + T_body.txt).

### Residue (all 133 call targets = retail; frame 0x7C both)
Pure register / frame-word allocation in EVERY region (retail-only instructions: head+hero 27, loop1 74, loop2 39, loop3 43,
loop4 94, path 50, end 15). Retail facts:
- head: `manager` never in a register (stored [esp+0x10] right after the call, every use reloads it); const 0 in ebx for the
  marker init and `cmp edi,ebx`, then `mov ebx,[ebp+8]` (parent) before the `je`; heroMarker in a frame word [esp+0x34] with an
  esi piece at SetAngle; physics reuses edi (hero). Ours: manager gets an ebx piece (#218 prio 27 > const-0 head piece 18).
- parent: retail gives it LONG ebx pieces (head..hero AddChild; loop4 first AddChild through the whole path loop; reloads at
  0x005FF5CA, 0x005FFBA3, 0x005FFCD9 only). Ours: every parent piece is a short 2-ref piece forwarded into ecx.
- loop1: retail keeps the GetTC LowerBound `end` (finish) in [esp+0x1c] and the teleporter-name compare bool in [esp+0xf]
  although ebx is unused there (=> ebx was held by a candidate later given back, or the candidates had no ebx); ours: ebx / bl.
- path loop: retail miscDef [esp+0x18] (memory), path node edi, parent ebx; ours miscDef edi, path ebx.
### Measured (this session, ~135 grades + 6 c2 traces)
- DIAG `CManager* volatile manager` => head identical up to #21 (`mov ebx,[ebp+8]`); + volatile heroMarker => const 0 takes ebx
  through the hero block (retail: immediates there), parent still ecx. So two levers needed: manager memory-only AND parent pieces.
- Code-identical spellings are inert (dead-local ruler 1..10, `!= NULL`, named player ref, marker decl after hero, physics inline,
  loop-4 own counter, map name, cards name, per-scope card decl, named sprite, CCHARSTRING_FORCEINLINE_EQ, NAMED_RESULT_ADD).
  Moving: heroPosition inline (HP=1: manager -> edi), regionName inline (RN=1), AddChild in each loop-1 arm (A1=1: frame 0x84),
  shared card for loops 2/3 (K2=1, much worse). Random combinations (30): none beats base (tmp/board.tsv).
- Reset<CComponent> TEXT: retail's own out-of-line Reset 0x00429C6F = the DEFAULT header text exactly (`ReleaseData(); Data=data;
  if (data) Info = new CCPPointerInfo(1, DeleteData, (void*)Data);`, no else), and the two expanded sites (0x005FF8C7 loop-3 site 1,
  0x005FFD4D teleport) reload Data from the member after `new` (`mov ecx,[esp+0x5c]`), i.e. the text reads Data. C4's fitted
  RESET_ELSE_NULL reads `data` (keeps it live across new: `mov [eax+8],esi`) -- wrong at the sites although its cost (c1 108)
  reproduces the 9-site split. Every Data-reading text tried (hdrx FABLE_DIAG_RESET_1..48, 30 texts) costs 95/106/112 (inl trace),
  never 108/109. DIAG forced split with the default text (noinline Reset + forceinline ResetF at the 2 sites): calls = retail,
  959 v 973 retail-only bytes -- the text is NOT the main allocation lever. Frequency model (c1 95 needs loop-3 site 1 in
  (2.97,5.94] and loop-1 sites <= 2.97; for/while list loops are code-identical): no loop-form combination fits; `&& true` folds.
  => open question for the coordinator: retail's c1 for the default text must be 108-109 in this TU (FableWin Reset 69i v 44i).
- Regime: c2 count 1780-1914 for this function (hand883 cnt tool): regime 0 (block weights 1<<depth).
- Not tried / next: model c2's split-region pass (0x1078B51C) for parent's pieces (why retail's pieces span calls); FableWin COMDAT
  list for this TU (GetRelativePosOnMap precedes CreateMapMarkers in FableWin -- check whether retail's CMM calls or inlines anything
  from it); compile CMM in the full retail TU order (ConstructRegionExitMarkers 0x005FE840 before it).

## Row 2: 0x005F1B10 CTCInventoryAbilities::ConstructPCMagicList -- DIFFER(8367v8383), best differ/005f1b10.cpp
Ported the acctB draft (ref/005f1b10/draft.cpp, 8380v8383, 158 hunks / 610 retail-only bytes) onto today's headers: only the
declaration opt-in FABLE_RETAIL_CTCINVENTORYABILITIES_PCMAGIC was needed in hdr/ (asserts pinned, hdr/README.md). Grader for
this ebp-frame row: tools/g3b.sh (grade + tools/alb.py = alignment with [ebp±x] AND [esp+x] masked).
What moved it (in order, each graded):
1. `if(surroundCount)` / `if(attackCount)` / `if(physicalCount)` (retail `je`, not the unsigned `> 0` `jbe`): 29 -> 26 masked hunks.
2. Loop-2 empties declared INSIDE the loop body (`EHeroAbility empty=...; EHeroAbility savedEmpty=...;` per iteration: retail
   `and [ebp+5Ch],0 ; and [ebp-4],0` at the loop top, 0x005F1C57) -- loop 2 now byte-identical up to frame words.
3. FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE instead of _OUTOFLINE (visible noinline push_back): small gain.
4. GetMiscThingsDef 0x005EE3B3 given its real body in the TU (out of line under the existing auto_inline(off) block:
   `long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1); CDefPointer<..> def;
   CGameDefinitionManager::Get().GetDef(index, &def); return def;` = retail's 101-byte body) -> the misc release at the end
   becomes retail's (no null test, no zero store, 0x005F3BAD): 26 -> 23 masked hunks, 159 -> 118 bytes.
Residue (frame-masked, tmp/alb_f1.txt):
- Loop 1: retail computes `min(size, TotalSpellsInPalettes)` ONCE into a frame word [ebp+6Ch] (size first, total second, `ja`,
  size recomputed in the other arm) and tests it at the top (`test eax,eax ; jbe`) and bottom (`cmp eax,[ebp+6Ch] ; jb`), while
  esi = &AssignedAbilities (CSE from the clear() call) and edi = &AssignedAbilitiesSave. The bound in the for-condition (draft)
  recomputes it every iteration; a named `count` (unsigned/long/const, ternary either way, std::min, reused as the loop-2
  counter) takes esi (c2prio: count 41 > the &AssignedAbilities CSE temp, whose range runs to the std::find at the entry loop,
  prio -716) and loads total first. Next idea: shorten the CSE temp's range (the std::find site spelled so it does not share
  `this+0x17C`), or find the form c2 hoists itself.
- Frame-word permutation (unmasked grade): retail `frame` [ebp+2Ch] (ours 34h), pairs 54h/58h, 10h/14h, 24h..34h -- packer
  density order (tmp/pk_f1.json via tools/pk/pk.py --func ConstructPCMagicList; tools/pkslots.py). Retail order puts the
  faded/deletion 8-byte slot before `frame` before the entry/swap/action slot; ours is the reverse.
- Level index (0x005F2E7C, 0x005F2F3B): retail loads Abilities._M_start before *it and does `lea edx,[eax+1]` (ours `inc edx`):
  temporaries' rotation; `1+x`, `+1L`, named `level`, VECTOR_INDEX_START all tried (no / worse).
- 0x005F3420..0x005F3457: `mov edi,[ebp-50h]` hoisted in retail before the Abilities lookup (order of the same loads).

## Session summary (agentC5)
Grades: 150 lines in ATTEMPTS.log (129 row 1, 21 row 2) + ~10 vs.py/fg batch grades not double-counted + 8 c2 traces (prio x5,
inline x2, packer x1) + fwsize. No row matched; no payload.json / src / test. NAME_FIXES: none proven this session.
Header changes: hdr/engine/CTCInventoryAbilities.h + FABLE_RETAIL_CTCINVENTORYABILITIES_PCMAGIC (declaration only, asserts pinned).
C4's opt-ins unchanged. hdrx/ = diag only.
