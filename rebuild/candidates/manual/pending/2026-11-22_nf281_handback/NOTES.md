# NF281 notes (2026-11-22, round 35): tracker batch T1-access + T2, ENTRIES order, first hand-back

Base: the repo working tree as of this session (never written; no git). NF279 / NF280 ran beside me: none of their entries, none of the
Codex pass 28 RESERVED rows, none of 0x00B4D370 / 0x00BD3070 / 0x00890250 / 0x0091C080 touched (work/offlimits.txt intersected with
every row I compiled: empty). Every row I compiled was claimed with tools/lock.sh first (work/locks.log: all `claimed-new` or
`claimed-released`); the two I claimed and then did not compile (0x009624D0, 0x00961C50) were written back to `released`.

Reading list read in full: BRIEF.md, agentNF278/BRIEF.md, NF214_BRIEF.md, docs/pipeline/ASM_RULES.md, EXTRA_RULES.md, docs/COORDINATION.md,
docs/pipeline/GOTCHAS.md (all 504 lines, in chunks), the nf266 and nf268 landing READMEs, the naming tracker SUMMARY.md and OPEN.tsv header.

## Method (every changed row)
1. **Census, before any edit.** (a) Exe scan of every E8/E9 / absolute reference to each entry address, mapped to landed rows
   (tools/census.py -> work/census0.txt). (b) A clang MS-ABI `-O0` object of every referencing row AND of every self row
   (tools/csym.sh -> work/cs/*.syms; 854 + 234 rows, a census only, not a grade, run without locks; 82 rows fail clang and fall back to a
   source grep: work/crep.txt). (c) Both PDB name sets for every entry (tools/pdbchk.py -> work/pdbchk.txt: every pdb_spelling is in
   Ego_r AND FableWin). The clang census found what the tracker could not: several T1 entries' self rows define a wrong spelling too
   (T1 entries 1, 2, 7, 18, 32, 34, 47, 48, 50; see STATUS), and a third spelling (0x00A254A0 RemoveLOD QAE) that the table never listed.
   Every wrong binder the clang census named was then confirmed from its VC7.1 HEAD object (the HEAD half of each grade).
2. **Edit in place, zero line delta** (tools/fix.py): `private:` / `protected:` written on the declaration's own line and the old access
   restored after it on the same line (`private: bool F(); public:`), `virtual` / `const` added on the same line; out-of-class definition
   gets `const` where the decl did; T2 rows also get the full mangled `// Leaf:`. tools/ctx.py checks the access in effect before each line
   (always public / struct default, as the wrong letters imply).
3. **Precheck** (tools/prep.sh, pret.sh: clang, in an earlier call, after the lock) of the new source, the HEAD source and the landed test.
   Six landed tests include VC7.1 `<cwchar>` / iostream, which clang cannot parse with the VC7.1 headers: those were prechecked with
   g++ and the host headers (tools/preth.sh, `TEST-HOSTHDR` lines in work/pre.log). DECISION D3 asks whether that is acceptable.
4. **Grade + proof** (tools/cls.sh: gr.sh = HEAD and new VC7.1 objects, every sc.sh under its own `timeout 900`; hk.sh = HEAD controls;
   proof.sh; lk.sh). proof.sh = objsec of HEAD v new object with the symbol renames read off the DEF/UND diff: every section byte- and
   relocation-identical after the rename (ALL_EQ) for every row; evidence/proof_<tag>.txt. Rows that emit `??_7` / `??_G` emit the same
   tables at HEAD and new (vtbl counts equal, objsec equal): vtmap of 0x009183A0 / 0x009490D0 / 0x00977C40 (evidence/vtmap_*): only
   intelligent-pointer tables, unchanged.
5. **Link** (lk.sh / lnk2.py, LINK unset, background): HEAD set (HEAD binders + HEAD definer controls) v new set (new rows + the same
   controls). In every batch the HEAD set leaves the wrong spellings unresolved and the new set resolves them, 0 duplicates
   (evidence/link_c1 .. c7.txt; the leftover UNRESOLVED lines in the new sets are other classes' symbols the watch regex also matches).
6. **Recheck** (rf2.sh: vland --recheck-landed with rows/<addr>.cpp and the landed test, `timeout 900` on the launch line):
   work/rf_b*.log, then recheck_final.log over every changed row.

## Decisions I need
- **D1 (friend for a cross-class private call):** 0x00A254A0 (RemoveLOD, called from its `C3DMeshLODInfo_Methods` stand-in) and
  0x00A25E70 (BuildMesh, called from C3DMeshInfo::GetMesh) needed a same-line `friend` in the local model to compile with the PDB's
  private access. 0x00A25E70's `friend class C3DMeshInfo;` is retail-faithful (the PDB access plus retail's cross-class call imply it; precedent GOTCHAS
  NF29: a `friend class` in a local model to mangle a protected callee reached from another class);
  0x00A254A0's `friend struct C3DMeshLODInfo_Methods;` befriends a stand-in (scaffolding, marked so). Both objects ALL_EQ. Land both, or
  hold 0x00A254A0 until it is ported onto engine/C3DMeshLODInfo.h? If held, entry 1 ends 4 -> 2 (ABE + 0x00A254A0's QAE) instead of 4 -> 1.
- **D2 (0x0094FBB0 IsTargetOnATower):** rejected: 0x00951FB0 holds a strong same-TU copy of the AAE symbol (its shared header declares it
  private); the self respell would give LNK2005. Needs a ruling (make the copy pick-any or declaration-only) before the self moves.
- **D3 (precheck tooling):** (a) host-header g++ precheck for landed tests that include VC7.1 `<cwchar>` / iostream; (b) whether a clang
  precheck may add `-Wno-address-of-temporary` (an MSVC extension VC7.1 accepts): entry 23's only binder 0x009624D0 fails clang only on that.

## Rule slips
- The first version of tools/csym.sh ran `rm -f $a.o` (a variable path, inside my work/cs) for three rows before I rewrote it to overwrite
  one fixed file; no other deletion outside tools/clean.py.
- The clang census ran without locks (it is not a compile of a grade; no VC7.1, no lane work dir).
- Six landed tests compiled in the rechecks had only the g++ host-header precheck (no passing clang precheck: VC7.1 <cwchar> / iostream):
  0x0096EE90, 0x00977C40, 0x00998010, 0x00858130, 0x00858590, 0x008587B0 (D3).
- Several Bash calls exited non-zero (a `kill -0` status test as the last command, two fix.py candidate listings); no command was
  repeated more than twice with the same arguments.
- Rows were copied to rows/ (cp2rows.sh) a few seconds before their locks were claimed in two batches; nothing was compiled before the claim.
- Control rows compiled HEAD-only for the link checks (38) were written back to `released` at the end (work/locks.log); rows.txt now lists
  only the 55 changed rows, all still NF281.
</content>
</invoke>

## Result of this hand-back (entries 1-53 reached, ENTRIES order; 54-234 open, not started)
- **41 done** (33 T1, 8 T2), **1 held (53, D4)**, **2 rejected** (2 BeginLoadingStats: needs `mutable` + ctor respell; 24 IsTargetOnATower: LNK2005 with
  0x00951FB0's same-TU copy), **1 skipped** (23: binder fails the clang precheck), **8 census-only, open** (39, 41, 42, 43, 45, 46, 47, 50:
  what the census found is in STATUS for the next agent). 55 rows changed (rows/, orig/ = HEAD copies, all 55 still equal to the current
  HEAD file at its current path, all zero line delta). 16 self-leaf renames (RENAMES section 1, each defined by exactly one landed source:
  evidence/name_census.txt; 13 rows carry the full new Leaf, 0x00856B20 keeps HEAD's plain `UpdateFacingDirection` Leaf and 0x00930EE0 /
  0x00717450 / 0x0052C7E0 have no Leaf line at HEAD (not added: zero line delta)), 49 caller respellings (section 2). No RELOC_NAME line in
  recheck_final.log names any new spelling. No changed row is #included by another row or test (basename grep of rebuild/src, tests, include). recheck_final.log: 55 / 55 RELOCATION_MATCH PASS, 1 RELOC_CONFLICT warn (D4). vtmap HEAD v new of every table-emitting changed row: identical (evidence/vtmap_*), only intelligent-pointer / event tables.
- **Side-effect self renames (entries 38 / 53):** adding `virtual` to the base Update in the polymorphic local models of 0x00717450 and
  0x0052C7E0 makes each row's own override virtual, so the row's leaf moves QAE -> UAE (both PDBs spell the overrides UAE). 0x00717450 has
  no other binder; 0x0052C7E0's UAE is what ~35 landed table emitters already bind (clang census), so both counts fall.
- **D4 (entry 53):** the 0x0052C7E0 recheck WARNs RELOC_CONFLICT `?Update@CComponent@NUISystem@@UAEXM@Z` -> 0x00531EC0 (here, right) v
  0x0052C7E0 (0x005367F3 CList::Update's local model spells its direct call to 0x0052C7E0 as the base). No address gains a spelling;
  the symbol gains a second (wrong, pre-existing) target. Land 0x0052C7E0 and queue 0x005367F3 (NAME_FIXES), or hold entry 53?
- **TABLE_CHANGES.tsv** (generated by tools/tabrec.py from RENAMES section 2 + reloc_bindings.tsv): 34 lines, replace / delete per
  witness that moved. No header touched: hdr/ is empty, no equiv_check needed. No flags changed.
- Locks: every row in rows.txt is mine (NF281) and stays claimed for landing; 0x009624D0 / 0x00961C50 written back to `released`.
