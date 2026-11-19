# NF96 notes (2026-11-19)

## Read first (coordinator)
- 28 rows in rows/, all MATCH or RELOCATION_MATCH + PASS (vland logs work/b1_q11369.log, b2_q11427.log, b3_q11339.log, b4.log, b5.log;
  recheck_final.log over all 28). selfcheck.log: 0 problems. Overlays in hdr/ (hdr/README.md): six headers, `#ifdef` opt-ins only.
- One-definition items (not spelling; need a coordinator decision): GetCardDef 0x008F0B20 (strong same-TU bodies in 0x008F0B80 [my row,
  kept strong because the declaration-only form gave DIFFER 78v73], 0x008F0BD0, 0x008F0EB0, 0x008F10B0) and 0x008F2280 (defines the
  PDB symbol strongly with a stand-in body); GetCategoryIndexForTradeType (q11204 / q11230, below).
- The vland `WARN RELOC_CONFLICT _$E2` on 0x0121E010 (against landed 0x01219F00) is the TU-local `_$E` leaf collision NF92 recorded for
  0x0121A440 / 0x0121A670 (rebuild/candidates/manual/naming/2026-11-17_agentNF92/NOTES.md line 24; NF89's 0x0121F0E0 / 0x01229BE0 the same).
  The reloc_bindings row `_$E2 -> 0x01228930 (0x01219F00)` is that TU-local name.

## q11369 (anim-component sort TU) -- done
- Ported 0x005E1C30 __make_heap, 0x005E2860 sort_heap, 0x005E2F40 __partial_sort, 0x005E37D0 __introsort_loop onto
  stlport/fable_containers.h (FABLE_RETAIL_STLP_HEAP + FABLE_STLP_FASTCALL, no Flags line). Self leaves are STLport template spellings.
- Checked after the nf95b landing (coordinator note 2): every symbol my four objects leave undefined resolves to a current repo Leaf:
  ??$__adjust_heap@ -> 0x005E1210 (its Leaf line is a prefix of the full symbol, pre-existing), ??$__pop_heap@ -> 0x005E1B60,
  ??$__make_heap@ / ??$sort_heap@ / ??$__partial_sort@ -> my rows. 0x005E2FB0 / 0x005E3550 (nf95b) are the CAnimationEntry
  instantiation (pair<unsigned long,CAnimationEntry>): a different family; none of my rows binds them and they bind none of mine.
- Cut below (NAME_FIXES): 0x005E19D0 (Leaf `__unguarded_partition`, still a placeholder in the repo; 0x005E37D0 now binds the template
  spelling) and 0x005E3AD0 (still a generated Sort_SElement8 placeholder; binds 0x005E37D0 as ?IntrosortLoop_005e37d0@@..., wrong before
  and after this change). Follow-ups: 0x005E0760 swap / 0x005E0FA0 / 0x005E07F0 (the shared std::swap needs a fastcall opt-in; NF95 held
  sibling rows).

## q11427 (particle heap) -- done
- 0x00769D50, 0x00769470, 0x0076A480, 0x0076AF10 + the extra claim 0x0076B530 (it bound 0x00769D50 through an inlined __pop_heap).
  0x0076BE30 binds 0x0076B530 with a struct-key comparator (NAME_FIXES).

## q11339 (GetCardDef callers) -- done
- 25 exe call sites checked (work/gcd_sites.lst). 11 wrong binders ported (30 grades for the family); 13 not-a-binder listed in STATUS.
- Inferred cast: 0x007D4F40 needs `(long)` on vector::size() in a modulo (retail signed `cdq ; idiv` at 0x007D4FA3).
- 0x007D71E0 still models CTCTavernGameCardPairs locally (pre-existing, not changed).

## q11204 / q11230 -- blocked (one definition)
- Retail 0x0059C6B0 is `mov edx,ecx ; call 0x0059B6F4 ; push eax ; mov ecx,edx`: the caller relies on the callee leaving edx alone, which
  VC7.1 assumes only for a strong, already-compiled same-TU definition. NF91 / NF92 measured the header inline + noinline (pick-any)
  body: DIFFER 47v45 (0x0059C6B0), 57v55 (0x0059C710). In q11224 the body was expanded in the caller; here the caller calls it. Needs a
  decision (accept per-TU strong copies, or one TU for the five rows). No grades spent.

## q11443 (0x004DA27F _Rb_tree<long,pair<const long,CDefString>>::_M_erase) -- blocked: port
- Self ported (stlport/fable_map.h explicit instantiation, recipe of 0x006EFCE0; `#pragma optimize("s", on)` kept).
- Exe call sites (nftools/xref.py): 0x004DA291 (recursion), 0x004DB898 in 0x004DB88A (_Rb_tree::clear, NF93, already this spelling),
  0x006DAED2 in 0x006DAEC0, 0x006DBA6F in 0x006DBA50. Retail keeps two identical clear bodies (0x004DB88A, 0x006DAEC0), so no ICF here.
- Left: 0x006DBA50 (??$GFSerialiseMapBinaryIn@VCDefString@@JU?$less@J@std@@@@..., Ego_r 0x0058B160 / FableWin 0x01F0AAC0) binds it through
  a local std::map model; a port also moves map::insert 0x006DB450 and a strong CPersistTraits<long>::TransferBinaryIn 0x0040FE60.
  0x006DAEC0 binds it as ?Detach_004da27f@CTCCarryable@@QAEXI@Z; its own name (map::clear vs _Rb_tree::clear) has one source only.
  Both bound wrong before this change too, so landing 0x004DA27F breaks nothing new.

## q11445 / q11446 / q11449 / q11465 / q11452 -- done
- q11449: all six exe refs to 0x013BA8AC covered (0x00686DC0, 0x00686DF0 [NF95], 0x00686ED0 [claimed], 0x0121E010, 0x01229260).
  0x0121E010 / 0x01229260 use the NF92 recipe (static defined on engine/CTCBase.h; FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE).
  ~list<CTCBase*> 0x004CC220 (placeholder self row) -> NAME_FIXES.
- q11465: self row only (coordinator). Binder 0x0062ABA0 is NF95 / NF97; 0x00590D32 already right.
- q11452: corrected label line in NAME_FIXES (repo is read-only for me).

## Resume point: q11453..q11457 (CEngineTextureMorph vector family, Account B's PROPOSED claims) -- open
- All five self rows and the binders 0x006CA950, 0x006CB470 are locked NF96 (rows.txt). Binder map (nftools/callers.py):
  0x006CA950 -> 0x006CA8C0 (?AddMorph@CTextureMorphs@@... local model); 0x006CB470 -> 0x006CB390 (public QAEX spelling);
  0x006CB390 -> 0x006CB340 (struct-key U?$allocator, QAEXXZ) / 0x006CB0D0 / 0x006CB170 (non-template spellings);
  already right: 0x00728830 -> 0x006CB340, 0x006CA8C0 -> 0x006CB390.
- Evidence caution: FableWin.pdb is Dinkumware (`_Tree`, `_Tmap_traits`), so it cannot be the second source for STLport spellings
  (_M_insert_overflow / _M_clear / __uninitialized_*). q11454..q11457 need a non-PDB second source (stlport/fable_containers.h as landed
  plus landed binders of other instantiations) or go to REJECTED. q11453 (AddEngineMorph) has Ego_r + FableWin.
- Then q11458..q11464, q11466, q11467 in order (ENTRIES give the two sources each claims).

## Descriptive / library names
- No made-up names. Template / STLport names come from the library (fable_containers.h, fable_map.h) plus the callers that bind them.
- `dynamic initializer for 'GlobalUpdateTCs'` / `dynamic atexit destructor ...` are the PDB names (Ego_r / FableWin).

## Rule slips
- 0x007D4F40: one needless regrade (a re-run of the same source).
- Two q11339 drafts lost their `// Leaf:` line to a sed range (caught before grading).
- 0x00686ED0: the first g.py call ran without `source $S/wine/env.sh` (NO_RESULT, no compile ran; counted as a grade in STATUS).
