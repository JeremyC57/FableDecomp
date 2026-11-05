# Phase 7 brief (from phase 6, 2026-11-06)

## State after phase 6 (counts in each wave's NOTES.txt / PASS.tsv)
- w1_getinstance: 27 rows, all PASS; 0x0041E5F2 has one spelling left (?GetInstance@CManager@NUISystem@@SIPAV12@XZ); queue lines
  62 / 568 / 579 / 701 closed, 649 closed_wrong.
- w2_cthing: engine/CThing.h + FABLE_RETAIL_CTHING_GETTC_EXPANDED / _STATEACCESS / _SCANINCLUSION_OUTOFLINE (hdrcheck2 default=same);
  1,760 CThing rows migrated and passing of 4,190 local-model rows; 2,430 deferred (DEFERRED.tsv, rows.tsv). 168 includer
  rows regraded PASS with the overlay.
- Index post files (from the phase start index, HEAD b7d0361): idxpost/MULTINAME.tsv (16,420 targets: 14,994 fable conflict, 80 fable ICF, 1,346 vendor conflict), idxpost/QUEUE_STATUS.tsv (5,148 not-done lines: 33 satisfied, 2,866 partial, 652 open, 1,597 unparseable).
- Waves 3 (CIntelligentPointer leftovers) and 4 (CTCBase, CThingCreatureBase) were not started: the CThing wave took the tooling budget.

## CThing: what is left and how to take it (the deferral classes of w2_cthing/rows.tsv, largest first)
1. header_lacks (754 rows): members the local models declare that engine/CThing.h does not have, or offsets without a public accessor.
   Top names: `Dead(other bits)` / `m_dead(other bits)` (rows testing +0x91..+0x93 masks the header has no accessor for, e.g. 0x21 =
   Dead|InLimbo, or masks on +0x92/+0x93: add the FableWin inline accessors that are missing, or a `GetStateFlags()`-free idiom), the
   lookup wrappers rows declared on their own CThing model (GetInterface / FindEntry / GetModeManager / GetTCConst / GetUID / Create /
   SetAsDrawable / DrawIsDrawable / SetCurrentAction -- derived-class members declared on the CThing stand-in), the members without a
   public accessor (Components +0x44, PDef/PBaseDef +0x70 -> add `PeekBaseDef()` use, PTCDriver +0x5c -> GetTCDriver under
   FABLE_RETAIL_CTHING_GETTCDRIVER, ScriptName +0x74, m_presentBits +0x20, m_componentsLast +0x48 = Components.end()), and
   `operator new(size_t, bool)` (0x004C7380 CThing::operator new, 13 rows: add it to the header behind a sub-macro).
2. local_std (567 rows): a local `namespace std { ... }` model (bitset / vector / pair / list stand-ins) that clashes with the
   STLport headers CThing.h brings. Replace by stlport/fable_containers.h models row by row (the CVectorMap wave did this for the map).
3. closure_model CDefPointer (460 rows): a local `template<class T> class CDefPointer` (or a non-template stand-in) whose pointee
   stand-ins do not derive CDefPointeeBase. Needs the pointee classes modelled on engine/CDefPointeeBase.h (0x1c bytes: vftable,
   RefCount at +4 ...) before the local CDefPointer can go.
4. quarantine_model (104 rows): `#define CThing CThing_PDB` + engine/_quarantine/CThing.h, or a `#define CClass DonorCClass`
   alias. Rewrite by hand from the quarantine model to the header.
5. searchtools_template (103 rows): the CThingSearchTools GetAllThings / GetPBestThing / GetPFirstThing bodies inline
   IsIncludedInThingScans with a different shape (`test al,4 ; je ; not cl ; test cl,1 ; ... test al,0x20`) from the header's
   `IncludeInThingScans && Activated && !Dead && !InLimbo` (`test al,0x21`). A second expression form of the inline body behind a
   sub-macro (or the out-of-line declaration) reproduces them; measure both.
