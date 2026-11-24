# NF332 lessons

- Compile HEAD and new at the same path. The anonymous-namespace `?A0x<hash>` comes from the path, so two paths give a false
  NOT_EQ (nf.py: `wd = W / 'g' / a`).
- nf.py proof pairs renames by sort order, which is wrong when one object has two renames. proof332.py pairs them by the
  `?Name@Scope@@` prefix. A rename that changes the name itself (vfunc_30 -> LeaveGame, a `_004af8f0` suffix, Reset_007a7000 ->
  UpdateDirectionOverride) needs an explicit `addr:old=new` map; otherwise it reports UNPAIRED.
- Self access / const fixes: a test that calls the row from outside the class breaks under `protected:` / `const` even though
  the row objects are ALL_EQ. Grades and objsec do not catch this; only the vland recheck does (LINK_FAIL). Fix it in the test
  with a test-only public accessor and zero line delta. Run the recheck before you call an access group done.
- Caller respellings: a test that stubs the callee under the old spelling (008915e0) must be respelled with the row, or the
  recheck fails to link.
- medit counts every occurrence, comments included. In the Alloc factories, `new CTCX(owner)` also appears in the header
  comment, so target the code line (`    return new CTCX(owner);`).
- The CTCX constructor family (`CThing*` -> `CThing&`, called as `new CTCX(*owner)`) is byte-identical: `push esi` passes the
  same register either way. Both PDBs also name the factory itself (`?Alloc@CTCX@@SIPAVCTCBase@@AAVCThing@@@Z`), which is a
  NAME_FIXES item, not an in-place fix.
- `virtual` on an empty local base (CThing, CTCBase): shrink the derived class's leading pad by 4 so every offset is kept. When
  an empty CTCBase sits under it, give it its real `virtual ~CTCBase();` so the vptr lands at +0 with no this-adjust.
- Re-screen every census binder against the off-limits lists before locking. Several entries come down to a single binder that
  is held or in T3 (39, 54, 56, 58, 139, 179), and the right status for those is skipped-held, with no compile.
- Before handing back, run the repo's `naming_batch/apply_table.py <TABLE_CHANGES.tsv> --dry` (it only reads). It caught
  that respelling a `_<addr>`-suffixed stand-in to the PDB name gave that spelling two targets. The suffix was hiding a
  second, wrong binder (008915b0 called SetBoastAsFailed under the SetBoastAsCompleted spelling). The fix is to respell that
  binder in the same landing, never to keep the suffix.
