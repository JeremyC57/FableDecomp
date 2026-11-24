# NF264 notes (2026-11-22, round 25b): the AllowsCombinationAnims unit (q12750); q12743 / q12744 / q12659 left open

Base: the repo tree with nf261 landed. NF263 runs beside me: none of its entries or locks touched (every row I compiled was `released` or
had no lock directory before I claimed it, work/locks.log; none is in codex_pass28 RESERVED.tsv). Tools: NF261's set re-pathed (tools/), plus
tools/coffvt.py (COFF reader: the symbol in slot N of every ??_7 vtable of an object), tools/cens1.sh (clang census of one row),
tools/hobj.sh (VC7.1 object of the landed HEAD source), tools/vl2.sh (vland recheck with the full merged include tree work/tree_new as
overlay, one payload per `timeout 900`). Read-first list: the brief chain NF264 -> NF261 -> ... -> NF215, NF214 and its list
(COORDINATION.md, NF92 BRIEF / LESSONS / NOTES, NF44 BRIEF, NF88-90 LESSONS, GOTCHAS.md, EXTRA_RULES.md and ASM_RULES.md in full, long
lines included), the nf261 and nf259 landing READMEs, the nf261 hand-back (NOTES, LESSONS, evidence, rows) and nf259 LESSONS.

## Entry 1 (q12750): the AllowsCombinationAnims unit -- done as a 24-row unit (23 changed rows + the held 0x00692890 + 2 headers), see decision 1
The queue line names three rows; the census below shows the unit is larger: once 0x00692890 defines
`?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ`, every landed table that binds that symbol at a slot where retail has another body
resolves to 0x00692890 in a full link. In VC7.1, 21 landed rows emit such tables (0x006F5C10 + 20 more; the clang scan flagged 35, 14 of
which VC7.1 does not emit), so all 21 move in this hand-back, or 0x00692890 must stay held.

### Spellings (one PDB + the retail slot + the body each, the nf228 / nf261-ruling-3 standard)
- 0x0062EA10 `?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ`: FableWin.pdb 0x01D916B0 (no Ego_r / Ego_d record: header-inline base
  body) + retail CCombatActionBase vftable 0x01259E44 slot 66 = 0x0062EA10, read directly (CCreatureActionBase's 0x01259D24 slot 66 =
  0x00692890) + body `xor al,al ; ret`. 302 retail tables hold 0x0062EA10 at slot 66 (work/slot66_retail.txt, Fable.exe read at each RTTI vtable).
- 0x006F57B0 `?AllowsCombinationAnims@CCombatAction_StrikeResponseScorpionKing@@UBE_NXZ`: FableWin.pdb 0x023F2520 + retail vftable
  0x01262874 slot 66 = 0x006F57B0 (the only retail table with that address at slot 66) + body `mov al,1 ; ret`. Not in the queue line:
  it is in the unit because ScorpionKing's slot 66 resolves to a wrong body either way (to 0x00692890 if left, to 0x0062EA10 once the base
  declares the override). Decision 2.
- 0x00692890 `?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ`: both PDBs (nf261 entry 2), unchanged draft.
- No code callers of 0x0062EA10 / 0x006F57B0 (callers.py: none); no table lines for any of the three symbols (vtable slots only).

### Census (evidence/census_slot66.tsv)
- Method: a clang MS-ABI object (`clang++ -target i686-pc-windows-msvc -fms-compatibility-version=13.10 -c -O0`, HEAD tree, no wine) of
  every landed row whose source mentions CCreatureAction / CCombatAction / CCombatSequence / AllowsCombinationAnims (2,650 + 1,351 rows),
  then the symbol in slot 66 of every emitted ??_7 (tools/coffvt.py), against the retail slot 66 of the same RTTI vtable. Cross-check: the
  clang object of 0x006F5C10 gives the same 30 tables and the same slot-66 symbols as NF261's VC7.1 vtmap. Assumption (cross-checked on
  0x006F5C10 and the 34 flagged rows, not proven): clang -O0 emits a superset of VC7.1's tables, so no VC7.1 emitter is missed; and the
  source filter is by class-name text (every one of the retail classes with 0x0062EA10 / 0x00692890 / 0x006F57B0 at slot 66 is a
  CCreatureAction* / CCombatAction* class, so a row that emits one without naming any such class in its own text would be missed).
