# NF304 notes (2026-11-23): T1 signature, NF300's continuation (ENTRIES 1-6 = NF300's re-scope entries 5, 6, 7, 26, 31, 41; 7.. = NF300's 72..)

Base: the repo working tree of today. No repo writes, no git. All 61 orig/ files (+ orig/00a10dc0_test.cpp) equal the repo at hand-back
(cmp). No header changed (hdr/ empty, so no equiv_check was needed).

## Result
- **60 entries looked at (1-60):** 40 done, 1 done-partial (24), 17 skipped-port, 1 skipped-held (9: its fix respells the T3-queued 0x004498C0),
  and 55 left `open` (deferred, not attempted: a six-row re-scope unit), so 59 classified. 61-227 `open`.
- **61 rows changed** (rows.txt == the 61 locks I still hold; every control lock released), zero line delta except one added line:
  `// Leaf: ??0CRenderTarget@@QAE@AAVCSurface@@0@Z` at the top of 0x00A10DC0 (it had no Leaf). Three bare Leafs became full mangled
  (0x00555440, 0x005556F0, 0x00492F69: self renames). One test changed: rows/00a10dc0_test.cpp (its model ctor takes `CSurface&` like the
  renamed self; without it the test LINK_FAILs).
- **5 self renames** (RENAMES section 1): 0x00A10DC0 (const -> non-const refs), 0x00555440 / 0x005556F0 (protected + global ::C2DVector),
  0x00492F69 (const CFontBank&), 0x006CB780 (const member, C3DVector& out). Each moved together with every binder that was RESOLVED
  against its stand-in at HEAD (compiled census), so no reference HEAD resolved is left unresolved.
- **RENAMES section 2:** 61 caller lines. **TABLE_CHANGES:** 46 lines (assemble.py: delete when the PDB line exists, else replace; plus 5
  table-only stale-witness deletes in work/tc_extra.tsv: entries 5, 10 x2, 32, 45). **NAME_FIXES:** 8 lines
  (self stand-ins left in place).
- **Proof per changed row:** HEAD + new VC7.1 objects RELOCATION_MATCH (evidence/grades.log); objsec ALL_EQ for all 61 (.drectve
  included, every emitted table unchanged: evidence/proof_g1, g1r, g2, g2c, g2d, g3, g3b); a `nf.py syms` HEAD v new diff for every row:
  only the respelled method symbols change (UND in binders, DEF only in the five selves) - this is the check `proof` cannot make, and it
  is why entries 1-3 / 8 are held (below).
- **Links** (evidence/link_g1..g3; /FORCE, HEAD set v new set, both with all compiled controls): 0 duplicates either side; no symbol is
  unresolved in the new set that was resolved in the HEAD set; every moved spelling resolves wherever its self defines the PDB spelling.
- **vland --recheck-landed** (recheck_final.log): 61 / 61 RELOCATION_MATCH + test PASS (the log also holds the held 0x00449CB0, PASS) (rc00-rc02, rc03 = 0x00A10DC0 with its updated
  test). RELOC_NAME warnings are pre-existing labels of other callees (none names a spelling I moved); no RELOC_CONFLICT.
