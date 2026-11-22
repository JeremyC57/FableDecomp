# NF251 notes (2026-11-22, round 18b): the CEngine pair landed by folding 0x00B2AB10, the CIEngine stand-ins, the 0.0 constant

Base: the repo tree with nf249 landed (CEngine.h / CIEngine.h / CEngineVideoOptions.h opt-ins in the repo). NF250 runs beside me (no
overlap: its entries are CODEXP27 lines, 0x0049D800, 0x01296F14). None of my rows is in codex_pass28 RESERVED.tsv. Locks: work/locks.log
(0x00B260B0 / 0x00B261D0 were pre-locked NF251 by the coordinator; every other row a new directory, claimed before its first grade).
Tools: NF249's set re-pathed (tools/), plus g3.sh (q.py grades, objects kept), gh.sh (HEAD grade), lk.sh (HEAD / naive / one-object-per-TU
links), vtsec.py (relocation list of the section defining a symbol, for vtable COMDATs), pre.sh (NF248's clang precheck: clean on every
row source before its grades; see Rule slips for the two exceptions). Evidence the notes cite is copied to evidence/ (work/ is scratch). Headers: none changed (hdr/ is empty; orig/ empty). Descriptive names added: one scaffolding class,
`CIEngineConstructorReference` (anonymous namespace, 0x00B23750; hashed per TU, not a retail name). Flags lines added: none.
Read-first list: the brief chain NF251 -> NF215, NF214 and its read-first list (NF92 BRIEF / LESSONS / NOTES, NF44 BRIEF, NF88-90 LESSONS,
COORDINATION.md, EXTRA_RULES.md and ASM_RULES.md in full); GOTCHAS.md by grep only (TU-emitter, fold, funclet, nf86 / nf88 / nf140 lines).

## Entry 1 (q12700): the CEngine pair + 0x00B2AB10 -- done (decision 1 on the link reading)
- 0x00B2AB10 (??_GCEngine, 30 B) is folded under the ctor row: rows/00b2ab10.cpp has no code of its own and includes
  `../src/compiled/00/b2/CEngine_CEngine_00b260b0.cpp`, the EH funclet rows' pattern (1,355 Unwind rows; the include resolves through
  rebuild/include). Its local one-slot CEngine model and the strong scaffolding `CEngine::CEngine() { }` are gone. Retail truth: VC7.1
  emits ??_GCEngine only beside the CEngine vtables, in a TU whose code stores the vptr (the ctor or dtor), and both have their own rows.
