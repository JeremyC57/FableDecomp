# AUTH1 header overlays
Per-row overlay trees live in <addr>/hdr/ (FABLE_HDR_OVERLAY=<addr>/hdr); base copies of the CURRENT repo headers in orig/.
- 00a4efc0/hdr: the repo's pending/2026-11-20_p25_open/00a4efc0/hdr.diff applied to today's headers (C3DKeyframe, C3DAnimFileSequenceChunk,
  CQuaternion (hunk offset +4), C3DAnimationSequenceData, C3DVector [DIAG-only opt-ins, drop before a batch]) plus a NEW add-only opt-in in
  engine/CSimpleMemoryPool.h: FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD (Ego_d-shaped AllocateType<T>, lines 21..34; the dead
  assert condition comes from FABLE_ALLOCATETYPE_ASSERT_COND in the row -- to be fixed to one text before any landing).
  pin_asserts.py rebuild/include 00a4efc0/hdr engine/CSimpleMemoryPool.h: pinned 0 (no FABLE_STATIC_ASSERT in the header).
- 008e18d0/hdr: engine/NParticleEngine_CParticleUpdateParameters.h with the add-only FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR
  opt-in (#ifndef round the declared virtual dtor), re-made from p25N's description; pin_asserts: pinned 0.
- 00bd3070/hdr, 00502e90/hdr: copied unchanged from the repo's p25S folders (fresh against HEAD as of 2026-11-21).
