# NF462 hand-back (2026-11-26): r460d and r460a rebuilt on header views

`NOTES.md` covers the method and resume point; `QUEUE_RESULTS.tsv` and `RULINGS_NEEDED.tsv` are beside it.

## Landed: 16 rows on 6 headers (+25 proven)
- **bB2_psc_alloc_headers, 14 rows (r460d, replacing NF460's declined bB):**
  - The 7 NParticleEngine factories move onto their PDB `?Alloc@X@NParticleEngine@@SIPAVCPSCBase@2@XZ` and call the namespace-qualified ctors, with no `reinterpret_cast`.
  - The 7 DatabaseClone rows are re-authored as the PDB virtual `?DatabaseClone@X@NParticleEngine@@UBEPAVCPSCBase@2@XZ`. That includes 3 that the caller census found calling the factories through stand-ins no table line bound.
  - **Header changes, all add-only:**
    - `CPSCRenderSprite.h` and `CPSCEmitterGeneric.h` get `_ALLOC` opt-ins;
    - `CPSCLight.h`, `CPSCUpdateNormal.h` and `CPSCSplineBase.h` get containers-free views (no libcp, the r459d style).
  - **Table:** 7 replaces and 4 deletes.
  - **WARN sets:** 15 at HEAD, none now.
- **bM_creaturemode_alloc, 2 rows (r460a):** 0x0073EC10 and 0x00743680 move onto `?Alloc@<Mode>@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z`, over a new containers-free view of `CTCCreatureModeManager.h`. 2 table replaces.
- **T3 lines closed:** the lander released them with `REL=nf303,nf386`; they were keyed on these rows' own addresses and this batch is their port. They are nf303 0x00AF7FB0 / 7ED0 / 8020 / 7D10 and nf386 0x00AF7FB0.
- **NAME_FIXES_QUEUE:** 9 lines marked done.
- **Coordinator checks:**
  - bindings_multi: 1;
  - funclets: 1,704 checked, 0 broken;
  - s units: 159 checked, 0 failed;
  - descriptive_check: 2,026 lines, 0 FAIL.
  - Tracker: 18 split_fixable + 7 agree_unverified → agree_pdb, with nothing worse.
- **equiv:** 57,160 rows checked; the 12 that differ are all batch rows, 0 outside (`EQUIV_CHECK.txt`).

## Rulings
- **r462a: granted (coordinator).** CPSCAttractor and CPSCSpline have no shared header, so local classes deriving from their RTTI base's shared header are allowed. The no-local-model rule covers only classes that have a header.
- **r462c: 0x006F5610 stays held.** Its header defines the ctor inline, and building from the full model would emit the class's complete `??_7` where HEAD has a one-slot table. That waits on the r459a short-vftable question (NF461 is measuring it).

## Next (NF462 resume point)
- **The CModeStunned unit:** ctor 0x00740160, binder 0x008ED950, factory 0x007427F0, on the bM views.
- **0x00861E00:** three bound spellings to respell onto `??0CModeStandBase@@QAE@AAVCThing@@@Z`.
- **The remaining particle debt:** CPSCRenderMesh Alloc 0x00AF7D80, and DatabaseClone 0x00B1FF80 and 0x00B0A190.
- **The 38 unreached queue lines** from NF460, including the MixFade opt-ins on the GameCODA render headers.
