# NF73 notes (round 7: the nine rows NF66 / NF71 left blocked)

Work dir: $S/w22/agentNF73. Grades in pS/ through g.sh (ledger grades.log; gx.sh = g.sh with the full relocation dump), drafts in
work/att/, relocation dumps work/x_<addr>.txt (port) and work/xo_<addr>.txt (landed baseline). Tools copied from NF71 / NF68 and
repointed. Grade counts in STATUS.tsv count every q.py compile of the row by NF73 (ports, baseline dumps, controls).

## Totals (from STATUS.tsv / APPLIED.tsv / recheck_final.log)
- rows.txt: the 9 slice rows plus one extra row I claimed (0x005955AB, the prerequisite NF66 named), 10 rows in STATUS.tsv.
- Ported: 5 rows (4 slice rows + 0x005955AB). Not-a-binder: 0. Blocked: 5 slice rows (0x0041B550, 0x00549B20, 0x005778CA,
  0x00599E3F, 0x006D53C0). Deferred: 0.
- Entries (15): 4 done (nf41:e36, nf41:e34, q10474, nf41:e172; each target's distinct spellings checked in reloc_bindings.tsv, callers.py
  run on 0x0042BED4 / 0x0042CD84 / 0x0059B5D7 / 0x0049F180), 11 blocked (nf41:e37: slice binder 0x00597006 ported, but the landed
  0x005907AB outside the slice binds 0x0042CD84 as ??0CAction@NUISystem@@QAE@ABV01@@Z, NAME_FIXES.tsv; e125, e124, e164, e154 by 0x0041B550; nf44:76 by
  0x00549B20; nf44:54, nf44:7 by 0x005778CA; q10440, q10441 by 0x00599E3F; e53 by 0x006D53C0).
- RENAMES: section 1 has 3 self renames (0x004091C0 LoadFileList -> GetSortedSaveNamesAndTimes; 0x00596CCE / 0x00597006 leaf-only
  access fixes to the PDB IAEJXZ). Section 2 has 28 respelled bindings, all dumped from objects (landed baseline vs port, xref2).
- NAME_FIXES: 7 entries (the two IAE self spellings for their callers 0x00597B20 / 0x0059A238, ToCharString, CAFile::IsPathFile,
  CAFile::GetFileModifiedTime, ActivateQuest bool, the CDeletion copy ctor for 0x005907AB).
- rows.txt / rows_all.txt: NF73 appended 005955ab (the claimed extra row) so STATUS.tsv carries it.
- grades_used are counted from grades.log (every g.sh / gx.sh run, including dumps; 0x00549B20's 3 include 2 preprocessor stops).

## How the CFrontEndManager pair matched (the mechanism NF51 / NF66 named)
- Retail compiles GetProfileNames 0x005955AB in the CFrontEndManager TU and calls it. With its body visible and kept out of line
  (overlay CFrontEndManager.h GETPROFILENAMES_BODY), and the STLport bodies it calls visible but out of line (push_back NOINLINE,
  _M_insert_overflow OVERFLOW_OUTOFLINE, erase(first, last) the new ERASE_RANGE_NOINLINE), VC7.1 knows the caller's local vector does
  not escape: RefreshAvailableProfiles strength-reduces the loop onto a pointer and a down counter and takes retail's 0x64 frame
  (NF66 a1 + body with the declaration-only STLport opt-ins: still DIFFER 838v824; with the NOINLINE ones: RELOCATION_MATCH).
- RefreshAvailableProfilesForDelete then differed only by ebx (0 constant vs the CList data pointer kept across the loop). The counted
  pointers' ReleaseData declared only (RELEASEDATA_DECL_ONLY) lets the local CCountedPointer<CList> escape; ReleaseData visible but out
  of line (overlay CCountedPointer.h RELEASEDATA_NOINLINE) gives retail's ebx = list Data: RELOCATION_MATCH.
- RefreshAvailableProfiles reloads the list pointer every iteration in retail (register pressure: ebx carries the action type), so it
  matches with RELEASEDATA_DECL_ONLY; I did not switch it to the NOINLINE form.
- 0x00599E3F (CFrontEndManager::Update, same TU) also calls GetProfileNames and should take the same recipe once ported.

## Other rows
- 0x004091C0: new STLport opt-in (compare inline); the row includes stlport/fable_string.h before fable_string.h (fable_string.h is
  `#pragma once` only, so copying it into an overlay double-defines CWideString; the stlport header has a macro guard).
- 0x00416953: one-grade port. The data globals 0x013B8648 / 0x013B8616 take the spellings most landed rows use
  (g_bCompileFrontendDefinitions, 5+ rows; g_bRuntimeLevelFiles, the ISM rows that define the flag's use); neither has a PDB name:
  descriptive. 0x013B8648 is the static-map export switch in this row, so the coordinator may prefer another descriptive name for the
  address everywhere; I kept one existing spelling rather than add a third.
- 0x005955AB (extra, claimed): RM in 2 grades; its binder fixes (ToCharString, CAFile::IsPathFile, CCharString::NullChar) are in RENAMES s2.

## Coordinator items
- recheck_final.log WARN RELOC_CONFLICT (0x00596CCE / 0x00597006): ?ShareData@?$CCountedPointer@VCList@NUISystem@@@@ and
  ?ReleaseData@..CList.. bind 0x0059AA86 / 0x0059A994 here and 0x00429486 / 0x00429288 in landed 0x005729D8 / 0x0041E6D3: two
  retail COMDAT copies of the same template member (one per TU). NF66's drafts bound them the same way. WARN RELOC_NAME: 0x0059A994 is
  catalogued '~CCountedPointer<NUISystem::CList>' while these rows call it as ReleaseData (the header's _RELEASEDATA rule: the 5-byte
  dtor jumps to ReleaseData); not filed as a NAME_FIX (one source only).
- 0x004091C0 WARN STANDIN_LEAF: the catalog still names the row LoadFileList until RENAMES section 1 lands.
- No EH funclet under rebuild/src/compiled includes any changed row (grep of each file name); none has an EH frame.
- 0x005778CA's self is ?CloseInventoryInternalPC@CTCInventoryExperience@@UAEX_N@Z (Ego_r.pdb 0x005ED15B; CTCInventoryExperience.h
  VIRTUALS maps slot 90 to 0x005778CA); its twin 0x0057770A (held by NF70) is CloseInventoryInternal.

## Rule slips
- The brief's "carry each row to a terminal grade (RM or 15 grades)" is not met for the 5 blocked rows: they have 1-2 NF73 grades
  each (the context budget went to the 5 ported rows). Their STATUS notes give the exact remaining errors and the header work each needs.
- One llvm-pdbutil call on Ego_d.pdb (type lookup) was attempted and denied; Ego_d was otherwise read only with `python3 -I` and
  egod_locals.py.
- One overlay copy of the root fable_string.h was made and removed (it double-defined CWideString); no change to the root header is
  proposed.
- Nothing written under /home/user/FableDecomp; no git; no __asm / naked / _emit / #error in rows (grep of rows/).
