# NF450 hand-back (2026-11-25): T1 special-member caller respells, continuing NF449

`NOTES.md` has NF450's method, gates and resume point.

## Landed: n1 (14 rows)
- **9 RESCOPE rows (r449a):** each row's own CScriptModuleBase model is wrapped in `namespace NCameraModes`. Target 0x005ED070.
- **2 r449d rows:**
  - 0x00529CC0, a qualified destructor call through `engine/CBaseClass.h`;
  - 0x004C3360, the same through `engine/C3DAnimationInterpolator.h`.

  Both use the engine headers, not a bodiless class, so strict vland still passes.
- **3 r449c rows:** 0x00442CC0, 0x00443010 and 0x00443080 now call FableWin-only NPlayerGui destructors. Their targets are registered in `descriptive_names.tsv` as `one-source:fablewin-pdb`:
  - 0x00441B90 CDrawStatUpdates;
  - 0x004420D0 CDrawMeleeActionButtonEffects;
  - 0x00442190 CDrawSpiritTargetingCursor.
- **Tracker:** 1 address split_fixable → agree_pdb, 3 → agree_descriptive, 2 partly closed, none worse. descriptive_check: 187 lines, 0 FAIL.

## Rulings (coordinator, 2026-11-25)
- **r450a: declined.** 0x00AF7C80 ported onto `engine/CDecal.h` gains `/DEFAULTLIB:"libcp"`, because the header pulls in STLport. Including a header must not add a library directive the row's object did not have. Wait for a CDecal.h opt-in that leaves out the CArray members.
- **r450b: declined.** In m1, six rows would rename their own base-class model to `NPlayerGui::CStatBase`. `engine/NPlayerGui_CStatBase.h` exists, so the rename would make a local model of a class that has a shared header. Port those rows onto that header instead. The four DTORX rows of m1 can land on their own if they do not depend on the six.

## Findings
- **NF449's 12 DTORX "models the class" lines were census false positives.** The compiled objects already call the PDB destructor (`null_census.tsv`).
- **The 0x00A0D3D0 stand-ins survive only as 11 stale table lines** (`STALE_TABLE_00a0d3d0.tsv`). They need a witness check before deletion, as NF448 did.
- **T3 lines fully resolved by n1's callers:**
  - nf303 0x00441B90;
  - nf334 0x004420D0;
  - nf295 0x00442190;
  - nf301 0x005ED070.

  They are keyed on target addresses, so the lander does not close them; close them with the next batch that touches those rows.
