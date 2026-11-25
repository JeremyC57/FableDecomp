# NF446 hand-back (2026-11-25): queued single-address name-fix units

NF446's method and every gate are in `NOTES.md`. Each unit renames a self row together with every landed caller of it.

## Landed
| Unit | Rows | Change | Evidence |
|---|---|---|---|
| u1 | 0x00793CB0 + caller 0x00868B40 | `?GetDamageMultiplier@CTCUnholyPower@@QBEMXZ` (the catalog said CTCDivineWrath) | both PDBs, donor order, interface 0x86 TCI_UNHOLY_POWER, field reads |
| u2 | 0x0074AC80 + NF435's held 0x0089EC60 | `?AddCreatureTriggerer@CTCCreatureGenerator@@QAEXAAVCThingCreatureBase@@@Z` (the catalog said CTCShop::AddShopper; the real AddShopper is 0x00773DC0) | both PDBs, FableWin /Od caller, code placement, +0x1C triggerer vector |
| u3 | 0x00A36FE0 + NF435's draft 0x006E72A0 | `?Validate@CClassFactory@@QAEXXZ`, on an add-only CClassFactory.h opt-in | both PDBs, CLASSLIST body |

- The lander released T3 lines nf291 0x0074AC80 and nf324 0x0089EC60.
- The pair line 0x0074A1E0 was already consistent at HEAD.
- **Tracker:** +5 proven, none worse.

## Held: u4, 0x006D6570 `?GetExperiencePickUpMultiplier@CTCHeroExperience@@QBEJXZ`, with binders 0x00899C90 and 0x008E7040
- **Gates:** every gate passes.
- **q-NF446-1 (coordinator): granted.** The test's bodiless declaration is the same r3 debt as its row.
- **Why the lander still holds it:**
  - 0x00899C90 is named inside the flatten-local2 queue line, which belongs to this very unit and is keyed on 0x006D6570.
  - 0x008E7040 is named inside the unrelated nf293 line (keyed on 0x006E9360).
  - The lander releases only lines keyed on the row's own address.
  - Getting past the hold needs an owner decision on how those queue lines are edited (see HANDOFF).
- **The u4 self fixes a wrong constant.** HEAD's local model of the double at 0x01237F00 added 0.5; retail's is 1.0. It still graded RELOCATION_MATCH, because the grade does not compare constant values (GOTCHAS).
- **equiv_check (u3 header):** 57,160 rows checked; only u3's own rows 0x00A36FE0 and 0x006E72A0 differ.

## u4 landed as u4a + u4b (advisor route, no queue edit)
- **u4a:** the self 0x006D6570 alone, as a pick-any re-row on `engine/CTCHeroExperience.h` with retail's 1.0 constant. Landed with `REL=nf303,flatten1,flatten-local2`, which closed those three lines.
- **u4b:** 0x00899C90 alone, with its test respelt (q-NF446-1). Landed with `REL=nf324`, which closed that line.
- **Still held:** 0x008E7040, behind the nf293 reservation. Its row and its `?GetComboMultiplier@CTCHeroStats@@QBEJXZ` delete stay in NF446's `batches/u4`. 0x006D6570 keeps that one stray spelling until then.
