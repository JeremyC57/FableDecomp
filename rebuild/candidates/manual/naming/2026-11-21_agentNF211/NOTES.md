# NF211 notes (round 10, 2026-11-21; single agent; both entries brought to a terminal state, in the brief's order)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log):
- 2 entries: q11772 done; q4389 blocked (partial: the in-scope rows 0x006EAC50 + 0x006ED510 are done; 0x006E93C0 / 0x006EA770 are left and
  out of naming slices per FINDINGS_LOG 2026-11-20).
- Rows: 3 ported (0x0047DB73, 0x006EAC50, 0x006ED510), all RELOCATION_MATCH + PASS in recheck_final.log; 6 not-a-binder (0x00410DE0,
  0x006F0250, 0x00662430, 0x006F3000, 0x006F0810, 0x006F2FD0); 0 blocked.
- RENAMES: section 1 has 1 self rename (0x006EAC50, leaf-only); section 2 has 5 caller respellings.
- Header opt-ins (hdr/README.md): stlport/fable_containers.h FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY (new), engine/CThingCreatureBase.h
  FABLE_RETAIL_CTHINGCREATUREBASE_ISINMELEEASATTACKED (NF210's held opt-in, verbatim). pin_asserts pinned 0 on both.
- selfcheck.log: 0 problems.

## q11772 (0x00410DE0 std::bitset<274>::test; last wrong binder 0x0047DB73)
- NF209's held RELOCATION_MATCH draft, unchanged except one NF211 comment, regraded on today's headers: RELOCATION_MATCH PASS. Its test is
  NF209's. 0x00410DE0 +0x33 moves from `?Do@D@@QAEXH@Z` onto `?test@?$bitset@$0BBC@@std@@QBE_NI@Z`. `?Do@D@@QAEXH@Z` had 0x0047DB73 as its only
  table witness and no landed source declares it now.
- Defined-symbol diff, HEAD object v new: only `+?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z`, the 8-byte copy-against-copy debt the nf210
  ruling accepted (NAME_FIXES DEBT line for 0x004118C0). The declaration-only HasTC draft is not used.

## q4389, row 0x006EAC50 (held at nf210) and the 0x006ED510 binder
- (a) FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY fixed it in the first form.
  - The vector<C3DVector> copy ctor is now UNDEF in the object. Its COMDAT and 8 helper COMDATs are gone.
  - The leaf's bytes and bindings are identical to NF210's draft (cmpbody 451/451 True/True).
  - The CCombatDebugInfo copy-ctor copy is byte-equal (79 = 79) to the owner 0x006F3000's object. Its 0x006F0810 calls bind the external
    STLport spelling; the owner's bind its external `??0?$CRetailVector@VC3DVector@@@@QAE@ABU0@@Z`. Both are bound at 0x006F0810 in
    reloc_bindings, so it is the same target (nf205 amendment). Its CCharString copy ctor is the same symbol on both sides.
- (b) The default-ctor owner is landed 0x006F2BD0. Its catalog name is the descriptive stand-in `SNamedZeroRecord::SNamedZeroRecord`.
  - A masked-byte search of Fable.exe for the COMDAT hits only 0x006F2BD0, and FableWin.pdb has `??0CCombatDebugInfo@@QAE@XZ` at 0x0241C530.
  - q.py of the row source at 0x006F2BD0: RELOCATION_MATCH. Against the owner's object: 46 = 46 bytes equal, and the same target
    `??0CCharString@@QAE@XZ`.
  - NAME_FIXES has the rename. It is not applied: the owner is outside my locks, and only one PDB plus the body support it.
- ~CCombatDebugInfo v 0x006F2FD0: cmpbody 44/44 True/True; both copies are COMDAT (LINK_ONCE_DISCARD).
- Other newly defined symbols that the table maps to another address all equal their owners (work/owner_q.log): GetCombatantInfo
  0x006E9360 (cmpbody True/True), less<u64> 0x00526120, _M_find 0x006F0250, ~vector<C3DVector> 0x004DCFBB, CCharString::GetAsCharArray
  0x00403A10 (NullChar v the owner's stand-in for 0x0129AAF4, nf205) and GetLength 0x00403A00.
- (c) 0x006ED510 is respelled in place inside its existing local CThingCreatureBase model: `bool IsCombatant();` ->
  `bool IsInMeleeAsAttacked() const;`.
  - binds: only 0x00662430 changes, onto the self row's Leaf (Ego_r 0x0053D2C0 + FableWin 0x01DA3370).
  - Defined-symbol diff: none.
- In-place respell (NF210): CCombatDebugInfo and CCombatSequenceBase moved out of namespace CombatDebug. They are the same class tokens as
  HEAD; neither has a shared header (CCombatSequenceBase is only in engine/_quarantine). The object defines the three CCombatDebugInfo
  implicit / inline members. Each is a COMDAT equal to its retail owner's copy (above); that is the pick-any case, not a one-definition clash.

- Owners with no table line: a `// Leaf:` scan of rebuild/src/compiled for each of the 41 newly defined symbols (work/newsyms.txt) hits
  only owners already checked: _M_find 0x006F0250, ~CCombatDebugInfo 0x006F2FD0, GetCombatantInfo 0x006E9360. The two stand-in owners
  0x006F2BD0 / 0x006F3000 were found by exe byte search instead.
- LNK2005 (accepted debt, nf181 / nf184): owner 0x006E9360 defines GetCombatantInfo STRONGLY (no LINK_ONCE). This row's copy is a COMDAT
  (engine/CCombatManager.h GETCOMBATANTINFO_INLINE) and is identical to it: cmpbody 41/41 True/True; q.py at 0x006E9360 RELOCATION_MATCH.
  HEAD precedent: landed 0x006E94D0 defines the same opt-in and already emits the identical COMDAT (q.py HEAD compile, cmpbody 41/41
  True/True), so this LNK2005 symbol exists at HEAD; this row adds a second identical copy. Queue: make 0x006E9360 pick-any. Other
  same-name copies: ~CCombatDebugInfo is COMDAT on both sides; _M_find / less<u64> are covered by the nf210 landing's 0-LNK2005 link.
- Row comment: NF210's callee line listed the CCombatDebugInfo copy ctor / dtor as callees. binds shows both are expanded, not called,
  so the line is corrected (comment only; recheck_final.log was re-run after it).

## Splits (none created, none grown)
- 0x006EAC50: 0 referrers (refs.py exe scan). The old self leaf is bound only by this row.
- 0x006F0250, 0x006E99B0: callers move onto the self rows' Leaves (narrows). The CombatDebug stand-ins had 0x006EAC50 as their only witness.
- 0x006F0810: moves onto the STLport spelling already bound there (witness 0x00765B70). 0x006F3000 still binds its CRetailVector stand-in:
  an existing split, unchanged.
- 0x0129AAF4: a new `?NullChar@CCharString@@1DB` binding at +0x103, already bound there (witness 0x00AE8D50).
- 0x00662430: NOT closed. The exe scan finds 8 referrers. 0x006ED9E0 (out of scope, lock `released`) still declares
  `bool IsCombatant(); // 0x00662430` and binds the stand-in; reloc_bindings shows only 0x006ED510 as the witness. The address stays at
  2 spellings (IsInMeleeAsAttacked, IsCombatant). NAME_FIXES has the respell for 0x006ED9E0.
- 0x00410DE0: `?Do@D@@` leaves. The self Leaf is the prefix `?test@?$bitset@$0BBC@`.

## Coordinator items
1. Queue 0x006ED9E0's in-place IsCombatant -> IsInMeleeAsAttacked respell, the same one-line change as 0x006ED510. It closes 0x00662430
   2 -> 1.
2. 0x006F2BD0 `SNamedZeroRecord` is retail `??0CCombatDebugInfo@@QAE@XZ` (FableWin + body). Queue its self rename with 0x006F3000's
   (land1120nf210), so both CCombatDebugInfo constructor rows carry the PDB names.
3. Debt: the 0x0047DB73 HasTC COMDAT (NAME_FIXES DEBT, nf210 ruling).
4. Keep reloc_bindings line 39363 (`?IsCombatant@CThingCreatureBase@@QAE_NXZ` 0x00662430): when 0x006ED510 moves, its witness must become
   0x006ED9E0, not be dropped.
5. Debt: the 0x006EAC50 / 0x006ED510 tests remain standalone host models (NF210 precedent). GetCombatantInfo LNK2005 v strong 0x006E9360
   (above).

## Local models, tests, flags, funclets
- No class token added to any row (selfcheck 0). 0x006ED510's vland SHARED_HEADER_AVAILABLE warnings are HEAD's local models.
- Tests: 0x0047DB73 uses NF209's test (std::bitset<274>::test double). The 0x006EAC50 and 0x006ED510 tests are unchanged: they are
  standalone host models that do not link the row (NF210 precedent) and use no renamed symbol. Both PASS.
- No `// Flags:` lines added. No Inferred, Bracket or Include-order lines added; 0x0047DB73 keeps NF209's two Inferred lines.
- EH funclets: no landed row or funclet includes any of the three changed files (grep of their file names under rebuild/src and
  rebuild/tests). 0x006F2300 names 0x006ED510's file only in a comment. 0x006EAC50's leaf-only self rename keeps its catalog name, so
  its file name does not move.

## Descriptive names
- None new. Every spelling is a PDB name, a self row's Leaf, or the repo STLport spelling (descriptive-by-library: the vector<C3DVector>
  copy ctor, _M_find).

## Locks
- Rows touched: 0x0047DB73, 0x006EAC50, 0x006ED510. All three are in rows.txt (locked NF211). No new lock was claimed.

## Rule slips
- None found. The pre.sh helper copied from an earlier agent pointed at agentNF130's overlay. I repointed it to agentNF211 before its
  first use.
- 0x006EAC50 took 13 compiles: 1 form, plus 9 owner-address grades of its source and the HEAD control. The round-10 budget counts forms.
