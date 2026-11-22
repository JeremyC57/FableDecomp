# 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents: AUTH23's three bounded checks (2026-11-22)
AUTH22's row (`$S/w22/auth22/0066b370/0066b370.cpp`, RELOCATION_MATCH, stock flags) is unchanged; nothing was re-authored. Grades: 3.
Control: AUTH22's final source + overlay regrades RELOCATION_MATCH here (c0).

## 0a. `const float&` accessor form (ORACLE, one grade): RELOCATION_MATCH
`a1_0066b370.cpp` + overlay `hdr_a/` = AUTH22's final with (1) CAnimSet::GetPercentageComplete / GetLastPercentageComplete returning
`const float&`, (2) the `friend class CTCSoundPlayer;` line deleted, (3) source line 158 calling
`panim_set->GetLastPercentageComplete(), panim_set->GetPercentageComplete()` exactly as Ego_d line 208 does (no member read anywhere).
So the whole residue of the Ego_d-exact text (AUTH22's r12, DIFFER(1844v1808)) is the by-value return of the two inline accessors: the same
finding as 0x0093D270 (CCombatManager::GetProximityForNavigation). It is an oracle, not a landable text: all three PDBs give the by-value
signature (0b). Object: `a1.obj`.

## 0b. PDB evidence for the member reads and the friend line
| PDB | CurrentPercentageComplete / LastPercentageComplete | GetPercentageComplete / GetLastPercentageComplete | friend records |
|---|---|---|---|
| Ego_d.pdb (TPI 0x210BC, class 0x210BD) | `private`, offsets 44 / 48 | `public`, type 0x210A2 = `float (CAnimSet::*)() const`, by value | 0 LF_FRIEND* in 22,635 field lists |
| Ego_r.pdb (TPI 0x1F4AB, class 0x1F4AC) | `private`, offsets 36 / 40 | `public`, type 0x1F491 (one shared type for both) | 0 in 23,075 field lists |
| FableWin.pdb (TPI 0x210BC, class 0x210BD) | `private`, offsets 44 / 48 | `public`, type 0x210A2 | 0 in 23,369 field lists |
- The two members are private in every PDB, so AUTH22's direct reads need either a friend or public members; public members are contradicted
  by all three PDBs (as far as these later revisions speak for the TLC header).
- `friend class CTCSoundPlayer;` is **not attestable, i.e. invented**: none of the three PDBs holds a single LF_FRIENDCLS / LF_FRIENDFCN record
  for any class (cl does not emit them), so the PDBs can neither confirm nor refute a friend. Same result AUTH2 had for CCombatManager.
- Method: `llvm-pdbutil dump -types` (Ego_d: `$S/dl_debug/types.txt`; Ego_r / FableWin: the repacked `$S/codex27/pdbx/*.4k.pdb`, streamed, nothing
  written), the full (non-forward) CAnimSet field list, and a count of `LF_FRIEND` over the whole stream as the positive control.
- Net: both spellings that match retail depart from the PDBs. Member reads + friend (AUTH22's text) contradicts no PDB record but needs an
  unattested access grant and departs from Ego_d line 208; `const float&` (0a) keeps Ego_d's line 208 but contradicts the by-value type in
  all three PDBs. The Ego_d-exact, PDB-exact text is r12, DIFFER(1844v1808).

## 0c. Pick-any copy GetSubDef<CEntitySoundDef> (owner 0x0065F604), one grade with a row-tail pragma: does NOT lift the hold as specified
`c1_0066b370.cpp` = AUTH22's final + `#pragma optimize("y", off)` / `#pragma optimize("s", on)` after the function (the owner row's own two
pragmas, `rebuild/src/compiled/00/65/CDefClassBase_GetSubDefCEntitySoundDef_0065f604.cpp` lines 19-20).
- Leaf: RELOCATION_MATCH (unchanged).
- GetSubDef<CEntitySoundDef>: 162 B, bytes equal to 0x0065F604 (pk.py prints NEQ:relocs only for the class-name literal symbol, unbound in
  reloc_bindings; retail 0x01235DD0 is "CEntitySoundDef").
- But the tail pragma changes every template instance emitted at the end of the TU: GetDef<CMaterialDef> 0x0066C340 and
  GetDef<CEngineThemeDef> 0x0063F3A0 108 -> 98 B, vector<CAnimSet*>::push_back 0x0066C4D0 50 -> 49, _M_insert_overflow 0x0066C3F0 216 -> 140,
  CVectorMap::LowerBound 0x0040F020 60 -> 64, bitset<274>::test 63 -> 34. All were EQ in the control (`ctl.pk.full.txt` v `c1.pk.full.txt`).
  Four new differing copies of landed owners is worse than the one held copy, so this device is rejected.
- Why (mini TUs, not grades): VC7.1 takes a template instance's optimize mode at the end of the TU, not at the template's definition
  (`mini/t1.cpp`) and not at an explicit instantiation (`mini/t2.cpp`). So the header-scoped size-pragma opt-in of EXTRA_RULES 2026-11-20
  cannot work for this copy as a template either.
- The one device that does isolate it (`mini/t3.cpp`): an `inline` explicit SPECIALIZATION inside the pragma pair gets the owner's mode and
  leaves sibling instances alone. On the row that would be a header opt-in (e.g. in engine/CEntitySoundDef.h:
  `optimize("y", off)` / `("s", on)`, `template <> inline bool CDefClassBase::GetSubDef<CEntitySoundDef>(...) const { the owner row's body }`,
  `optimize("", on)`; the owner row 0x0065F604 already spells the function as `template <>` with these two pragmas). It needs a second grade
  (is the leaf still RELOCATION_MATCH with the specialization inlined, and is the copy EQ?), which is outside this bounded check: named here
  for the coordinator. Not run.

## Files
`c0_0066b370.cpp` (control), `a1_0066b370.cpp` + `hdr_a/` + `a1.obj` (0a), `c1_0066b370.cpp` + `c1.pk.full.txt` / `ctl.pk.full.txt` (0c),
`owners.tsv`, `mini/t1..t3.cpp`, `LOG.md`.
