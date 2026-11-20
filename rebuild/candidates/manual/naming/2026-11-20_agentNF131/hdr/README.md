# NF131 header overlay (2026-11-20)

Every overlay is a copy of the repo working-tree header (base copy in ../orig/, byte-equal to the repo when taken) plus `#ifdef`
opt-ins only. Without the opt-in macros each header is token-identical to its base (the default path is unchanged).

## engine/CAIStateGroup_HandyMan.h
- Base: rebuild/include/engine/CAIStateGroup_HandyMan.h (orig/engine/CAIStateGroup_HandyMan.h).
- Added: `FABLE_RETAIL_CAISTATEGROUP_HANDYMAN` (q4946, row 0x0091B170): the polymorphic retail class
  `class CAIStateGroup_HandyMan : public CAIStateGroup_WorkBase<CAIStateGroupBase>` (RTTI) with IsRunnable (UAE, slot 11), Update (UAE, slot 12),
  private AcquireWorkSpot (AAE) and WorkSpot +0x14, both PDBs; sizeof 0x18 asserted. The whole flat generated body moved into the `#else`
  branch unchanged. Needs FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS.
- pin_asserts.py: pinned 0 (the flat asserts are already FABLE_STATIC_ASSERT_AT); it warned only about the new assert inside the opt-in
  (line 23, compiled only under the opt-in).
- Landed includers (grep): 0x008FDD5F, 0x00957B80, 0x0091B170 (this hand-back), 0x0091B190, 0x0121F86F. None defines the new macro, so
  the opt-in changes none of them.

## candidates/landscape_background_lod_compiled.h
- Base: rebuild/include/candidates/landscape_background_lod_compiled.h (orig/candidates/).
- Added (q6182, row 0x00BDC180):
  - `FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_GETBLOCKDATA`: `const unsigned char* GetBlockData(long) const` (Ego_r.pdb 0x00A41E20 +
    FableWin.pdb 0x02DF74B0 ?GetBlockData@CEngineStaticMapFileBlockManager@@QBEPBEJ@Z) instead of the two-argument stand-in.
  - `FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_LOD_CTOR`: the PDB ctor `(const void*, unsigned long)` (??0CMemoryDataInputStream@@QAE@PBXK@Z) instead
    of the one-argument stand-in, plus a 4-byte tail so the class has the Ego_r size 0x1C; under this macro the size assert reads 0x1C (a new
    assert inside the opt-in), the old 0x18 assert sits unchanged in the `#else`.
- pin_asserts.py: pinned 9 (every FABLE_STATIC_ASSERT below the inserted lines became FABLE_STATIC_ASSERT_AT(<old line>, ...), so the
  typedef names are unchanged); the `#ifdef/#else/#endif` around the 0x18 assert only moves the already-pinned line.
- Landed includers: the row 0x00BDC180 and its test only (both in this hand-back, both define the two macros).

## engine/CAIStateGroup_FollowPlayer.h
- Base: rebuild/include/engine/CAIStateGroup_FollowPlayer.h (orig/engine/).
- Added: `FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER` (q5141, row 0x0094EF70): the polymorphic retail class `class CAIStateGroup_FollowPlayer :
  public CAIStateGroupBase` (RTTI) with OnActivate (UAE, slot 2, both PDBs), Player +0x14 (opaque 8 bytes, CIntelligentPointer<CThingPlayerCreature>)
  and Proximity +0x1c; sizeof 0x20 asserted. The flat generated body moved into the `#else` branch unchanged. Needs FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS.
- pin_asserts.py: pinned 0 (flat asserts are already _AT); it warned only about the new assert inside the opt-in.
- Landed includers (grep): 0x008FDDF5, 0x0094EF70 (this hand-back), 0x0094EF90, 0x0094F000, 0x0121F86F. None of the others defines the new macro, so the opt-in changes none of them.
