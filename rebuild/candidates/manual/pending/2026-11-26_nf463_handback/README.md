# NF463 hand-back (2026-11-26): CModeStunned unit, particle debt, GameCODA MixFade statics

`NOTES.md` covers the method and resume point; `QUEUE_RESULTS.tsv` and `RULINGS_NEEDED.tsv` are beside it.

## Landed: 10 rows (+7 proven, +4 best guesses)
- **bS_cmodestunned_unit, 3 rows:**
  - ctor 0x00740160 → `??0CModeStunned@NCreatureMode@@QAE@AAVCThing@@@Z`, a parameter-only respell (r463b);
  - binder 0x008ED950, which used to model it as CModeDazed;
  - factory 0x007427F0 onto its PDB `?Alloc`.
  - The NAME_FIXES identity line (NF455) is done.
- **bP_psc_alloc_debt, 3 rows:**
  - 0x00AF7D80 `CPSCRenderMesh::Alloc`, a local class on `CPSCBase.h` (r462a);
  - 0x00AF8100 `CPSCDecalRenderer::Alloc` and 0x00B1FF80 `DatabaseClone`, on a new add-only `_ALLOC` opt-in in `CPSCDecalRenderer.h`.
- **bF_salrender_mixfade, 4 rows:** the GameCODA SDK private statics `MixFadeIn` / `MixFadeOut` of CSalRenderDisable and CSalRenderPan, on add-only `_MIXFADE` opt-ins.
  - This removes 2 duplicate definitions that existed at HEAD.
  - Catalog: names relabelled, Module `_global` → the SDK class (r463c).
  - 4 `one-source:gamecoda-sdk` registry lines.
- **Coordinator checks:**
  - bindings_multi: 1;
  - funclets: 1,704 checked, 0 broken;
  - s units: 159 checked, 0 failed;
  - descriptive_check: 2,030 lines, 0 FAIL.
  - Tracker: 7 → agree_pdb and 4 standin → agree_descriptive, with nothing worse.
  - equiv: 57,160 rows checked; the 6 that differ are all batch rows, 0 outside (`EQUIV_CHECK.txt`).

## Held
- **bQ (0x006EA770 ComputeAttackVector → GetAttackerPlacementVector).** It passes its gates, but the lander held it on T3 nf287. That line is keyed on the row itself but concerns a different stand-in in it (`GetPreferredPos@CCombatPositionMethods`), which bQ does not fix, so it was not released. Land it together with that port.
- **r463a: the 0x00861E00 CModeStandBase family** (70 or more binders across three stand-in spellings).
  - It waits on the r459a short-vftable question: moving a ctor row onto the header grows its `??_7` from 4 to 8 bytes and adds a `??_G` COMDAT.
  - It also needs 0x00740340, which is under COORD_HOLD_NF455 (a duplicate definition).
- **Measured this pass:**
  - 0x00B93290 needs an EnumerateScene opt-in on `CEngineScene.h`.
  - The WatchFor* lines are a port onto the CScriptThing header, not a rename.
