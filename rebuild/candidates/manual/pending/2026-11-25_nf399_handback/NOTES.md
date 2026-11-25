# NF399 hand-back (2026-11-25): T3 nf396 lines 0x00578FF5, 0x005796AB, 0x004DF54F

## Summary
- b1 ACCEPT (every gate clean): 0x00578FF5 -> `_Rb_tree<EHeroTrainableStatType, pair<const EHeroTrainableStatType,
  NUISystem::CComponent*>, ...>::_M_erase`, 0x005796AB -> that tree's `~_Rb_tree`. Both spellings were already the table's (lines
  witnessed by 005793e8 / 0057846f), so the self respell changes no binding. No table change, no header change.
- b2 HOLD_RULING Q1 (all gates run; ready as one unit): 0x004DF54F -> `vector<const CSteeringAgent*>::~vector` (MATCH, 5-byte
  `jmp ~_Vector_base 0x004DF554`), binder 0x004DF507 (target 004df54f), census port 0x004DF365, new header opt-in
  `FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_DECL_ONLY` (stlport/fable_containers.h), TABLE_CHANGES (delete `??1CMember_004df54f@@QAE@XZ`,
  add the ~vector line, witness 004df507). Held: once 0x004DF54F carries its name, 0x006CC1E0's unreferenced ~vector copy (local
  model, the expanded 14-byte body) reads NEQ there (R5). Q1 / Q2 in RULINGS_NEEDED.tsv.
- VC7.1 compiles: 54 cl runs (8 evidence probes p1 x3, p2, p3, p4, p5a, p5b; vland pairs a / b / c = 5 rows x HEAD/new, object +
  test each = 20; 22 census objects; 4 for the two equiv-differing rows under the overlay). Clang only for prechecks and census prefilters.
- Locks: kept on 00578ff5 / 005796ab (ACCEPT); released on 004df54f / 004df507 / 004df365 (held), the 22 census rows, and
  0x006CC1E0 / 0x0064E978 / 0x0064AE80 (re-claimed for probes / rechecks, released after).

