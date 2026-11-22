# NF250 header overlay (2026-11-22)

Base copies in `../orig/engine/` (taken from the repo tree at the start of this run; neither header is in NF249's overlay).
Both edits are add-only `#ifdef` blocks under new member-specific macros; no existing line changed, so the default path is token-identical.

- `engine/CWorld.h`: `FABLE_RETAIL_CWORLD_GETLOCALSEED` -- `unsigned long* GetLocalSeed() const;` (declaration only), added right after the
  existing `GetWorldSeed()` declaration in BOTH class branches (retail branch line 203, default branch line 966). Evidence: Ego_r.pdb /
  FableWin.pdb ?GetLocalSeed@CWorld@@QBEPAKXZ; Ego_r donor order GetLocalSeed / GetWorldSeed = retail 0x0049D800 / 0x0049D810.
- `engine/CMainGameComponent.h`: `FABLE_RETAIL_CMAINGAMECOMPONENT_GETLOCALSEED` -- `unsigned long* GetLocalSeed() const { return
  const_cast<unsigned long*>(&LocalSeed); }` (public, after the SAMPLEBANK_WORLDSEED block, mirroring its GetWorldSeed). Evidence: FableWin.pdb
  ?GetLocalSeed@CMainGameComponent@@QBEPAKXZ; LocalSeed +0x16140 (Ego_r.pdb, already in the header).
- pin_asserts.py (repo include v this overlay): `pinned 0` for each header.
- equiv_check.py (repo tree v a hard-link copy with the two overlay files): 57,125 landed rows (sources + tests) checked, 0 differing
  (work/equiv.log). 1,223 landed sources / tests mention one of the two headers; none defines either macro, so the opt-ins change none of them.
- Users: only the new row 0x0049D800 defines the two macros.