- **Table deletes v the full caller list:** for each target touched, the exe census (evidence/census_exe.tsv, census_x1.txt for
  GetPlayer 0x004498C0) was compiled row by row (controls) and read with bind.py (evidence/bind_*.txt). Rows I could not compile (another
  agent's entry or lock, or off limits) were read from source and are named in the STATUS line: 0x0063E480, 0x0074A5D0 (e7), 0x005BCAFE
  (e16), 0x00998570 (e34), 0x006FCC80 (e45), 0x00492ED1 / 0x00492FB4 / 0x0074EE90 / 0x0092FAB0 (e35-37), 0x00449BF0 / 0x00449C10 (GetPlayer,
  NF302's locks). None of them can bind a deleted spelling.
- Name census (evidence/name_census.txt, defscan.txt): no landed row defines any new spelling except its own self; other definitions found
  under the same Class::Method are other overloads (checked by Leaf / signature).

## Decisions / questions
0. **Evidence note:** evidence/proof_g1.txt and link_g1.txt were built with the HELD variants of 0x006B8640 / 0x00449E60 / 0x00449CB0 in the
   new set (all ALL_EQ; the link adds nothing unresolved); they are not in rows.txt. No landed row #includes any changed file by path (grep of
   rebuild/src + rebuild/tests for each basename: none). Two-source check: pdbsets.py, both Ego_r and FableWin name sets give every PDB
   spelling of entries 1-60 (evidence/pdb_namesets_g1.tsv, _g23.tsv).
1. **Q1 (entries 1-3, 8): re-scopes whose only extra effect is renamed pick-any COMDATs.** The re-scoped variants are RELOCATION_MATCH,
   objsec ALL_EQ and bind the PDB setters (held in evidence/held/, work/held/), but the TU's own DEFs change name: 0x006B8640's implicit copy
   ctor ??0CRadialBlurPositionParams (new name = FableWin's) and its three local inline getters on the stand-in CScreenEffectPriorityStack
   (PDB: NScreenEffect::CScreenEffectPriorityStack::GetCurrentRadialBlurParams / ...PosParams / ...ScreenFilterParam); 0x00449E60's
   implicit ??0 / ??1CPlayerSave (new names = FableWin's). Ruling NF300 Q2 says no own definition may change name, so I held them. Land them
   when the renamed COMDATs are implicit members of the re-scoped type itself (now spelled as the PDB), or keep them T3?
2. **Q2 (entries 56 / 57):** the selves define the deed type as `long`; the binders pass a `long Type` member of their local deed model.
   Retyping that member to the shared engine/EOpinionDeedType.h enum (PDB member type) plus the parameter type is beyond ruling NF300 Q1
   (literal -> enumerator). In place, or T3?
3. **Q3 (entry 4):** the CAddPrimitiveDesc re-scope would also respell the stand-in callee CEngine_01436e80::UpdatePrimitive (0x00B32000).
   Its probable identity is ?UpdateRepeatedZSprite@CEnginePrimitiveRenderer@@QAEXABVCEnginePrimitiveHandle@@ABUCAddPrimitiveDesc@NEnginePrimitiveManagerRepeatedZSprites@@@Z
   (both PDB name sets), unproven here. Queue as a callee port?
4. **Q4:** 0x006B8640 also models CDisplacementParams globally; FableWin nests it (??0CDisplacementParams@CEnvironmentSpellEffectInterface@@),
   but SetDisplacementParams (0x00884F90, not in ENTRIES) is in neither name set. Left alone.
5. **Q5 (entry 9, held):** the binder 0x00449CB0's fix (struct -> class CPlayer) necessarily respells its GetPlayer binding (0x004498C0),
   a T3_PORT_QUEUE item (nf301). Measured: `CPlayer& GetPlayer(long)` gives the PDB QAEAAVCPlayer@@J@Z and objsec ALL_EQ; its PAU line has
   no other binder, and the PAV line's witness 0x0066FB80 is stale (rewitness to 0x00449D90). May it land with that queue item, or wait?
6. **Q6 (self renames):** five selves were respelled in place (0x00A10DC0, 0x00555440, 0x005556F0, 0x00492F69, 0x006CB780; for the two
   CScrollableList ones the self's own definition changes name through the C2DVector re-scope). I read NF282 D2 ("binder / owner self names
   are T3") as covering stand-in owner NAMES, not a PDB-signature fix of a self whose body stays byte-equal (NF300's 0x00AE3A30 landed so),
   but the letter of NF300 Q2 is the reason I held entries 1-3 / 8. These rows are ready pending that ruling; the selves 0x00A64E70 /
   0x00AEE940 / 0x004B3A20 / 0x009FC5C0 / 0x009BA110 / 0x009F35D0 / 0x009F36E0 were left as NAME_FIXES (their fixes need body or model ports).
7. **Debt noted (not mine):** census row 0x00A76A80 DEFINES CSmallVector<const CANavQuadTreeNode*, 8>::resize under the PDB spelling
   (out-of-line template member, pick-any) - the binder 0x00A772D0 I moved (e31) can resolve to that copy in a full link. Entry 24:
   the ??_G row 0x008270C0 calls a free stand-in ?CScriptedMapBrush_dtor@@ (NAME_FIXES).

## Method
- Groups: g1 = entries 1-20, g2 = 21-34 + 39-44 (+ the entry 27/28 self unit and entry 24), g3 = 35-38 + 45-60. Per group: changed list +
  controls (every self + the full exe census of every touched entry), pre_tests.sh on every landed test BEFORE the locks, locks, medit.py
  specs (evidence/spec_*.json), cpp_err.sh + clang prechecks (NOWCHAR reruns where L"" / wchar_t failed), D1 cl /Zs for two tests, grades in
  4 background lanes, bind / proof / syms diff / link, then STATUS lines.
- Edit classes: pointer -> reference (`*p` / dropped `&` at the call), `const` member, access labels on the declaration's own line
  (`private: ... public:`), return type (bool), `unsigned long` <-> `long`, `bool` for a literal-1 `unsigned long`, dropped `__fastcall`
  on a member with a by-value class argument, struct -> class key, a forward declaration moved out of a namespace, C2DVector moved out of
  NUISystem (six TUs), the typedef of a callback's parameter, `virtual` on a non-polymorphic local dtor with the leading pad shrunk
  (NF299 Q3: no ctor / dtor defined inline in the TU, so no ??_7 / ??_R).

## Rule slips
- Once I ran `rm -f g1xk_*` (a glob in my own work/ folder, before a `split`; it matched nothing and removed nothing). Every other removal
  used clean.py.
- Two test prechecks were re-run after the lock (T_008139c0 with NOWCHAR, a pre-lock clang attempt existed; NT_00a10dc0 is my new test).
- Clang prechecks ran in the foreground under `timeout 120`; every VC7.1 grade, cl /Zs, link and vland ran in the background with
  `timeout 900` on its own launch line, checked with kill -0 in later calls. No sleeps.
- Reading list: my prefix, the standing brief, GOTCHAS (all), NF300's hand-back and landing README, CLAUDE.md, EXTRA_RULES (all),
  COORDINATION.md. Not read: ASM_RULES.md (no asm question arose) and the older landing READMEs nf281-285 / nf296's hand-back.
- No retail or donor bytes in hand-back files (the vland oracle files stay in work/).
