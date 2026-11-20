# NF146 notes (round 10: CAFile / CWideString remainders and nf141 follow-ups)

Counts (APPLIED.tsv / STATUS.tsv / recheck_final.log: 3 MATCH + 10 RELOCATION_MATCH, 13 PASS): 5 entries: 4 done (q4283, q4285, q11812, q11818), 1 blocked (q11817, decision
item 1). Rows changed and handed back: 13 (rows/), all in one recheck_final.log run. Census / link-input rows listed in STATUS as
not-a-binder or unchanged; 0x00994930 blocked (not handed back); 0x00BF3B60 probed only (not changed). Header opt-ins: none (hdr/ empty).
selfcheck.log: 0 problems.

## q4283 (CAFile::ConvertPathToFullPath 0x00999110): done
- 0x009F2DD0 self leaf -> PDB ?LoadFromBitmapFileToPreallocatedSurface@CSurface@@QAE_NABVCWideString@@K@Z (Ego_r 0x00B46AA0 + FableWin
  0x030C9340); ported onto engine/CSurface.h (FABLE_RETAIL_CSURFACE + _BITMAP; engine/C2DExtentsI.h first) + engine/CAFile.h
  (FABLE_RETAIL_CAFILE) + fable_string.h; form 1 RELOCATION_MATCH. Binds 0x00999110 as the claim; 0x0099AF00 / 0x0099B510 as the plain
  CWideString spellings other rows bind (the N_0099af00 namespaced model is gone). Object defines only its leaf. Test rewritten on the headers.
- Its 7 exe-scan referencers (work/refs2.tsv), compiled from the working tree (work/bindchk_9f2dd0.txt): 0x00642820, 0x00494900, 0x0049A0B0,
  0x0041E6D3 and 0x0066CA80 bind the PDB spelling; 0x00885F50 and 0x0055FC90 bound a void (QAEX) spelling from their existing local CSurface
  models: respelled in place (void -> bool; result unused; defined symbols identical to HEAD). 0x0066CA80 (lock NF142, its NF142 text in
  the tree) was not touched.
- 0x00999110 census: NF129 / NF143 found 15 of 17 REFS rows bound the claim; none changed since 11-19; 0x00A0B710 (NF143) is in the tree now
  with the claim; 0x009F2DD0 here: all 17.