- clang failures: 48 rows. 13 recompiled with -fdelayed-template-parsing / -Wno-error=address-of-temporary (no AllowsCombinationAnims
  reference). Of the other 35, the 18 of pass 2 are STL / NScript rows; the 17 of pass 1 (CWorld::Init, the CCreatureActionBase /
  CCombatActionBase member rows, 0x0078B640, 0x007E80C0, 0x008C7480, 0x0093EA90, 0x00940960, 0x00839100 / 0x0083AFE0 / 0x0083B170,
  0x004C6140) were read by hand: none defines a ctor / dtor of a CCombatActionBase-derived class, and the ones that build a temporary action
  build a CCreatureActionBase-direct class (retail slot 66 = 0x00692890: UnsheatheItemFromInventory, Read, UseObject).
- HEAD: 126 emitted tables name AllowsCombinationAnims at slot 66. 65 bind the MBE CCreatureActionBase symbol where retail has 0x0062EA10
  (64 tables in 35 rows, clang) or 0x006F57B0 (1); 57 are right already (35 MBE at 0x00692890, 22 the CCombatActionBase UBE at 0x0062EA10, e.g.
  0x008B83C0 / 0x00955620 / 0x0096DDB0 / 0x0096DF50); 4 bind `?AllowsCombinationAnims@CCreatureActionBase@@UBE_NXZ` (0x00693B30,
  0x00693DC0, 0x0069F0F0, 0x006A0D50: the nf261 NAME_FIXES UBE-v-MBE access split, unresolved before and after).
- VC7.1 refinement (work/fh: VC7.1 objects of the HEAD sources of the 34 flagged rows): 14 of them emit no AllowsCombinationAnims table in
  VC7.1 (clang -O0 also emits the tables of inline ctors / dtors that VC7.1 /O2 drops; e.g. 0x009531A0 is the ~DragonRearUpFull row, a
  1.8 KB object with no ??_7): 0x007175D0 0x007AF490 0x008C2550 0x008C2C50 0x008E8C20 0x008E8CB0 0x00952890 0x009531A0 0x00957300
  0x0096AE50 0x009735D0 0x009756B0 0x0097BDA0 0x0097C900. They are NOT changed (drafts dropped, locks released). The other 20 do.
