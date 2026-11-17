# NF87 notes (q11121..q11149, the NF85 follow-ups)

All numbers below were counted from STATUS.tsv, APPLIED.tsv, RENAMES.tsv and recheck_final.log at hand-back.

- STATUS.tsv has 91 rows: the 83 in rows.txt plus 8 claimed extras.
  - ported: 54. All 54 grade RELOCATION_MATCH or MATCH with behaviour PASS in recheck_final.log (one vland run over rows/).
  - not-a-binder: 34.
  - blocked: 3.
  - deferred: 0.
- Extras, all claimed with claim.sh:
  - ported: 0x0077F4C0, 0x0077F4E0, 0x007ACD30, 0x008ABD80, 0x009EE470, 0x00A344E0, 0x00A72AA0.
  - not-a-binder: 0x00A33A70.
- APPLIED.tsv: 26 entries done, 3 blocked.
  - q11122: 0x004B4770 is blocked.
  - q11127: 0x006BC510 is blocked.
  - q11128: 0x0071D3B0 is blocked.
- RENAMES.tsv: 42 section-1 lines and 95 section-2 lines.
- NAME_FIXES.tsv: 14 lines, 3 of them added in this round, for the heap-family callers that still need a family port.
- REJECTED.tsv: 1 line, the stale WORKLIST pair (q11135, 0x008AE510).

## Descriptive names
- No made-up names were introduced. Every own name comes from Ego_r.pdb plus FableWin.pdb, or is an STLport library name.
- STLport instance names are descriptive-by-library: the template name is STLport 4.6's, and the instance (element, comparator) is
  proven from the callee/caller bindings. This covers:
  - every std::sort_heap, __make_heap, __pop_heap, __partial_sort and sort row;
  - the _Rb_tree_iterator operator++ / operator-- rows;
  - _M_upper_bound and equal_range;
  - the vector copy ctor and _Construct.
- CVectorMap Sort / Update for CSoundBankLH* and C3DVector (0x00A42040 / 0x00A420C0, 0x0077F4C0 / 0x0077F4E0) are Ego_r.pdb member
  names on an instance that is proven only from bindings (descriptive-by-library).
- A data spelling changed in 0x009EB0D0 (section 2): the shared fable_string.h names the reference to 0x0129AAF4
  `?NullChar@CCharString@@1DB`, where the old row used the `""` literal.

## Per row (this round's later entries)
- 0x00EE8870 OakValeFire:
  - Moved onto CCS_OakValeRevisitedScript.h with the OAKVALEFLAG_MAP opt-in.
  - Retail expands the implicit CScriptThing::operator= at the CreateEffect store. VC7.1 calls it at the default /d2inlT74 and expands
    it at T76, T78, T82 and T90.
  - The row uses `// Flags: /d2inlT76` with an `// Inline-budget:` marker. This is option (2) of the owner's 2026-11-08 inline-budget
    rule. T76 is the smallest value tested; T75 was not tried.
  - Bisect probes showed that each part of the first half inlines alone and the whole half does not, so this is a budget split.
  - The row took 16 grades (slip 3).
- 0x009A8150: renamed from the descriptive CStringTable_009a8150::SetEntry to CBankFileManager::SetBankAlias.
  - Ego_r + FableWin give the member. Ego_d gives the parameter names bank_handle and alias.
  - The map at +0x18 is BankAliasMap (Ego_r.pdb).
- 0x00E6DA80 TortureVictim: the master-data flag at +0x10 is CQ_SunnyvaleMasterData::HenchmanSacrificedBodge. The name comes from the
  Ego_r.pdb layout in the shared header; the old row's TortureInProgress was descriptive. The retail store is unchanged.
- 0x00E57020: CV_BookCollectingScript has no shared header. The row declares a minimal local shell: only the nested CBS_Teacher and the
  two members it reads. The landed sibling 0x00E55CE0 also models the class locally.
- 0x009EB0D0: uses NF85's local CConsole model from 0x009EAFF0 (CConsole has no shared header). The unused SetGuessLine helper was
  dropped.
- 0x009EE470 (extra): the STLport operator++ self row. Its only retail E8 caller, 0x009EB15C, is in 0x009EB0D0.
- 0x005A45F6: now on the CThingPlayerCreature.h retail branch, using the recipe of its twin 0x005A731F. The bindings are unchanged.

## Landing notes
- 0x00EE8870 needs catalog CompilerFlags `/d2inlT76` (its `// Flags:` line). vland's payload carried the flag
  (recheck: RELOCATION_MATCH PASS).

## Rule slips (work/slips.txt)
- Slip 1 and slip 2: two Bash calls exited 1, a false `[ -n ]` test and callers.py run outside the repo. No state changed.
- Slip 3: 0x00EE8870 used 16 grades against the 15-grade budget.
- Slip 4: STATUS was written late for 0x009EB0D0 and 0x00A35260, after the next row's grade.

## Follow-ups
These are not in this hand-back. They are listed in NAME_FIXES.tsv.
- 0x00A35490: a __introsort_loop for pair<unsigned long, C3DMeshVolume>, with its __unguarded_partition 0x00A34260. It still models
  std::CKeyPairCompareLess and binds the old 0x00A35260 spelling.
- 0x00A73910 (long) and 0x00A73A00 (CCharString): __introsort_loop rows of the C3DMeshVolume families. They bind the
  pair<.., SNamedChunk> PAV stand-ins, as do their __unguarded_partition / __median bindings.
- Each of these needs a whole-family port, not a respell. Their other callers (0x00A353DC, 0x00A3459E, 0x00A72439, 0x00A71E1E,
  0x00A724EE, 0x00A7373C, 0x00A7375C) have no landed row.

## recheck_final.log warnings (23, all read)
- RELOC_NAME / STANDIN_LEAF lines name catalog labels that this hand-back's section 1 respells, or labels of entry self rows. Every
  STANDIN_LEAF row has a section-1 line except 0x008ACA20. Its leaf is byte-for-byte the landed row's leaf, so the warning is the
  checker comparing the long template catalog name with the demangled leaf, not a rename.
- 0x00D347B0 binds 0x00CD3CCB as `vector<CScriptGameResourceObjectScriptedThingBase>::resize`, exactly as the old row did and as 4 other
  landed rows do (reloc_bindings). The catalog label of 0x00CD3CCB ("CArray<std::pair<long::push_back") is wrong. That is outside
  this hand-back.
- RELOC_ALIAS lines are CRT import thunks (_stricmp, strncmp).
- Binder sweep: reloc_bindings.tsv was checked for every section-1 address. 4 landed callers outside rows/ bind an old spelling, and
  all 4 are in NAME_FIXES (0x005A8A5D -> 0x0059DA10; 0x00A35490, 0x00A73910, 0x00A73A00 -> the three __partial_sort rows).
  Retail pointer references (dir32) to the renamed functions sit in unlanded code. For example 0x00EE83DD holds &OakValeFire. The
  E8 callers of SetBankAlias, 0x004028A9 and four more, are in an unlanded function.

## Blocked
Full reasons are in STATUS.tsv.
- 0x004B4770 ~CQuestManager: CQuestManager.h pads the members the implicit destruction needs.
- 0x006BC510 CRegion::LoadBinary: DIFFER(1386v1480).
  - VisMaps resize is inline in this compile and out of line in retail.
  - SetRegionDefName is inline.
  - It needs a READPRESIZEDSTRING opt-in, which was removed because no finished row uses it (work/notes_removed_optins.txt).
- 0x0071D3B0 UpdateTattoos: CTCHeroMorph.h has no layout.