## q4285 (CWideString(const unsigned short*) 0x0099B6B0): done
- 0x009C7A60 ported off the row-local NGameText::CWideString / CWideStringData model (+ its four same-TU noinline copies of real CWideString
  members) onto fable_string.h (FABLE_RETAIL_CWIDESTRING). The header-only form matched first time, so no same-TU strong copy (nf96a) was
  needed and none is added. Own leaf -> PDB ?ConstructStringInternal@NGameText@@YA_NAAVCWideString@@JZZ (Ego_r 0x00B157D0 + FableWin
  0x02F6E140; `long first`, was int). Callees on the plain CWideString spellings (RENAMES s2), the made-up FormatIndex helper replaced by the
  PDB inline GFIntToWideString (engine/GFIntToWideString.h), the separator formatter 0x00A0DAC0 bound under its landed self spelling
  ?FormatNumberWithSeparators@@YIXJPAG@Z (descriptive, existing; the NGameText stand-in spelling leaves the repo: that address's set 2 -> 1),
  and the format data stand-ins replaced by the retail literals L"%d" (0x0122DE44) / L"%" (0x0129B470), read from Fable.exe (the NF143
  precedent for 0x0122DE44).
- Self rename family (exe scan, work/refs3.tsv): 0x009C7A60 is referenced by 0x009C7B70, 0x009C7B80, 0x009C7BA0. 0x009C7BA0 already bound the
  PDB spelling. 0x009C7B70 / 0x009C7B80 were made-up stand-ins (void ConstructString(void*, void*) / extern "C" FableConstructString_9c7b80)
  calling stand-ins for 0x009C7A60: re-authored as the PDB overloads ?ConstructString@NGameText@@YI_NAAVCWideString@@J@Z / ..JJ@Z (both
  PDBs) in the 0x009C7BA0 form. Their referencers (work/bindchk_7b70.txt): 13 + 4 bound the PDB spellings; 0x0072E710 / 0x0072E940 (global
  pointer spelling) and 0x004A0370 (global reference spelling) respelled in place to the NGameText declarations (same bytes, same defined
  symbols). Every address in the family ends on one spelling.
- One definition: 0x009C7A60 emits GFIntToWideString pick-any; its owner 0x00405F00 is pick-any in the tree now (NF143, nf143). The literals
  are pick-any COMDATs.

## q11812 (0x00964360): done
- Leaf-only self rename: `_Methods` stand-in removed; defines ?FindPStealableItem@CAIStateGroup_StealStealableItem@@ABEPAVCThing@@XZ through the
  landed NF141 opt-in _FINDPSTEALABLEITEM. The search keeps CThingObject as its template argument: the 0x00964230 / 0x008FD000 / 0x004C73D0
  pairs are identical to HEAD; objdump -t differs only in the own leaf. Only referrer 0x00964DA0 binds the new spelling (reloc_bindings).

## q11817 (0x00994930 PBEJ -> PBXK): blocked, decision item 1 (not handed back)
- Link matrix in work/link (LINK= cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED):
  - ctl_strong.log: NF141's strong draft + 0x00BF3B60 HEAD object -> LNK2005 + LNK1169 (exit 145).
  - ctl_any.log / ctl_any_rev.log: the pick-any scaffolding draft (work/d/00994930_any.cpp: ctor `inline` + anonymous-namespace
    inline_depth(0) helper; the ctor COMDAT is selection 2) + 0x00BF3B60 HEAD -> LNK2005 + LNK1169 in both link orders: 0x00BF3B60 defines
    the ctor at selection 1 (nodup), and nodup vs any is LNK2005.
  - probe_inline_any.log: pick-any draft + 0x00BF3B60 with its ctor made `inline` -> exit 0, but 0x00BF3B60 then emits ITS OWN 0x19-byte
    body (Data / Size / Position layout) under the same pick-any symbol: a wrong-pick hazard, not a fix.
- Probe work/d/00bf3b60_probe_decl.cpp: with 0x00BF3B60's ctor definition removed (declaration only), LoadForegroundPatch calls the
  constructor out of line, as retail does (0x00BF3B60 is an exe-scan referencer of 0x00994930), where HEAD expands it. q.py still grades the
  wrong function in that no-Leaf row (DIFFER 825v1480; HEAD 809v1480).

## q11818 (one definition of the STLport list-iterator base ctor): done
- 0x0041C780 moved onto stlport/fable_containers.h with the nf86 / nf88 scaffolding (inline_depth(0) helper): MATCH, ctor now COMDAT
  selection 2. Link proof: work/link/ctl_q11818.log (HEAD 0x0041C780 + 0x00913F30 / 0x00964CA0 HEAD + 0x00964360 new: 3 x LNK2005, exit 145)
  vs new_q11818.log (exit 0, LNK4088 only). No other row defines the leaf (grep of Leaf lines and out-of-class definitions).
- 0x0041C7A0 / 0x0041C7C0 checked the same way: they defined stand-ins (?Step@Iter@@QAEXXZ, ?NotEqual@CP@@QBE_NPBU1@@Z), so no clash, but
  every exe-scan referencer binds the STLport spellings (work/bindchk_c7a0.txt + 0x0041E6D3's HEAD object). Both moved to the same
  scaffolding (address-taking helpers) with leaf-only self renames to ?_M_incr@_List_iterator_base@std@@QAEXXZ /
  ??9_List_iterator_base@std@@QBE_NABU01@@Z: MATCH; catalog names fixed (RENAMES s1); the vland STANDIN_LEAF warning is the old catalog
  name. Names descriptive-by-library (STLport; both PDBs are Dinkumware), proven by the bodies and the callers.

## One definition across the hand-back (context link)
- work/link/context_link.log: the 13 new objects (pS) + 30 working-tree referencer / owner objects (pH: the 0x009F2DD0, 0x009C7B70 / 0x009C7B80,
  0x009C7A60, 0x0041C7A0 / 0x0041C7C0 referencers, 0x00964DA0, 0x00405F00, 0x00913F30, 0x00964CA0, 0x00BF3B60), LINK= cleared, /DLL /NOENTRY
  /NODEFAULTLIB /FORCE:UNRESOLVED: 2 LNK2005, both between untouched reference rows (0x005AD571 vs 0x0041E6D3 on
  GetSubDef<CWeaponDef>, 0x006DE8C0 vs 0x006DE280 on CTCTrophy::PlaySoundW; pre-existing), none involving a new object.
- Source grep: no other landed row defines _List_iterator_base::_M_incr / operator!= / the constructor out of class, or the J / JJ
  ConstructString overloads or ConstructStringInternal (the other ConstructString definitions are different overloads: 0x009C7980,
  0x009C7C20, 0x009C7BA0, and the global stand-in 0x00A35B50).

## Locks claimed beyond rows.txt
0x00885F50, 0x009C7B70, 0x009C7B80, 0x0072E710, 0x0072E940, 0x004A0370, 0x0041C7A0, 0x0041C7C0 (free); 0x0055FC90 (`released`). None is
in UNMATCHED_ROWS.tsv; COORDINATION_CLAIMS has no Account B line for any of them.

## Self renames (RENAMES section 1) and their census
- 0x009F2DD0 (7 referencers, all on the new spelling), 0x009C7A60 (3), 0x009C7B70 (15), 0x009C7B80 (5), 0x00964360 (1; 0x00964DA0
  compiled from the tree binds it), 0x0041C7A0 (4), 0x0041C7C0 (4): every exe-scan referencer binds the new spelling after this hand-back.
  0x0041C780: leaf unchanged, so it is not in section 1 (the NF141 / NF143 convention for leaf-unchanged scaffolding rows).
- Rows outside the entries renamed: 0x009C7B70 / 0x009C7B80 (both PDBs agree; q4285 family) and 0x0041C7A0 / 0x0041C7C0 (STLport; q11818).

## EH funclets
- No source under rebuild/src or rebuild/tests includes the file of any changed row (grep of each file name).

## Tests
- Rewritten on the shared headers: 0x009F2DD0, 0x009C7A60, 0x009C7B70, 0x009C7B80 (link the row object; fakes for the callees),
  0x0041C7A0, 0x0041C7C0 (behaviour tests on stlport/fable_containers.h replacing print-only stubs). PassPatterns unchanged
  (0x009C7B80 keeps CONSTRSTR_OK).
- Unchanged (standalone, PASS): 0x00885F50, 0x0055FC90, 0x0072E710, 0x0072E940, 0x004A0370, 0x00964360 (a verbatim copy of its old source
  with an inlined generated header), 0x0041C780.

## Flags lines
None added or changed.

## Descriptive names
- No new made-up names. Scaffolding helpers (anonymous namespace, internal, `// Fidelity: scaffolding, not retail`):
  ReferenceListIteratorBaseCtor (0x0041C780), ReferenceListIteratorIncr (0x0041C7A0), ReferenceListIteratorNotEqual (0x0041C7C0).
- Existing descriptive names reused, not created: FormatNumberWithSeparators (0x00A0DAC0's landed self leaf).
- Removed: FormatIndex (0x009C7A60's made-up helper, now GFIntToWideString), the ConstructStringHelper / FableStringCtor_9c7b80 /
  FableConstructString_9c7b80 stand-ins, Iter::Step, CP::NotEqual.

## Decisions for the coordinator
1. q11817: the nf86 / nf88 pick-any scaffolding on 0x00994930 cannot land the PBXK rename while 0x00BF3B60 (old no-Leaf landscape row,
   candidates/landscape_patch_load_foreground_compiled.h) defines ??0CMemoryDataInputStream@@QAE@PBXK@Z at COMDAT selection 1 (link logs
   above). Proposed: re-port 0x00BF3B60 so it drops its strong CMemoryDataInputStream definitions (ctor, and its Read8/16/32 stand-ins); retail
   calls the ctor out of line there (probe work/d/00bf3b60_probe_decl.cpp). Then NF141's draft (or work/d/00994930_any.cpp) lands as is.
2. q4285 lands after nf143: 0x009C7A60 emits GFIntToWideString pick-any, which only links once 0x00405F00 is NF143's pick-any form (it is in
   the working tree now).
3. q4283 assumes nf142 / nf143 as now in the tree (0x0066CA80 NF142 text, 0x00A0B710 NF143 text).

## Rule slips
- Three Bash calls exited non-zero: an `ls` of NF141's rows directory, a `diff ... && echo SAME` whose files differed, and the lock /
  `grep -c` loop whose last grep counted 0.
- Many read-only Bash calls (cat / grep / objdump / file writes) ran without the full `source env.sh` preamble; every compile, link and
  vland run went through g.sh / bindchk.sh or had the preamble.
- GOTCHAS.md, EXTRA_RULES.md and ASM_RULES.md were grepped for the relevant lines and their headings / recent sections read, not read in full.
- grades_used in STATUS counts q.py compiles (HEAD bindchk + drafts + probes, work/grades.log); the vland runs (b1..b5, recheck_final) are
  not counted.
- Two scratch compiles of 0x00BF3B60 (not locked, not changed, no file handed back) as q11817 link probes; drafts kept in work/d/.