- Spellings per address after the unit: 0x0062EA10 1 (UBE; was 2: self stand-in + the emitters' UBE), 0x006F57B0 1 (was 2), 0x00692890 2
  (MBE + the four rows' UBE; was 3 with the self stand-in), so no new split; the MBE symbol is no longer bound at 0x0062EA10 / 0x006F57B0.
- After this hand-back: 0 VC7.1 tables bind a wrong body's symbol at slot 66 (the 30 of 0x006F5C10 and the 20 rows' tables move; the 4
  UBE-split rows stay unresolved, as at HEAD).

### Changes
- Headers (hdr/README.md): engine/CCombatActionBase.h `FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS` (both class branches,
  guarded against the PASS27 blocks that already declare it); engine/CCombatAction_StrikeResponseScorpionKing.h
  `FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_ALLOWSCOMBINATIONANIMS` (PROTOTYPE class). Add-only, declaration-only; one assert
  pinned (ScorpionKing line 12).
- 0x0062EA10, 0x006F57B0: self rows off free `__fastcall` stand-ins (no Leaf line) onto the headers; MATCH first form; new tests link the row
  objects (raw storage + qualified call, catalog PassPatterns kept).
- 0x006F5C10: the two defines. Its match rests on an inline-budget margin (its header comment), so its line count is kept: three comment lines
  of the Inferred paragraph were re-joined to make room for the two defines and one NF264 comment line (the function and its dead-arm
  FABLE_RETAIL_ASSERT stay on lines 197 / 199). RELOCATION_MATCH first form. Symbol diff against the HEAD object
  (evidence/symdiff_006f5c10.txt): UND -?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ, +the two UBE symbols, nothing else; vtmap
  (evidence/vtmap_006f5c10_*.txt): slot 66 SAME on all 30 tables, every other slot line identical.
- The 20 emitter rows (work/fam20.txt: 0x0074ACE0 0x0085FF10 0x008A9100 0x008B8590 0x008B9010 0x008C4E20 0x008C5370 0x008E08C0
  0x008E20F0 0x008E24F0 0x008E68E0 0x00924C70 0x00951430 0x009537B0 0x009551A0 0x00956960 0x00957170 0x009619C0 0x00962990 0x00974110):
  one define line each (`FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS`, with an NF264 comment
  on the same line), placed before the row's first preprocessor line. None of them has a FABLE_STATIC_ASSERT / FABLE_RETAIL_ASSERT / __LINE__
  in its own text, and no file includes any of them (no funclet / fold), so the extra line is inert. clang symbol diff of each object
  (work/census/fam_clang.txt) and VC7.1 objects, HEAD v new, with the merged tree as overlay (evidence/family20_objects.txt): exactly UND
  -MBE +UBE CCombatActionBase in all 20; every function COMDAT byte-identical with the same relocation symbols, except that 0x00924C70's
  Update names c1 local labels one number higher ($L8196 -> $L8197 ...: the one extra declaration shifts c1's numbering; bytes equal);
  every slot 66 in the new objects is the CCombatActionBase UBE symbol. (objsyms.py compares function COMDATs only; the tables of the 20
  rows are covered by the undefined-symbol diff plus the slot-66 read; a full vtmap was made for 0x006F5C10 only.) 0x008E08C0's `class CCombatActionBase;` is a forward declaration,
  not a local model.
- 0x00692890: the nf261 draft unchanged, rechecked here on today's tree.

### Checks
- Recheck (vland --recheck-landed, overlay = the full merged tree work/tree_new): first run work/ra..rd.log over the 37 rows drafted then
  (incl. the 14 later dropped) + 0x00692890 + control 0x006DF800: 2 MATCH + 37 RELOCATION_MATCH, all PASS, 0 RELOC_CONFLICT;
  recheck_final.log: one run over the 24 hand-back rows (rows.txt).
- Prechecks: every changed row compiled under clang before its grade (the census objects / pre.sh); the HEAD tests of the family rows,
  0x006F5C10 and 0x006DF800 prechecked: the only errors are in the VC7.1 headers (cwchar:9, functional:154, ostream / istream) or VC7.1's
  assert macro, all reproduced by the two-line controls work/ctl_hdr.cpp / ctl_hdr2.cpp (clang's, not the tests').
- Link, equiv_check, pk: see "Link" and "Equivalence" below.

### Link (evidence/link_e1.txt; LINK unset, /DLL /NOENTRY /FORCE /NODEFAULTLIB, tools/lnk2.py)
- Core: new 0x0062EA10 / 0x006F57B0 / 0x006F5C10 / 0x00692890 + HEAD 0x006DF800: 0 duplicates; every AllowsCombinationAnims reference
  resolves (one definer each: 0x0062EA10 UBE, 0x006F57B0 UBE, 0x00692890 MBE); the only watched unresolved line is 0x00692890's GetFlags
  call (owner 0x005DC210, not in the set; as at nf261). Positive control (a renamed copy of the new 0x0062EA10 object): LNK4006 on
  ?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ. HEAD control (HEAD 0x0062EA10 / 0x006F57B0 / 0x006F5C10 + 0x006DF800): the MBE symbol
  unresolved in 0x006F5C10 and 0x006DF800.
- Full unit: the core + the 20 new emitter objects: 0 AllowsCombinationAnims references unresolved (HEAD control with the 20 HEAD objects:
  21 MBE references unresolved). One duplicate in both, pre-existing: 0x009537B0's own ctor ??0CCreatureAction_DragonBurninateInto@@...
  is also defined by 0x00951430 (it carries the ctor body in its TU); the same LNK4006 with the HEAD objects, not touched here.

### pk / one definition
- 0x0062EA10 / 0x006F57B0 define only their leaf (symdiff). 0x006F5C10: all 110 function COMDATs byte- and relocation-identical to the HEAD
  object (objsyms.py), so no new pick-any copy. The 20 emitter rows: the same (evidence/family20_objects.txt; 0x00924C70's label numbering
  aside). So pk.py had no new copy to check.

### Equivalence (evidence/equiv.log)
- Whole-tree equiv_check, repo rebuild/include v work/tree_new (the repo tree with the two overlay files), every landed source and test.

## Entries 2-4 (q12743, q12744, q12659): open, not started
Entry 1 grew from 3 rows to 24 (and a 4,000-row census); on the advisor's advice I hand back at its boundary rather than start q12743,
which needs another header change, a second whole-tree equiv_check and two multi-class ports (nf261 NOTES entry 4 is still the plan;
q12744's caution in nf261 NOTES entry 5 still holds). No lock of those rows was taken.

## Whole-pass checks
- Descriptive names added: none. Flags lines added: none (no changed row has CompilerFlags; none needs one).
- Funclets / folds: no `../src/compiled/` include names any of the changed files (grep of src, tests, include).
- Locks (work/locks.log): the 24 hand-back rows stay NF264 (rows.txt); the 14 dropped emitter rows and the control 0x006DF800 are back to
  `released` (0x006F57B0 had no lock directory: created with plain mkdir).
- Nothing written in the repo (work/root_before.txt v root_after.txt identical); no git; no sleep.

## Rule slips
- Four GOTCHAS / EXTRA_RULES extracts (g3.txt, g4.txt, e1.txt, e2.txt) were written to the session scratchpad outside the agent folder to
  read them in full; deleted at once (advisor caught it).
- The clang census compiled ~4,000 landed rows (clang, not VC7.1; nothing written outside work/census) with no lock taken on them: a
  read-only scan, recorded here. Every VC7.1 compile of a row ran after its lock was mine.
- Two Bash calls exited 1: a lock `cat` on a missing owner file during the first lock survey, and an `objdump | grep -c` loop whose last count was 0.
- One background batch of 30 HEAD compiles was launched under an outer `timeout 1800` (each q.py inside under 300 s); I stopped it by its
  PID (`kill` on it and `pkill -P` on its children, my own process only; no cl.exe was killed by name) after 7 compiles and reran the rest in batches under `timeout 900`. The equiv_check ran under `timeout 2700` (not a grade).
- The census over-approximated (clang -O0 emits tables VC7.1 /O2 drops): 14 rows were drafted, rechecked (work/ra..rd.log) and then dropped
  once the VC7.1 objects showed no table; their landed sources are untouched.

## Decisions for the coordinator
1. Scope: q12750 lands as the 24-row unit (headers + 0x0062EA10 + 0x006F57B0 + 0x006F5C10 + the 20 emitter rows + 0x00692890), or, if the
   20-row define family is not wanted now, the safe sub-unit alone (headers + 0x0062EA10 + 0x006F57B0 + 0x006F5C10, which resolves the 30
   prototype tables and the 22 tables already on the UBE spelling) with 0x00692890 still held behind the 20 rows.
2. 0x006F57B0 (ScorpionKing override; FableWin + retail slot + body) joins the unit although the queue line does not name it: accept?
3. The two opt-ins are declaration-only and add-only; the CCombatActionBase one sits in both class branches under one macro. Accept, or
   should the declaration become the default of both branches later (it is retail: every CCombatActionBase has the override)?
4. NAME_FIXES (queue): the 4 rows on ?AllowsCombinationAnims@CCreatureActionBase@@UBE_NXZ (0x00693B30, 0x00693DC0, 0x0069F0F0, 0x006A0D50);
   the 0x006F5C10 prototype tables' other slots (70 of 81 emitted, CCombatActionBase overrides bound under CCreatureActionBase spellings).
- Result: 57,125 rows checked, 0 differ (DONE 0). Neither macro is defined by any landed row, so the default path of every includer is
  token-identical; only the 23 changed rows (which define the macros) change, and they are rechecked. orig/ re-diffed against the repo
  headers at the end: still current.
