# NF157 header overlay

## engine/NCameraModes_CBase.h
- Base: the repo working-tree copy at session start (orig/engine/NCameraModes_CBase.h, byte-equal to rebuild/include at hand-back).
- Added (add-only, one `#ifdef` block in the default, non-CYLINDER branch, right after GetModeType, public section):
  `FABLE_RETAIL_NCAMERAMODES_CBASE_FRAMEUPDATE` declares `void FrameUpdate();` = Ego_r.pdb 0x008459B0 / FableWin.pdb 0x02883CC0
  `?FrameUpdate@CBase@NCameraModes@@QAEXXZ` (0x0084DC10, parked self row; landed 0x00873DB0 binds the same spelling). Declaration only,
  no layout change. Used by rows/0069b790.cpp.
- Without the macro the header is token-identical to the base (the block is the only difference: `diff orig/ hdr/`).
- pin_asserts.py: `engine/NCameraModes_CBase.h pinned 0` (every assert is FABLE_STATIC_ASSERT_AT with its own line tag; none moved).
- Landed includers: 55 rows under rebuild/src/compiled include engine/NCameraModes_CBase.h; none defines
  FABLE_RETAIL_NCAMERAMODES_CBASE_FRAMEUPDATE, so the opt-in changes none of them. The CYLINDER branch already declares FrameUpdate; the new
  macro is only in the default branch, so no member is declared twice. NF155 / NF156 overlays do not carry this header.
