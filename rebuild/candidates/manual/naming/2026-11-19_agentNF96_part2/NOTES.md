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

## (part 1's resume point, superseded: q11453..q11457 are done in part 2, see the Part 2 section)
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

# Part 2 (resumed NF96, 2026-11-19, after the nf96a landing)
## Read first (part 2, coordinator)
- 27 rows changed in part 2 (PART2_ROWS.txt; rows/), all RELOCATION_MATCH + PASS (vland work/b6..b10.log; recheck_final_part2.log over
  all 27). One test changed: rows/006cb340_test.cpp. selfcheck.log: 0 problems. One new overlay opt-in (hdr/README.md, Part 2 table).
- Entries: q11204 / q11230 done (one definition, the nf96a decision applied); q11339 part-2 closure (GetCardDef owner + copies);
  q11453 / q11459 / q11461 done-already; q11454..q11458 / q11462 done; q11460 blocked (0x006EDA80 only); q11463 / q11464 / q11466 /
  q11467 open (not started: context).
- Resume point: q11463 (0x0137DAD8 NThingMap::LayerInfo). 33 landed rows bind it at +4 as `extern CLayerInfo LayerInfo_0137dad8[]` with
  a local `struct CLayerInfo` (29 identical lines) and 2 as g_MapwhoLevels. Respelling needs NThingMap::CLayerInfo from
  NThingMap_CThingMap.h (CTHINGMAP_LAYERINFO): renaming the local struct into NThingMap would be a NEWLOCAL, so it is a scripted
  35-row header move. Then q11464 (self 0x0095A6C0 + binder 0x0095AD20, both in rows.txt), q11466 and q11467 (label tables only).
