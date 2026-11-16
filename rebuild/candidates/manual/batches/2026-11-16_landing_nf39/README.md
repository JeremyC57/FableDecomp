# 2026-11-16 landing nf39: name-fix round 4, NF39 (66 landed rows, 66 renamed)

Owner: "Continue the naming fix queue with two agents until complete". Round 4 continues NF37's half: tags land1011fj ..
land1113fz, 290 entries. Brief and records: `naming/2026-11-16_agentNF39/`.

## Queue (290 entries, counted from APPLIED.tsv)

| Status | Entries |
|---|---|
| done | 64 |
| done-already | 2 |
| partial | 6 |
| blocked | 15 |
| carry | 203 |

- **partial:**
  - e73 / e74: binders 0x00744730, 0x00744DB0 and 0x00745730 are local CPlayerGui / GObj models;
  - e111 / e138: 1 of 9 binders fixed;
  - e164 / e165: 0x0041B550 is left.
- **blocked:** all port.
  - e40: 7 grades; its best try was DIFFER 81 v 81, a register choice in the final `add`.
  - The other 14 were read and blocked without a grade, each with the missing header or opt-in named. The next agent takes them.

## Rows (66; tests unchanged)
- **RENAMES section 1:** 66 self renames. Every new spelling is in Ego_r.pdb or FableWin.pdb, or is the compiler's spelling of an
  STL instantiation whose types the PDB and the callers prove.
- **RENAMES section 2:** 10 binder respellings.
- **Rule slips, fixed by the agent before hand-back:** NF39 dropped 0x00A52650's `// Retail-asm:` line and then restored it.
  It also corrected 28 RENAMES lines to module = class, name = method.
- **Recheck (`logs/recheck_part0..1.log`):** all 66 graded from the repo after landing: 13 MATCH and 53 RELOCATION_MATCH, every
  test PASS. No landed funclet includes a changed row, and funclet_includes_check reports 1,355 checked, 0 broken.
- **Headers:** ten overlays, opt-in additions only on top of the current repo headers (NF40's included); every conditional
  balances. New opt-ins, among others:
  - CCreatureAction_UnsheatheToShowToSheatheTrophy `..._IMPLICIT_DTOR`;
  - CTCCreatureModeManager `FABLE_RETAIL_CMODEBASE_CTOR`;
  - CTCPerceiveThing `..._TIMEVISIBILITY_INLINE`;
  - CCombatActionBase `..._TARGETCTOR`;
  - CEngineSurfaceCompositionManager `..._CALCCOLOURDIFFERENCE`;
  - CTCLightningNode `..._BRANCH_CTOR_DECL` / `_SETTERS_DECL`;
  - CFileInstaller, CGraphicInfoFrame, CTCHeroAttachableAppearanceModifiers, CDefinitionManager.
- **Bindings:** rebind run, bindings_multi_check at baseline.
