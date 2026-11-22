# 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents: AUTH23's three bounded checks (2026-11-22)
**SUPERSEDED HEAD: read the section "FINAL" at the end first.** The row is now final (`0066b370.cpp`, overlay `hdr_f/`; `hdr/` and `hdr_a/` are superseded), 4 grades by AUTH23; the pick-any hold IS lifted there. Sections 0a..0c below are the three bounded checks as first reported (0c's "does not lift the hold" is about the row-tail pragma only).
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

## FINAL (after the owner's decision "land as Inferred", coordinator message 2026-11-22)
**`0066b370.cpp` (= f1) + overlay `hdr_f/` (patch `hdr.diff`): RELOCATION_MATCH at stock flags, test PASS, strict dry-run lander WINS 1.**
- Text: AUTH22's final with Ego_d line 208's two accessor calls restored (no member read, no friend line). The accessors return
  `const float&` only under the new off-by-default opt-in FABLE_RETAIL_CANIMSET_PERCENTAGE_REF_INFERRED (engine/CAnimSet.h, Inferred comment
  modelled on FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY_REF_INFERRED); the by-value default stays under FABLE_RETAIL_CANIMSET_ANIMINDICES. The
  row carries an `// Inferred:` line for it and one for the stop-event arm.
- Pick-any GetSubDef<CEntitySoundDef>:
  1. `extern template` (mini/t4.cpp): VC7.1 3077 warns C4231 and then stops with fatal error C1001 (internal compiler error) at the call
     of the member-template instance. Not usable.
  2. Nested opt-in FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY beside the template in engine/CDefinitionManager.h: an
     `inline` explicit specialization inside `optimize("y", off)` / `optimize("s", on)` .. `optimize("", on)`, body = the template's tokens
     with T = CEntitySoundDef. One grade: leaf RELOCATION_MATCH; pk.py over all 100 defined symbols: GetSubDef<CEntitySoundDef> 162 B,
     EQ 0x0065F604; every other symbol identical in size and class to the a1 object; no new symbol; 0 NEQ. **Pass: the hold is lifted.**
     The opt-in needs engine/CEntitySoundDef.h (it includes it) and a TU with no optimize pragma above the include.
- pk table: `f1.pk.full.txt` (all symbols), owners in `owners.tsv`. Copies with a retail owner, all EQ: operator new 0x00405C10,
  CDefString::GetString 0x00415D70, operator CCharString 0x00415DD0, CVectorMap::LowerBound 0x0040F020, GetDef<CEngineThemeDef> 0x0063F3A0,
  GetDef<CMaterialDef> 0x0066C340, GetSubDef<CEntitySoundDef> 0x0065F604, bitset<274>::test 0x00410DE0, vector<CAnimSet*>::_M_insert_overflow
  0x0066C3F0, CThing::HasTC 0x004118C0, vector<CAnimSet*>::push_back 0x0066C4D0.
- Header proofs: pin_asserts moved nothing in CAnimSet.h / CDefinitionManager.h / CTCSoundPlayer.h / CThingSoundEmitter.h (the last was
  pinned once by AUTH22). `g++ -E -P` of CAnimSet.h and CDefinitionManager.h, overlay v current repo copy: identical md5 on the default
  path and with a retail macro set (GETSUBDEF_BODY + DEF_CLASS_BASE + CDEFINITIONMANAGER); CAnimSet.h differs only when the new
  FABLE_RETAIL_CANIMSET_ANIMINDICES is defined. The new macros have 0 hits in rebuild/src, rebuild/include, rebuild/tests.

### The stop-event forwarder against the standing criterion (ADVISOR_DECISIONS_B 2026-11-21)
Facts: retail's only calls to CBaseIntelligentPointer::GetPItem 0x00A01B50 in this function are 0x0066B755 and 0x0066B764, both inside the
arm, followed by the slot-12 virtual call 0x0066B77A; Ego_d's line table goes 215 (16 B) -> 228 (5 B), so lines 216..227 hold no code;
Ego_d lists GetPItem as a proc.
- **It fits clause (1) as written, through its "or the reverse"**: retail's block calls a named, non-debug, out-of-line callee (GetPItem),
  and the Ego_d block cannot call it or anything else, because it has zero bytes. The 2026-11-21 clarification (run the test by size) is
  met in the strongest form a size test has: an empty line range decodes as no call under any calibration. It is not a "more code" size
  argument, which the clarification excludes.
- Two limits, so the scope is clear. (a) "Ego_d's FUNCTION never calls GetPItem" is true but weak evidence on its own: at /Od every
  intelligent-pointer use goes through an out-of-line wrapper, so no /Od function calls GetPItem directly. The finding rests on the empty
  block, not on the function-wide name test. (b) The criterion only unbinds the block from Ego_d. It attests nothing about the text put
  there: the forwarder CThingSoundEmitter::StopAllCriteriaContainingString stays Inferred, and its name stays DESCRIPTIVE (Ego_d has no
  `StopAll*` name on either class; the slot name is Anniversary's).

### Notes for the lander
- pk.py's control on AUTH22's object first printed NEQ:relocs for bitset<274>::test and CThing::HasTC (the `bitset` literal); the final table
  shows both EQ with no owners line added by me, so reloc_bindings.tsv gained that literal while I worked. Not a change in the objects.
- hdr.diff has one removed line: `FABLE_STATIC_ASSERT(sizeof(CThingSoundEmitter)==0xb8);` becomes `FABLE_STATIC_ASSERT_AT(74, ..)`, AUTH22's
  pin_asserts pin. Bases: orig/ = the repo's current copies of all four changed headers.
- Portability: the new specialization opt-in uses `__declspec(noinline)` and `#pragma optimize` without the `_MSC_VER` guard that the template
  above it and the neighbouring GETDEF_INDEX_BODY block have. It is off by default and token-identical under MSVC, so the grade stands;
  add the guard at landing if wanted (no regrade needed for an #ifdef that is true under VC7.1, but the lander should confirm).
- The lander ran WITHOUT --land (dry run); nothing was written to the repo.
