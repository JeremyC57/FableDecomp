# NF239 notes (2026-11-22, round 12b): CODEXP27 name fixes, NF237's open + deferred lines

## 1. Triage (work/census0.tsv, work/triage.tsv, work/hc/)
- census.py (NF237's) per line, then a HEAD compile census (tools/hc.sh: every landed self row and every row the Fable.exe scan says
  references the address, 285 rows, objects graded and their (target, symbol) relocations kept in work/hc/xr_<addr>.txt). triage.tsv
  classifies each line from the COMPILED bindings, not the table: self DEFs the claim or not, and each binder's spelling (OK / BAD).
- PDB publics: work/pub_*.txt, deleted for disk (llvm-pdbutil dump -publics of $S/codex27/pdbx/*.4k.pdb, names only).
  Ego_d TYPE records: $S/dl_debug/out/types.txt (an existing dump).
- Off set: NF238 rows.txt / locks, the 29 still-held p27 lines (work/held_still.tsv: the 54 held lines minus the 25 now in the table),
  their witness rows and targets. I edited none of those rows. L38 / L39 / L37 deferred for that reason (APPLIED).
- Census gap (found and fixed): census.py's build_candidates regex required Address / Module / Source on three lines; 65 catalog
  entries put them on one or two lines (e.g. 0x00899DB0), so their call sites were invisible. tools/census.py now matches both
  (57,104 rows). Re-run for every address I touched (work/census_touched.tsv): new referencers only at 0x00733EE0 (0x00899DB0, L57,
  not changed) and 0x0122DEE0 (0x0066F7D0 / 0x0066F840 / 0x0066F8B0, all compiled: they bind the literal). Then a compiled census of
  all 118 0x0122DEE0 referencers (work/hc/): 114 bind __real@bf800000, my two g_fMinusOne rows, and 0x0041735A bound the 0.5f
  literal __real@3f000000 at 0x0122DEE0 (a wrong constant hidden by masked relocations: fixed, STATUS). 0x0042AADC (1 referencer)
  and 0x005F88C8 (3) compiled as stated in STATUS.
- Source greps for the old tokens (LinkedList_AllocateNode_12bytes, AllocateInsertNode_005f88c8, SitInSeat, UpdateTorchTasks,
  g_fMinusOne) match only the rows changed here.

## 2. Applied (20 rows; recheck_final.log: one vland --recheck-landed run over all 20, every one MATCH / RELOCATION_MATCH + PASS)
- **Access fixes (in-place, the row's own local model):** L88 0x0090FDB0 / 0x009387C0 `protected:` SitInSeat (0x00938380 2 -> 1);
  L45 0x0065C781 `private:` UpdateTorchTasks (the row's UpdateHornTasks pattern; 0x00659625 2 -> 1); L52 self 0x00726BE0 private
  UpdateAdditionalImpulse (binder already AAE; 2 -> 1). Ego_r + FableWin publics for all three.
- **NUISystem list family (L13 / L14 / L16):** 0x00429FBE `_M_create_node`, 0x0042AADC `insert`, 0x0042A141 `clear`, 0x0042AB1C
  `~_List_base` of list<NUISystem::CActionParam*>, each an explicit instantiation on stlport/fable_containers.h (the 0x0042AB01
  pattern; CREATE_NODE_NOINLINE / CLEAR_OUTOFLINE where retail calls the callee). 0x00429FBE 2 -> 1, 0x0042AADC 2 -> 1, 0x0042A141 3 -> 1,
  0x0042AB1C 3 -> 2. Left at 0x0042AB1C: 0x0053E860, a non-polymorphic scalar deleting dtor (FableWin ??_G?$list<CActionParam*>
  QAEPAXI@Z; NAME_FIXES) that binds ?CLandscapeBackgroundPatch_dtor_0042ab1c@@ (one probe, `delete` of a list*, did not emit it).
- **L17 / L18 self rows (count-neutral):** 0x0042ABAF and 0x0042ABCA are `_List_base<CComponent*>`'s ctor / dtor, not list's: retail
  0x0042B6A6 (list(const allocator&): `call 0x0042ABAF`) and 0x0042B6D2 (~list: a 5-byte `jmp 0x0042ABCA`) are the size-optimised
  list COMDATs wrapping them (STLport 4.6: `~list() {}`, the sentinel allocation and the clear + free live in _List_base). The same
  wrapper pattern proves 0x0042AB1C (0x0042B660 `jmp`) and 0x004AB490 (0x004AC050 `jmp`). The header comment on
  FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST ("FableWin.pdb names ??1?$list; no ??1?$_List_base for any instantiation") reads a Dinkumware
  PDB (`_Tidy`, `_Orphan_ptr`, `_List_val`; no _List_base type at all), so it does not arbitrate STLport layering: the opt-in's
  rationale is wrong, its bytes are right (a header-comment fix for the coordinator; I did not edit the header).
- **CPlayerGui (L20 / L21):** 0x00437640 IsDrawingTargetingCentre and 0x00437650 SetFlourishEffect onto engine/CPlayerGui.h
  (FABLE_RETAIL_CPLAYERGUI_MEMBERS, the ~CPlayerGui 0x0043A570 define set) + two NEW headers (hdr/README.md; Ego_d field lists).
  L22 done-already. L61 0x007B2990 onto engine/CTCGuard.h (+ a header test; the HEAD test modelled the stand-in).
- **0x0122DEE0 (entry 2):** only two rows spelled g_fMinusOne (source grep; the other ~113 referencing rows already use the literal):
  0x00744500 / 0x00744540 now compare with `-1.0f` (bytes and relocation target unchanged; the object defines the __real@bf800000
  COMDAT). The table line goes; no named spelling remains at 0x0122DEE0.
- **0x004AB490 (entry 3):** self -> ~_List_base<CThing*>; 0x004DE6ED onto the shared list header (BASE_OUTOFLINE) with a new test
  stub (the HEAD test stubbed the local ~list: LINK_FAIL otherwise). 3 -> 2; 0x004FE030's CThingList stand-in left.
- **L40:** 0x005F807C / 0x005F88C8 list<EHeroAbility> `_M_create_node` / `insert` (the L13 recipe). 0x005F807C 2 -> 1, 0x005F88C8 3 -> 2.
- 0x0041735A (entry 2 census): CMainGameComponent::InitWorld passed 0.5f to DisplayProgress where retail pushes the -1.0f at
  0x0122DEE0; now -1.0f (RELOCATION_MATCH, the relocation now binds __real@bf800000).
- vland WARNs: no RELOC_CONFLICT; the STANDIN_LEAF / RELOC_NAME lines name the old catalog labels of RENAMES section 1 rows (the
  coordinator renames), plus two pre-existing 0x0065C781 label WARNs (unchanged from HEAD).
- Proof: sc.sh symbol diff per row (only the respelled symbol, or the header's pick-any inlines on ported rows); pk.py (work/fin_final4.pk.tsv):
  every new copy with a retail owner EQ (_M_create_node x2, `??2` placement new, `_List_iterator_base` ctor, `~list<CThing*>` = 0x004AC050);
  3 NEQ copies (0x0065C781 IS_THING_ALIVE / LowerBound, 0x00726BE0 GetAllThingsInRoughArea stand-in) are NEQ at HEAD too (work/pk_hv.tsv);
  69 NO-OWNER inline copies. Link (work/fin_final4.lnk.txt): 0 LNK2005 / LNK4006 over the 20 objects. No existing header changed.
- No address grows: every TABLE_CHANGES add replaces a delete at the same target.

## 3. Open (APPLIED.tsv has each blocker), in the order I would take them
1. **list<CComponent*> ctor / dtor family (L17 / L18 binders):** 0x0042ABCA: 31 rows still bind ??1?$list (work/cls_L8.tsv: 9
   DTOR_OUTOFLINE + DEFAULT_CTOR_NOINLINE, 9 DTOR_OUTOFLINE, 5 DTOR_OUTOFLINE without a direct header include, 23 NOHDR, 4 local list
   models ...); 0x0042ABAF: 10 bind ??0?$list + 0x005C3947 ??0CComponentList. Recipe for header rows: swap DTOR_OUTOFLINE (+
   DEFAULT_CTOR_NOINLINE) for BASE_OUTOFLINE (0x004DE6ED shows it: list's inline ~list calls ~_List_base out of line); but
   BASE_OUTOFLINE applies to every list in the TU, so a new member-specific opt-in may be needed. A dedicated pass with every row locked.
   0x004046B0 (list<CStringParserCommentBoundaries>, ~70 refs) is the same family shape.
2. Self ports with the binder already right (one row each): L34, L71, L72, L74, L83, L86, L89, L92, L100, L104, L107-L110, L115, L117
   (the L14 clear recipe), and the virtual-slot rows L29, L32, L50, L95, L96, L101-L103, L118 (vtcheck first).
3. L57 0x00899DB0 (port onto engine/CTCCombat.h), L111 0x00A427A0 (CLipSyncDataBank.h _CLASS + Update), L36, L28, L114.
4. L120 sky builders (one header unit: 0x00B620A0 + its four callees' self rows), L23 / L24 (CGamePlayerInterface Controls typed),
   L27 (fable_boot.h opt-in), L4 / L5, L129 (91 rows), 0x0049D800 (CWorld.h GetLocalSeed opt-in + 0x008EE720 full port).

## 4. Decisions needed from the coordinator
1. The two new headers (hdr/engine/NPlayerGui_CDrawProjectileTargetingCentre.h, NPlayerGui_CDrawMeleeActionButtonEffects.h): add-only
   files from Ego_d field lists; PMeleeAbilityDef is declared only under FABLE_RETAIL_DEF_CLASS_BASE. Accept as new shared headers?
2. L17 / L18 (0x0042ABAF / 0x0042ABCA, marked HOLD-PENDING-DECISION in STATUS, separable from the other 18): the self rows now carry
   the _List_base names while 31 / 10 binders still spell list's (count-neutral, a split that existed). Of those binders, 27 / 9
   define their own pick-any ??1?$list / ??0?$list copy; 4 leave ??1?$list<CComponent*> UND (0x005AE6AF, 0x005AE9DE, 0x005AEAEE,
   0x005B43A2) and 1 leaves ??0?$list UND (0x0042BED4): with the self rows renamed, those resolve only to another row's pick-any
   copy (not the retail owner's body). Land now (NF214 brief: a self rename waits unless every referencer moves) or hold for the
   family pass?
3. The FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST comment in stlport/fable_containers.h cites a Dinkumware PDB as evidence; correct it
   (comment-only) when the family pass lands?
4. L38 / L39 tail-call 0x006A15A0 / 0x006A1620, and 0x006A1620 is a held p27 line (NF238 q12668): leave to NF238, or release?

## Flags lines / funclets / tests / descriptive names
No row needed new flags (every grade used the row's catalog CompilerFlags or its own `// Flags:` line). EH funclets: a grep of
rebuild/src/compiled and rebuild/tests for the 20 source file names finds only a comment mention of 0x0042ABCA's file in 0x00627400;
no row is #included by another.
Tests: rows/004de6ed_test.cpp and rows/007b2990_test.cpp are new (shared headers). The ported self rows 0x00429FBE, 0x0042AADC,
0x0042A141, 0x0042AB1C, 0x0042ABAF, 0x00437640, 0x00437650, 0x004AB490, 0x005F807C, 0x005F88C8 (and 0x0041735A) still PASS on HEAD
host-model tests that do not link the row object: a test follow-up (rewrite each on the shared header, link the object).
No descriptive names introduced: every new spelling is a PDB public or the STLport 4.6 spelling of the shared header (descriptive-by-library).
Catalog renames for the coordinator: RENAMES section 1 (12 rows; 0x00726BE0 keeps its catalog name).
