# Re-port amendments: 0x007332F0 CCombatAbility::IsValid, 0x00733620 CTCCombat::GetCombatAbilityData (p25N, 2026-11-20)

Landing order 2 of 3. These replace the landed files' local models with the shared-header text that 0x00733CF0 copies verbatim.

| row | landed file | grade now | diff |
|---|---|---|---|
| 0x007332F0 | rebuild/src/compiled/00/73/CCombatAbility_IsValid_007332f0.cpp | RELOCATION_MATCH (71) | `src_007332f0.diff` |
| 0x00733620 | rebuild/src/compiled/00/73/CTCCombat_GetCombatAbilityData_00733620.cpp | MATCH (106) | `src_00733620.diff` |

- **Strict vland:** passes (`vland.log`, run with `--recheck-landed` because both rows are landed): LANDVERIFY_STRICT_HEADERS=1,
  FABLE_HDR_OVERLAY=`hdr/` (the same overlay as batches 1 and 3), fresh LANDVERIFY_WORK, behaviour PASS.
- **Line 2 / Retail / Identity lines:** kept. The comments are rewritten where the text changed.
  - 0x00733620's Retail line now says 106 bytes; the old line said 105, but the body runs to the `ret 8` ending at 0x00733689.
  - 0x00733620's comment drops the Plan-phase-5 CCountedPointer-wave line, because the local models it described are gone.

## 0x007332F0 IsValid
- **Text:** Ego_d (tc_combat.cpp, file id 3264) lines 166 / 168 / 170 / 172 / 174 are 15 / 4 / 29 / 4 / 2 bytes at /Od, and this text
  gives the same.
  - The condition is one 29-byte statement at line 170. So it is `RepeatTimer == -1 && GFRandom(100, prandom_seed) < ..`: an IsUsable()
    call alone is 15 bytes, and the landed hand-stepped seed is four statements.
  - GFRandom is engine/GFRandom.h (Ego_r.pdb ?GFRandom@@YIKKPAK@Z).
  - Parameter names are Ego_d's: combat_ability and prandom_seed.
- **IsActive:** the file carries `// Fidelity: same-TU strong definition of 0x00733090 (retail TU-mate)`, the batch-1 row verbatim, so
  IsActive is expanded as retail does. With IsActive declaration-only: DIFFER(74v71) (`evidence/isvalid_no_isactive.cpp`).
- **Tests:** the landed test (`rebuild/tests/00/73/CCombatAbility_IsValid_007332f0_test.cpp`, a self-contained model) still passes. The
  new `test/007332f0.cpp` links the REAL object: 480 cases (4 repeat x 4 active x 6 chances x 5 seeds), covering the seed write-back,
  GFROR13 called only when stepping, and the unsigned compare.
- **Mutants:** 8/8 KILLED.

## 0x00733620 GetCombatAbilityData
- **Body:** the landed loop over CTCCombat.h's types. The InterfaceType read goes through CCombatAbilityData::GetInterfaceType(), the
  shared header's accessor for its private member.
- **Names:** Ego_d's parameter names, ittybitty and interface_type.
- **Symbol fix (NAME_FIXES):**
  - Old: `?GetCombatAbilityData@CTCCombat@@QAE_NAAU_Rb_tree_iterator@@J@Z` (public, long).
  - New: `?GetCombatAbilityData@CTCCombat@@AAE_NAAU?$_Rb_tree_iterator@..@std@@W4ECombatAbilityInterfaceType@@@Z`, private with the enum
    argument, as both PDBs spell it (Dinkumware _Tree_iterator there, STLport in retail).
  - The leaf prefix `?GetCombatAbilityData@CTCCombat` is unchanged.
- **Caller still on the old spelling:** landed 0x00733750 (the const overload) calls 0x00733620 through its own local
  CCombatAbilityIterator model. That is re-port debt, not touched here.
- **Tests:** the landed test (a self-contained model) still passes. The new `test/00733620.cpp` links the REAL object over a five-node
  STLport tree: leftmost, root, a successor through the parent, rightmost, two matches (the first wins), none, and an empty map.
- **Mutants:** 5/5 KILLED.

## Define block
The tc_combat.cpp rows' common block, as in batches 1 and 3.
