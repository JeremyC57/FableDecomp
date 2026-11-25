# NF431 hand-back (2026-11-25): the CGameScriptInterface script API, round 1

The family is 152 open tracker addresses. `NOTES.md` has the method, the gates, and a resume recipe for the next agent.

## Landed: 18 rows (`batches/2026-11-25_landing_nf431_cgsi_a`)
- **What the rows are.** RTTI-placed virtual slots that were landed as free stand-ins. Each is now the real `CGameScriptInterface::<Method>` on `engine/CGameScriptInterface.h` (`FABLE_RETAIL_NSCRIPT`). That header already declares all 918 slots with the PDB signatures.
- **Shapes:**
  - 9 tail jumps / forwards;
  - 6 member reads / writes through World or a singleton;
  - 2 constant-argument calls;
  - 1 empty body.
- **Sources per name.** Each self has its RTTI placement plus the Ego_r / FableWin name sets. Callee names and argument order come from FableWin's /Od body, checked against retail's call target.
- **Grades** are unchanged (7 MATCH, 11 RELOCATION_MATCH), and every test passes. The mutation control fails on all 18 rows.
- **Name gate:** PASS on every row and test.
- **Copies and vftables:**
  - pk_all reaches no differing copy (0 NEQ).
  - No `??_7` is emitted.
- **Header opt-ins.** Five, all add-only:
  - CGameScriptInterface.h `_IMPSETENTITYASALLY`;
  - CEnvironment.h `_DEFAULTTHEME_CHANNEL`;
  - CEnvironmentSpellEffectInterpolator.h `_SCRIPTFORWARDS`;
  - CPlayerGui.h `_SETSHOWMONEYBAG`;
  - CThing.h `_ISINLIMBO_DIRECT`.
- **Equivalence.** The coordinator reran equiv_check copy against copy on the tree at landing time.
- **Table changes:**
  - 3 stand-in callee lines deleted;
  - 2 PDB callee lines added: `CUserProfileManager::SetSoundAsMuted` at 0x00408340 and `CEnvironment::TransitionToDefaultTheme` at 0x006B3880.
- **Released T3 lines:** 10 nf324 port lines (`T3_RESOLVED.tsv`).

## Judgment calls (coordinator, 2026-11-25)
- **CWorld members accepted; accessors queued as debt.** Four rows (0x0088F450, 0x0088F950, 0x0088E380, 0x0088F460) touch the public CWorld member directly, where FableWin calls an inline accessor.
  - The bytes are the same, and no name is bound by either spelling.
  - Adding the accessors as `engine/CWorld.h` opt-ins and respelling is queued (nf431 CWorld accessors line).
- **`ISINLIMBO_DIRECT` accepted with an nf399-form debt line.** 0x0088FD60's TU emits an unreferenced pick-any `?IsInLimbo@CThing@@QBE_NXZ` in the byte form.
  - Retail places no out-of-line copy, and none is referenced.

## Held: 0x008901E0 (`hold/`)
Every gate passes except name_gate. Its jump target, 0x006B8390, carries a made-up `std::_Cons_val` label in the catalog.
- The real name is `CEnvironmentSpellEffectInterpolator::DisplacementMonochromeEffectColourFadeOut`, in both PDBs, and FableWin's body equals retail.
- Land the row after that name fix, and re-run vland first.

## Name fixes (queued in NAME_FIXES_QUEUE.tsv)
- 0x006B8390 and 0x006B8310 (the spell-effect interpolator).
- Five CTCAIScratchpad static data members. Each rests on a FableWin.pdb public plus the matching store / read in FableWin's /Od body; Ego_r lacks these names.

## Residue (133 rows; `RESIDUE.tsv`, `T3.txt`; nf431 T3 family lines)
| Group | Rows | What it needs |
|---|---:|---|
| body ports | 80 | per-row ports onto the CGameScriptInterface header |
| non-RTTI split / stand-in research | 23 | research |
| T1 caller respells | 8 | gen423 |
| CTCAIScratchpad statics | 7 | one data-symbol unit |
| IsThingA* | 5 | a `CThing.h` GetTC opt-in |
| CScriptThing by value | 5 | |
| STLport container signatures | 4 | |
| 0x00890250 | 1 | flatten_pilot HELD |