- Devices ruled out (no grade spent): a scaffolding derived class cannot emit ??_GCEngine (CEngine's ctor is out of line, so the TU never
  stores CEngine's vptr); an `inline` out-of-class ctor would be a second ctor body (outside the entry).
- Grading: the ctor draft is staged at $A/src/compiled/00/b2/ (the overlay $A/hdr resolves `hdr/../src/...`); objsyms.py: the fold
  object's 88 function COMDATs equal the ctor object's (it defines ??0CEngine, so a HEAD include would show ??0EngineNative instead).
- Pair: NF249's drafts regraded on the landed tree (empty overlay): both RELOCATION_MATCH + PASS first time; the ctor row gains one
  comment line (TU emitter of ??_GCEngine). 0x00B2AB10 RELOCATION_MATCH + PASS. vland (work/r1.log, + 0x00B26340): 4 / 4, no RELOC_ warning.
- pk.py (evidence/pk1.tsv): ctor 7 EQ / 80 no owner; dtor 5 EQ / 46 no owner; fold 7 EQ (??0CEngine 280 B EQ owner 0x00B260B0 once
  `__real@0000000000000000 -> 0x0122ED70` is bound: the q12692 add line) / 80 no owner; 0 NEQ.
- Census: 0x00B260B0 binder 0x00B26340 already ??0CEngine@@QAE@XZ (1 spelling after); 0x00B2AB10 only the CEngine vtable 0x012A0F3C slot 0
  (.rdata), leaf unchanged; 0x00B261D0 bound by 0x00B2AB10 as ??1CEngine@@UAE@XZ (table witness line holds from the fold object).
  Table lines: the five q12692 lines re-verified against today's reloc_bindings (4 deletions + the __real@0 add; TABLE_CHANGES.tsv).
- Link (evidence/lk1_cengine_set.txt; 0x00B260B0 0x00B261D0 0x00B2AB10 0x00B23770 0x00A5C570 0x0099A310 0x00B23750 0x00B26340 0x00B25D80):
  HEAD 2 LNK4006 (both HEAD debt); new, one object per TU (the fold object is the ctor's TU): 2, the same two; new, naive per-row link:
  3 = those two + ??0CEngine (0x00B2AB10.obj v 0x00B260B0.obj), the SAME TU compiled twice, identical body (pk EQ). Decision 1.
- The fold row and the pair must land in the same commit (the fold includes the new ctor file; with the HEAD ctor file it has no ??_GCEngine).

## Entry 2 (q12701): ??0CIEngine IAE, the five slot targets in the entry and five more the CIEngine vtable census found -- done (11 rows)
- Evidence: ??0CIEngine@@IAE@XZ in FableWin.pdb and Ego_d.pdb (raw grep: IAE 1 / QAE 0 in each; Ego_r has neither: inlined); the five
  slot names in Ego_r.pdb + FableWin.pdb (ghidra_out name sets, one each); vtable slots 11 / 18 / 39 / 49 / 50 (NF249 work/e3/slots.tsv).
- Census (callers.py, absrefs.py, reloc_bindings): no E8 caller and no table line for any of the six; the only references are the
  CEngine vtable 0x012A0F3C and the CIEngine vtable 0x012A0D24 (.rdata), whose landed emitters (the q12700 rows) already bind the PDB names.
- 0x00B23CF0 / 0x00B23D90 / 0x00B23DE0 (CEngine.h FABLE_RETAIL_CENGINE + OVERRIDES) and 0x00B23350 / 0x00B23540 (CIEngine.h): each the
  PDB member on the header, off its local `struct ..._Methods` stand-in; MATCH first time; each object defines only its leaf.
- 0x00B23750: off its local CInitBaseClass / CIEngine models onto CIEngine.h CTOR (NF249's inline protected ctor). It must not be strong:
  the CEngine rows carry pick-any ??0CIEngine@@IAE@XZ copies (a strong IAE definition would give LNK2005, nf140). The nf86 scaffolding:
  an anonymous-namespace class derived from CIEngine with an out-of-line ctor (CIEngine is abstract: never instantiated), so the TU emits
  the inline ctor as a pick-any COMDAT. RELOCATION_MATCH first form. Its object also emits the header's ??_7CIEngine@@6B@ (85 slots,
  relocation list identical to the ctor / dtor rows' copies: evidence/vt_ie_*.txt; HEAD emitted a 1-slot stand-in table under that name,
  the nf176 data-COMDAT rule) and ??_GCIEngine EQ its owner 0x00B23770; the scaffolding class's own members are hashed (?A0x...).
- Link (evidence/lk2_ciengine_set.txt): one object per TU HEAD 2 -> new 1 (the ??0CIEngine QAE pair 0x00B23770 v 0x00B23750 is gone); left: HEAD
  ??0CInitBaseClass in 0x00B23770 v 0x0099A310.
- Spellings: the five slot addresses 2 -> 1 each once q12700 lands (self + vtable emitters on the PDB name). 0x00B23750: IAE (self + the
  CEngine rows) and QAE (only 0x00B23770's strong TU-emitter scaffolding, which nothing binds): count-neutral; 1 when 0x00B23770 moves
  (NAME_FIXES; it can take the 0x00B23750 scaffolding verbatim with leaf ??_GCIEngine@@UAEPAXI@Z: not done, outside the entry).
- CIEngine vtable census (advisor; evidence/vt_cie_slots.tsv: retail 0x012A0D24 by rvt.py joined by index with the object's 85 slots and
  each target's landed Leaf line): the new ??_7CIEngine@@6B@ (emitted by the q12700 pair rows and by 0x00B23750; at HEAD only 1-slot
  stand-ins carried that name) binds PDB names at slots whose self rows were `CIEngine_Methods` stand-ins: 18 / 50 (in the entry) and
  26 / 35 / 36 / 39 / 49 = 0x00B233C0 AddChildPrimitive, 0x00B23450 GetEditorAnimation, 0x00B23460 AddCustomMorphTexture, 0x00B23490
  IsStaticMapNeedingUpdate, 0x00B23530 OnActivate (CIEngine's own). Left alone they would go 1 -> 2 spellings (a split the pair would
  already create), so they moved in this hand-back the same way (FableWin + Ego_d, or Ego_r + FableWin; the only reference of each is that
  slot): MATCH first time, each object defines only its leaf. The 9 __purecall slots agree with retail (0x00BFEA08 at the same indices);
  the other 69 targets' landed rows have no Leaf line (the same no-Leaf debt class as NF249's 34 CEngine-table targets): not censused.
- Tests unchanged (standalone; 0x00B23750's models CInitBaseClass / CIEngine locally: HEAD debt).

## Entry 3 (q12702): 0x0122ED70 onto __real@0000000000000000 -- done
- Census: table lines ?InitialPreloadTime@@3NB (0x00B260B0, goes with q12700), ?g_clearDouble@@3NB (witness 0x00ADF1E0), ?g_zeroPitch@@3NB
  (0x0089BDF0); a source grep of `extern ... double` naming 0x0122ED70 found a third stand-in with no table line, ?g_ProgressTimeZero@@3NB
  (0x00499CE0, CProgressDisplay ctor).
- 0x00ADF1E0 already binds __real@0 at HEAD (`CurrentTime = 0.0`; xref of the HEAD object): not a binder, its table line is stale.
- 0x0089BDF0: `cos(0.0)` / `sin(0.0)` for the stand-in global: VC7.1 does not fold the intrinsic cos / sin of the literal and loads it from
  __real@0 at the same two sites (evidence/xr_0089bdf0_*.txt: only the symbol changes). RELOCATION_MATCH first form; test copy (a verbatim
  copy of the source) updated the same way, PASS.
- 0x00499CE0: `= 0.0` for the three timers (VC7.1 loads a double 0.0 store with `fld qword [__real@0]`, not fldz, as in the CEngine ctor):
  RELOCATION_MATCH first form; the row comment that called it "a named constant" corrected.
- Symbol diffs: each object only loses its UND stand-in and gains DEF __real@0000000000000000 (the compiler's pick-any constant).
- Census by absrefs.py (advisor): 188 absolute references to 0x0122ED70, 66 landed rows (12 references in unlanded code); their `extern`
  double / float declarations (evidence/census_0122ed70_externs.txt) name 0x0122ED70 only in 0x00B260B0 (InitialPreloadTime), 0x0089BDF0
  and 0x00499CE0; the rest name other addresses. 0x0122ED70 then has one spelling among landed sources, __real@0000000000000000.

## Entries 4-6 -- open (context; handed back early as the brief allows)
- q12703 (0x00890C80, the 0x00890D80 recipe): needs one header opt-in first: CThingCreatureBase.h does not declare
  ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ (0x006649C0; Ego_r + FableWin + Ego_d). Add it as a member-specific add-only opt-in
  (e.g. FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON, one line inside or after the UNSHEATHERANGEDWEAPON block, keeping the line
  count as NF249 did, or pin_asserts), plus CTCCarrying.h's existing FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON, the event
  GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON = 33 and UNSHEATHE_MELEE_WEAPON. Count effect: 0x006A15A0 (7 spellings in the table)
  and 0x006649C0 (3) each lose the CThingNative spelling if no other row emits it.
- q12704 (0x006648D0 / 0x006649C0 / 0x00664A50 onto CThingCreatureBase.h): shares that opt-in (0x006649C0's own leaf).
- q12659 (0x00418DCA): not started (last, needs the 469-includer proof).

## Rule slips
- GOTCHAS.md read by grep only (not in full); the nf247 / nf244 landing READMEs and nf249's hdr/README.md not read (only the nf249 landing
  README, NOTES, LESSONS, RENAMES, NAME_FIXES, APPLIED, STATUS, TABLE_CHANGES, lk3.txt and the drafts).
- The clang precheck was not run before the first fold probe (0x00B2AB10 graded from the unchanged NF249 ctor draft, which NF249 had
  graded) nor before the HEAD-source grades (gh.sh / mkobj HEADONLY: landed sources); every new or edited source was prechecked.
- Link control rows 0x00B26340 / 0x00B23770 / 0x00A5C570 / 0x0099A310 / 0x00B25D80 were compiled from their landed sources for the links
  without locks (none had a lock directory; none was edited). 0x00B26340 was also in a vland payload (unchanged source).
- One Bash call exited non-zero (a trailing `kill -0` test on a finished job). No git, no foreground sleep (every wait by PID with kill -0 in a later call), every grade under
  `timeout` (900, or mkobj's 300), no rm -rf (tools/clean.py only), the clang precheck before every grade, nothing written in the repo
  (work/root_before.txt v root_after.txt).

## Decisions for the coordinator
0. 0x00B2AB10 is the first non-Unwind include row (it includes 0x00B260B0's file): an includer grep limited to *Unwind* rows misses it when
   0x00B260B0 next changes (and the HEAD -> new ctor file must land in the same commit as the fold row). If decision 1 holds the pair,
   TABLE_CHANGES.tsv `alt-add` says what still lands (q12701, q12702, with the __real@0 line witnessed by 0x00ADF1E0).
1. q12700 link reading: the fold row's object is the ctor's TU, so a per-row link that takes both objects shows one LNK4006 on ??0CEngine with
   an identical body (as every EH funclet row would against its parent). One object per TU: 0 new duplicates. Accept the funclet reading
   for the fold (one TU, two addresses: the "one-TU-for-several-addresses" rule nf234 ruling 5 waits on), or hold?
2. 0x00B23750's form: the inline ctor emitted pick-any through an anonymous derived-class ctor (nf86 scaffolding; the class name is
   descriptive, hashed). Accept? It also emits the full header ??_7CIEngine@@6B@ where HEAD emitted a 1-slot stand-in (nf176 rule).
3. 0x00499CE0 was not in the entry (found by census of 0x0122ED70); moved in the same unit so the address ends on one spelling. Accept?
4. 0x00B23770 (??_GCIEngine TU emitter: strong ??0CIEngine@@QAE@XZ and ??0CInitBaseClass, a global ?MakeOne@@YAPAXXZ, no Leaf line): queue
   the 0x00B23750 scaffolding for it (leaf ??_GCIEngine@@UAEPAXI@Z; its ??_GCIEngine copy in 0x00B23750's object is EQ), which would clear
   the last ??0CIEngine QAE spelling and the ??0CInitBaseClass LNK4006 (NAME_FIXES)?