6. template_instance_row (38 rows): rows that ARE an out-of-line GetTC<T> instantiation (Leaf ??$GetInterface@V<T>@@@CThing@@...,
   86 bytes, a runtime id). The header's HasTC is STLport's range-checked bitset::test (`cmp edi,0x112 ; jb ; push ; call
   __stl_throw_out_of_range`), retail's is the unchecked word test: the row needs `_Unchecked_test` (or a FABLE_RETAIL_STLP_BITSET_
   opt-in) and the PDB's one-argument spelling ??$GetTC@V<T>@@@CThing@@QAE_NPAPAV<T>@@@Z. 7 more rows (gate `TARGETS differ (0 vs
   index N)`, e.g. 0x008990C0) fail only because the range check splits the function into `$L` label blocks that
   reloc_identity.function_relocs does not merge: the same fix clears them.
7. closure_derived / local_base / nested_in_namespace / own_definition / pack_region: small classes, by hand.
8. The one-argument migration: GetTC(TCI_X, &v) is FableWin's ??$GetTC@VCTCBase@@ shape; the PDB's calls are GetTC<T>(T**) with
   T::StatGetInterfaceType(). Once the TC classes have engine headers (CTCBase.h derivatives with StatGetInterfaceType), respell the
   911 GETTC_EXPANDED rows to the one-argument form (the bytes are the same: the id folds).
9. Gate FAIL leftovers of the rewritten rows (DEFERRED.tsv `compile_fail` / `bytes_differ`): 248 rows, each with the compiler's
   first error; the commonest are const lookups whose result the row returns non-const (the index binds the const LowerBound
   0x004365B0: retail's receiver was const; the row's helper signature or a cast must be decided by hand), the NGraphicAppearance
   stand-ins defined after their first use, and derived-class pads that the header's 0x98 base shifts (C2118 in CClass.h = a
   FABLE_STATIC_ASSERT on a size the row's own layout no longer meets).

## Other leads found in phase 6 (not acted on)
- 0x00639E40 is bound as `?GiveGold@CTCHeroMoney@@QAEXJ@Z` (stand-in) while its interface number 0x8e is TCI_REGION_DISPLAY
  (ETCInterfaceType.h) and the catalog label is CTCRegionDisplay::GlobalDraw: one of the three names is wrong (naming queue).
- Gate NAME_CLASH notes (w2 logs/gate_*.log): `?GetPlayer@CThing@@QAEAAVCPlayer@@XZ` bound to 0x004C7A10 (= PeekPlayer) in 27 rows;
  `?CreateComponent@CManager@NUISystem@@QAEPAUCArrowComponent@2@J@Z` at 0x0041D21B; placeholder namespaces NScreen<addr> /
  PhotoInput / RetailUI; Slot<n> virtual names.
- CThing::operator new(size_t, bool) 0x004C7380 has no header declaration (13 Alloc rows).
- `#define __forceinline inline` / `#define __declspec(x)` in 87 CThing rows: the rows redefine keywords the header relies on; they
  pass the gate today but should lose the defines when their stand-ins are migrated.
- 725 CThing rows had no `// Leaf:` line; the wave adds the index symbol. The same gap exists outside CThing (index LEAFGUESS.tsv).

## Tooling (handback/tools, all phase 6, documented in each header)
wavelib.py (shared), hdrmig6.py (the CThing rewriter; its rules generalise to the next class: FIELD / BITACC / MACRO_OF / CLOSURE
tables), gatewave.py (gate batches NJ=2, --redo), vlandwave.py (vland --recheck-landed batches, --before baseline), conscheck.py
(one target one spelling, OK(pdb) rule), handback.py (the apply_w35c.py format), idxpost.py (MULTINAME / QUEUE_STATUS), objdiff.py
(index object v rewrite disassembly diff: the first tool to reach for on a BYTES / LENGTH failure), gatestats.py, survey_cthing*.py,
w1_respell.py.