## 0x00578FF5 (_M_erase)
- References (E8 / E9 / abs scan of all sections, evidence/rawref_3.txt): its own recursion (call 0x00579007) and the call at
  0x005793F6 in clear 0x005793E8 (landed nf396_b2 as this tree's `_Rb_tree::clear`). nf396_b2's table line binds this exact spelling
  here (witness 005793e8). Chain to the owner: clear <- ~_Rb_tree 0x005796AB <- ~CTCInventoryExperience 0x0057846F (UpgradableStats
  +0x224, Ego_r.pdb map<EHeroTrainableStatType, NUISystem::CComponent*>; retail bytes: `mov esi,ecx` at 0x00578471, then
  `lea ecx,[esi+0x224] ; call 0x005796AB` at 0x005784E2, and +0x230 (UpgradableAbilities) -> 0x00579616 just before:
  evidence/retail_0057846f_member_calls.txt). Two sources: the table line (compiled witness) + the caller
  chain to the PDB member; the body is the repo STLport _M_erase (VC7.1 RELOCATION_MATCH, probe p1 and vland).
- Form: explicit instantiation on stlport/fable_map.h (the 0x00527950 form). Old self `?BuildTreeArray@Tree@@QAEXPAUNode@@@Z` is bound
  at the family template 0x0042A1B6 and keeps 5 other (strong) definers (clang census); the row's HEAD copy was EQ at 0x0042A1B6.

## 0x005796AB (~_Rb_tree v ~map, NF389 rule)
- Reference sites (E8 and E9, all .text): `call 0x005796AB` at 0x005784E8 in ~CTCInventoryExperience, and `jmp 0x005796AB` at
  0x00579887. 0x00579887 is not a catalog start: it is a 5-byte body right after the 25-byte map ctor 0x0057986E, and nothing references
  it (rawref_thunks.txt). A map's implicit destructor is only its _M_t member's destruction, so the 5-byte tail jump is the ~map shape
  and its target is the tree destructor. Probe p3 (VC7.1, shared header, the 0x004DF885 recipe) emits
  `??1?$map<EHeroTrainableStatType, NUISystem::CComponent*>` as exactly `jmp ??1?$_Rb_tree<...>` (5 B), and a caller destroying that
  map reaches `??1?$_Rb_tree` directly, as the owner's call at 0x005784E8 does (evidence/probe_p3_map_objdump.txt). The 0x00579887
  identity is an inference (no row, no reference); it is in T3.txt. The same TU repeats the pattern: 0x0057985B `jmp 0x00579616`,
  0x0057A101 `jmp 0x00579FAA` (0x00579FAA landed as ~_Rb_tree by NF195).
- The owner row 0x0057846F hand-declares `~_Rb_tree()` in its local model, so its RELOCATION_MATCH alone would be circular; the
  decision rests on the jmp at 0x00579887 plus probe p3.
- Form: the 0x00579FAA recipe (FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE, explicit ~_Rb_tree). No table add for its clear call: the sibling
  0x00579EA3's clear line is witnessed by 0x00577216, not by 0x00579FAA, and clear 0x005793E8 is a landed self.

## 0x004DF54F (~vector) and its unit
- Default header: the ~vector instantiation inlines ~_Vector_base (DIFFER 14v5, probe p1). Lever: a declaration-only ~_Vector_base,
  the precedent of FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY / FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (the landed 5-byte
  destructor thunks 0x004194B5 / 0x00429B72 / ...). With it: MATCH; `nf.py xr` resolves the jmp to retail 0x004DF554 =
  ??1?$_Vector_base (probe p2). One additive `#elif` in fable_containers.h (hdr/, batches/b2/hdr/, HEADERS.tsv); no landed row defines
  the macro. equiv_check over the 16,112 rows reaching the header: see "equiv_check" below.
- 0x004DF507 (binder): member +8 becomes the shared vector (Ego_r.pdb DummyAgentList) with FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE; the
  leaf is byte-equal with the call target renamed (RELOCATION_MATCH H/N). The model's other stand-ins (base v00 / m_4) are left as they
  were (they shape the emitted vtables; changing them would move more sections); queued in T3.txt.
  objsec (batches/b2/evidence/objsec/004df507.*): vtables EQ; leaf RELOCS only; `??0CObstacleNavigatorExternalInterface@@QAE@XZ`
  (implicit ctor, 9 -> 20) and `MakeOne_004df507` (22 -> 33) change -- both are the vtable scaffolding and now construct a real
  vector (Q2); 9 added COMDATs (vector ctor chain, ~_Vector_base, ~_STLP_alloc_proxy, ~allocator, deallocate) all tier 1 v 004df365
  (R10); its ~_Vector_base copy is EQ at 0x004DF554 in pk_all. `.drectve` gains /DEFAULTLIB:"libcp" (the STL include).
- Census (VC7.1): ~vector<const CSteeringAgent*> copies exist in 0x004DF365 and 0x006CC1E0, both only referenced by .debug$F, both
  the expanded 14-byte body. HEAD: NOADDR; with the new name at 0x004DF54F: NEQ (reach n). 0x004DF365 takes the opt-in (row in b2):
  leaf RELOCATION_MATCH H/N, its copy becomes the jmp (EQ 5=5), ~_Vector_base / deallocate copies are no longer emitted, every other
  section EQ (objsec/004df365.*). 0x006CC1E0 cannot, measured (Q1): p5a (local ~_Vector_base declared only) makes its copy the jmp
  but moves the leaf (DIFFER 121v137); p5b (user-declared FABLE_FORCEINLINE local ~vector) keeps the leaf RELOCATION_MATCH but the
  14-byte copy is still emitted (0x0057846F's FORCEINLINE local ~vector copies are emitted too). evidence/probe/p5_vector_copies.txt.
- Census reach: 112 rows reach the type through headers; 19 name it in their own text (the only ones that can instantiate its
  destructor: the headers only use it in signatures, pointers and CTCCreatureNavigation's member Agents, which is a vector only under
  FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE, used by 0x006CC800 alone (evidence/census/framupdate_optin_users.txt; its VC7.1
  object defines no ~vector), while the class destructor is declared virtual out of line; 0x006CC1E0 models the class locally). All 19 were VC7.1-compiled (2 already in the first census set). Clang -O1 and -O0 prefilters over all 112 found only
  0x006CC1E0 (they miss .debug$F-only copies, e.g. 0x004DF365), so clang was not used to exclude anything for this name.

## Gates
- vland --recheck-landed HEAD and new, one LANDVERIFY_WORK path per pair (work/wd_<tag>h / n): a (00578ff5 005796ab), b (004df54f
  004df507), c (004df365): every row MATCH / RELOCATION_MATCH PASS both sides, tests unchanged and PASS. WARN lines: STANDIN_LEAF /
  RELOC_NAME are catalog labels (relabel queued with the selves); RELOC_CONFLICT on HEAD 00578ff5 is the old stand-in. No WARN
  LITERAL_DIFF anywhere (0 LITERAL lines in all six logs).
- pk_all + pickany_vs_retail on HEAD (H), new (N) and new against the new tables (T: work/troot = repo with TABLE_CHANGES applied and
  the new selves appended to self_symbols.tsv): no NEQ on any changed row; reach flips 0 (`reachflip.py evidence/pk`: 5 pairs).
  Census objects H v T: unchanged except the two ~vector copies above (b2 only); b1 changes no verdict (names already bound).
- R1 / R10 tiers (batches/bN/evidence/scan.tsv, tools/tier396.py): b1 extras TIER2_SMALL (1-byte _Destroy / ~allocator /
  ~_STLP_alloc_proxy) and TIER2_NOT_UNIQUE (deallocate<node> 20 B: 3 masked starts, all resolved; ~_Rb_tree_base 14 B: 1117); b2
  extras all TIER1: against the unit's new 0x004DF365 object (evidence/scan_b_unit.tsv, used in batches/b2/evidence/scan.tsv) 7 v
  004df365 and ~_Vector_base / deallocate v 004df554 (the unit's 004df365 no longer emits those two); against HEAD 004df365 all 9 v
  004df365 (scan_b_vs_HEAD_004df365.tsv). No residue.
- apply_table --dry: b1 empty (0 misses); b2 delete 1 + add 1, 0 misses, multi-target 1 -> 1, none new. Repo untouched (checked).
- /FORCE link HEAD v new, 22 census objects in both sets: b1 as it lands (new b1 + HEAD b2 objects, evidence/link_b1only.txt):
  duplicates 0 -> 0, new-only unresolved none, resolved: the CComponent ~_Rb_tree / _M_erase, ?clear@list_T_005796ab. b2 unit on top
  of b1 (new 004df54f / 004df507 and the unit's new 004df365 object, evidence/link_b2unit.txt): duplicates 0 -> 0, new-only unresolved
  none, resolved ??1CMember_004df54f, _ForwardTarget_004df554. (link_all.txt: the first combined run, HEAD 004df365.)
- Selves: all three COMDAT selection Any (no NoDuplicates self). Removed names: `?BuildTreeArray@Tree@@QAEXPAUNode@@@Z` keeps 5 other
  definers; `??1list_T_005796ab@std@@`, `?clear@list_T_005796ab@std@@`, `??1CMember_004df54f@@`, `_ForwardTarget_004df554` occur in no
  other source or header (text census of rebuild/src, include, tests: only the rows themselves and 005796ab's unchanged test model);
  `_Forward` keeps 78 other definers.

## equiv_check (header opt-in)
- `equiv_check.py` (gcc cpp, both include trees as scratch copies: work/eq/old = repo, work/eq/new = repo + the opt-in) over the 16,112
  rows reaching stlport/fable_containers.h, sources and tests: no row source differs; 2 test files differ
  (0x0064E978, 0x0064AE80 tests). Both `#define abort() fixtureAbort(__LINE__)` themselves, so the header's
  `__stl_throw_out_of_range` abort reports header line 1911 instead of 1905 (the 6 inserted lines). Test-only diagnostic text, no row
  object can change (evidence/equiv_containers.tsv, batches/b2/evidence/equiv_0064e978_test.diff). Both rows rechecked with the
  overlay (vland --recheck-landed, FABLE_HDR_OVERLAY=hdr): RELOCATION_MATCH, test PASS (batches/b2/evidence/vland_equiv_rows_overlay.log).

## Existing debt seen (not holds)
- 0x00579616 carries two table names (??1CAbilityComponentMap@@QAE@XZ v the EHeroAbility ~_Rb_tree): T3.txt.
- 0x004DF365's ??_GCObstacleNavigatorExternalInterface copy (28 B, calls an out-of-line ~CObstacleNavigatorExternalInterface) is not
  0x004DF507's 43-byte body; pk_all does not walk vftables, so it reads nowhere. 0x004DF365 / 0x006CC1E0 HEAD NEQ
  ??_GCNavigatorExternalInterface at 0x004DF532 (unchanged).

## Deviations
- One read-only `git log --oneline -1` early in the session, before I re-read the "no git commands at all" rule; no other git.
- `rm -rf` of four of my own generated subfolders (batches/b1/{rows,orig,head,logs,evidence}) to rerun the assembler after its first
  run stopped half-way; literal paths inside my agent folder, not clean.py. batches/b2/evidence/scan.tsv was rewritten from
  evidence/scan_b.tsv + scan_c.tsv for the same reason (the half run had appended 004df54f's lines once).
- equiv_check was run from the repo root (it reads rebuild/build_candidates.ps1 relative to cwd); it writes only to my folder and
  $S/stubinc_equiv.
- No repo writes.

## Tools (tools/)
- Copied from NF396 (paths / lock owner respelt to NF399): nf.py, nf387.py, prec.sh, evcompile.py, rawref.py, pk1.py, reachset.py,
  clcensus.py, tier396.py, comdats.py, headonly.sh, runpair.sh and the rest of NF396's folder.
- New: runpair399.sh (overlay on the new side only), tier399.py (HOBJ / SCANSUFFIX), clcensus0.py (-O0 prefilter), assemble399.py,
  mktroot399.sh (work/troot), thunk5scan.py (landed 5-byte destructor thunks, evidence/thunk5_rows.txt).