- Reloc_bindings is stale for q11453 (0x006CA950 already binds AddEngineMorph) and gains the part-2 respellings (RENAMES s2).
STATUS lines for 0x0059B6F4 / 0x0059C6B0 and the q11339 not-a-binder lines of 0x008F0BD0 / 0EB0 / 10B0 / 2280 were replaced in place
by their part-2 outcome (part 1's text is in naming/2026-11-19_agentNF96_part1/).

## q11230 / q11204 -- done (one definition, advisor decision of landing nf96a)
- q11230: NF91's held owner port (batches/2026-11-17_landing_nf91/held/0059b6f4.cpp) regraded on the current headers: RELOCATION_MATCH +
  PASS (work/b6.log). Only its stale comment changed (it said q11204 was open and 0x0059C747 used the _Retail name). RENAMES s1 carries
  NF91's skipped line.
- q11204: the four TU-mates 0x0059C6B0 / 0x0059C6DD / 0x0059C710 / 0x0059C747 keep same-TU strong copies. (1) PDB spelling: yes.
  (2) Owner body verbatim: work/q11204_bodydiff.txt (diff after the signature line empty for all four; the signature differs only by the
  header's `inline` / `__declspec(noinline)`). (3) `// Fidelity: same-TU strong definition of 0x0059B6F4 (retail TU-mate)` added.
  0x0059CF49 declares only (not-a-binder). Locks: 0x0059C6DD / 0x0059C710 claimed new; 0x0059C747 was `released` (NF94) and re-owned.
- The copies still define CTradeTypeCategory / g_TradeTypeCategories locally (pre-existing; the header has them only under
  TRADECATEGORY_BODY, which those rows cannot define without a second definition). No own header for that struct, so selfcheck is clean.

## GetCardDef 0x008F0B20 one definition -- done
- The owner row itself was the defect too: 0x008F0B20 defined ?GetCardDef@...@VCTavernGameCardBaseDef (non-const CDefPointer) as a
  stand-in on local models and bound GetSubDef 0x008F27A0 by a non-template spelling. Ported (claimed; lock was released) onto the
  0x008F0B80 define set with the real body; it now defines the entry spelling and binds ??$GetSubDef@...QBE (Ego_r + FableWin).
- Owner body = the landed 0x008F0BD0 copy (`new_pdef`, `GetParentThing().PeekBaseDef()`): 0x008F0BD0 cites Ego_d.pdb scopes naming
  pdef / new_pdef (pending/2026-11-17_egod_e10/NOTES.md:64). I could not re-dump Ego_d locals here (llvm-pdbutil reads only its 28 link
  modules), so the local name rests on that one record; it changes no bytes. GetParentThing() is CTCBase's unconditional inline accessor
  (FableWin.pdb), same code as `ParentThing->`.
- Copies made verbatim + Fidelity: 0x008F0B80, 0x008F0EB0, 0x008F10B0 (were `def` / `ParentThing->`), 0x008F0BD0 (Fidelity only).
  0x008F10B0 also carries GetPackPosition 0x008F0B80's body: verbatim already, Fidelity line added.
- 0x008F2280 (stand-in GetCardDef body over a made-up FindCardDef + a dozen local models): ported onto 0x008F0EB0's define / include set
  plus FABLE_RETAIL_STLP_VECTOR_RANGE_INSERT; first form RELOCATION_MATCH. Self leaf QAEXXZ -> PDB UAEXXZ (0x007DA070 already binds it);
  0x008F28A0 now bound with its allocator argument (its own Leaf). Old drafts: work/f2280/.
- Left (not GetCardDef; record only): CCard::Move 0x008F0660 has same-TU strong copies in 0x008F0BD0 (Ego_d parameter names new_pos /
  immediate, differs from the owner's position / immediately), 0x008F0EB0 and 0x008F2280 (owner text verbatim), and the owner row
  0x008F0660 models CCard locally. A Move one-definition pass (Fidelity lines + owner spelling of the parameters) is a follow-up.

## q11453..q11457 (CEngineTextureMorph vector family, Account B's hand4f_b5 claims) -- 1 done-already, 4 done
- q11453: nothing to change. The self row and its one binder 0x006CA950 (exe site 0x006CA96E) already use the PDB spelling. 0x006CA950's
  local CEngineTextureMorphSet declares the protected AddEngineMorph, and one g.py grade of the repo source shows the relocation. Only
  reloc_bindings.tsv still holds the stale ?AddMorph@CTextureMorphs@@ line (coordinator: rebind).
- q11454..q11457: STLport spellings. FableWin.pdb is Dinkumware and Ego_r.pdb has no STLport vector members, so no PDB names them. The two
  sources are (1) stlport/fable_containers.h as the STLport 4.6.2 model (_M_insert_overflow / _M_clear protected, class allocator,
  __uninitialized_* function templates), which 258 IAE _M_insert_overflow and 67 IAE _M_clear bindings of other instantiations in
  reloc_bindings follow, and (2) the landed binders 0x006CA8C0 (B) and 0x00728830, which bind these exact spellings. Each self row also
  grades RELOCATION_MATCH from the header.
- Ports: 0x006CB390 and 0x006CB340 are now explicit instantiations on the 0x006CA8C0 define set. 0x006CB390 now binds _M_clear, both
  helpers and vector<CCompositeBlendDesc>'s copy constructor 0x005BB346 by their library / PDB spellings. B's NAME_FIXES had listed that
  constructor's struct-key spelling from this row. 0x006CB470 is now the fable_containers.h push_back: it binds IAEX, and its self leaf
  is unchanged. 0x006CB0D0 and 0x006CB170 already defined the entry spellings.
- 0x006CB340's test linked the old public one-parameter symbol, which gave LINK_FAIL. Respelled minimally: a two-parameter vector,
  protected _M_clear and `friend int ::main();`. PassPattern kept, no new local class.
- Open name question, not claimed (one source): 0x006CB470 has no E8/E9 caller. Ego_r / FableWin name only Lionhead's
  ?push_back@?$CArray@VCEngineTextureMorph@@@@QAEXABVCEngineTextureMorph@@@Z (Ego_r 0x0056D7D0) for this element type, so the row may be
  CArray<CEngineTextureMorph>::push_back rather than std::vector's. That needs a second source.
- 0x008F2280's self leaf moved QAEXXZ -> UAEXXZ: its exe refs (xref.py) are the jmp from 0x007DA070 (already binds UAEXXZ) and three
  vtable slots (.rdata 0x0126F684 / 0x0126F7CC / 0x01288364); no direct call from another row.

## q11458 (0x005BF221 CCategory::RemoveItem) -- done
- The self row still defined ?RemoveItem@CCategory_Retail@@QAEXJJ@Z on a local view; ported onto NInventory_CTCInventoryBase.h +
  CCategory.h with the 0x005BE85B define set (PASS22 retail layout, CIsSameItemType, out-of-line find_if) + a new overlay opt-in
  FABLE_RETAIL_NINVENTORY_CITEM_REMOVEITEM (CItem::RemoveItem(long), Ego_r + FableWin). First form RELOCATION_MATCH + PASS (work/b9.log).
- Binders already right: 0x0059C747 (NF94) and 0x005BF3C6 (NF93's port; the entry's `CCategory_Retail` binding there is gone). I briefly
  wrote NF96 into the released 0x005BF3C6 lock and put `released` back (no edit made; it is in NF93's old rows.txt).
- 0x005C190B's catalog label is a propagated `_Ucopy` (vland WARN RELOC_NAME): NAME_FIXES.

## q11459 (0x009A08B0 GFFloatToCharString) -- done-already
- Not landed (UNMATCHED parked, name right). reloc_bindings has only the PDB spelling (binder 0x0099FA90); NF94 respelled every landed
  declaration; 31 rows keep the old FormatDouble name in comments only (not changed).

## q11460 (0x00747B80 CTCStealth::SetAsInDanger) -- blocked (one row left: 0x006EDA80)
- 31 exe call sites (work/sad_sites.txt). Self + 11 binders respelled to ?SetAsInDanger@CTCStealth@@QAEXABVCThing@@W4EDangerStatus@NStealth@@@Z,
  all RELOCATION_MATCH + PASS (work/b10.log). Transformer t/sad.py (proved by hand on 0x0093C830 / 0x006ED9E0 / 0x009621F0 first): it
  respells the local CTCStealth declaration to (const CThing&, NStealth::EDangerStatus), passes `*ptr` and the Ego_d enumerator, and puts a
  `namespace NStealth { enum EDangerStatus {...}; }` with the CTCStealth.h names in place of the row's global enum. The landed siblings
  (0x00956C00 ...) use the same local-enum recipe.
- 0x0093BDF0 / 0x0093C230 / 0x0093C420 called 0x00747B80 as a made-up `struct CTCSeekTarget::RemoveSeeker(CThing*, long)`. Renaming that
  stand-in to CTCStealth would be a NEWLOCAL, so they now include engine/CTCStealth.h (+ FABLE_RETAIL_CTCBASE for its layout, +
  FABLE_RETAIL_NSTEALTH_EDANGERSTATUS) and the stand-in class is gone.
- Blocked: 0x006EDA80 RegisterAttackingCombatant. Its local CThingCreatureBase is not derived from CThing (it pads CThing's members
  itself), so the PDB (const CThing&) call needs engine/CThingCreatureBase.h (a dozen-member move). No grade spent.
- Unlanded sites: 0x0093BC87, 0x0096B1E2, 0x0096E0F8 (gaps after 0x0093B8B0 / 0x0096AF60 / 0x0096DDB0).

## q11461 (0x013D2D70 N3DMeshAllocators::ClothMemoryPool) -- done-already
- NF94 respelled all six binders. reloc_bindings has only the Ego_d spelling, and I confirmed the exact string in Ego_d.pdb (grep -a).
  No label line for 0x013D2D70 exists in ghidra_out/labels_*.

## q11462 (0x00803540 CTCTargetingPlayer::UpdateTargetedPositionRandomWalk) -- done (label tables only)
- No row (parked). The binder 0x00805600 is right. NAME_FIXES gives the label fix for labels_forge_autoname4.tsv:667 and the engine_api.tsv
  name column at line 20669. Ego_r and FableWin give the same spelling (pdb.sh).

## Part 2 checks and records
- Condition 2 for GetCardDef / GetPackPosition: work/getcarddef_bodydiff.txt. Every copy (0x008F0B80 / 0BD0 / 0EB0 / 10B0 / 2280
  against the owner 0x008F0B20; 10B0 / 2280 against GetPackPosition 0x008F0B80) is IDENTICAL to the owner body. For q11204 the record is
  work/q11204_bodydiff.txt.
- New bindings checked against their owners' Leaf lines: GetSubDef 0x008F27A0 and vector<CCompositeBlendDesc>'s copy constructor
  0x005BB346 define exactly what 0x008F0B20 / 0x006CB390 / 0x006CB470 now bind.
- rows/ still holds the 27 part-1 files (all `cmp`-identical to the repo, plus 008f0b80 which part 2 changed). Land only PART2_ROWS.txt
  (27 rows) + rows/006cb340_test.cpp.
- STATUS lines replaced in place by part 2: 0x0059B6F4, 0x0059C6B0, 0x008F0B80 (part-1 row), 0x008F0BD0 / 0EB0 / 10B0 / 2280.
- recheck_final_part2.log: 27 RELOCATION_MATCH + PASS. Its WARN lines are all pre-existing:
  - SHARED_HEADER_AVAILABLE marks local models the rows already had.
  - RELOC_NAME lines are label / catalog mismatches on callees my edits did not touch. The one exception is 0x005C190B (find_if, now
    bound by 0x005BF221), which has a NAME_FIXES line.
  - The single RELOC_CONFLICT (`_mult_to_stay_off_table` on 0x008F0BD0 against 0x007DAEF0) is a TU-local file static that the landed
    rows already had.
- The q11460 self rename breaks nothing new: the blocked 0x006EDA80 bound a wrong spelling before too.
- Old leaves derived from source, not from an object: 0x008F0B20, 0x006CB390, 0x006CB340, 0x005BF221 (RENAMES old_leaf / s2 old_symbol).

## Part 2 rule slips
- I wrote NF96 into the released 0x005BF3C6 lock (NF93's old rows.txt) and then restored `released`; that row was not edited.
- I re-owned `released` locks (0x0059C747, 0x008F0B20) by overwriting the owner file instead of with an atomic mkdir.
- Several read-only Bash calls ran without the env line. No grade was affected.
- I wrote two scratch files outside the workspace, in the session scratchpad (an llvm-pdbutil module dump and a g++ syntax copy). Both
  are deleted.
- 0x006CB340's first vland was a LINK_FAIL because I did not check the test for the old symbol before grading.
- One duplicate st.py STATUS call for 0x0059B6F4 (same line rewritten, harmless).
