# Chunk 3004 header overlay (grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3004/hdr`)

This overlay is the merge of the two prior hand-backs this retry pass starts from; chunk 3004 made NO header change of its own (every
probe macro added during measurement was removed again; the files below are byte-identical copies of their prior-chunk versions).

- From chunk 2024 (`prior/2024/hdr`; its README copied as hdr/README_chunk2024.md): engine/CEngineLightingManager.h, CEngineScreenEffectOutlineGlow.h,
  CEngineShadowRenderer.h, CGPUScratchBufferTextureManager.h, CMatrix4x4.h, CRenderStateCaptureBlock.h, CRenderTarget.h,
  CShadowClipVolume.h, CShadowRendererSceneData.h. Used by 0x00B4FC40 (RELOCATION_MATCH): CMatrix4x4.h
  (`FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4`), CEngineShadowRenderer.h (`_MEMBERS`), CEngineLightingManager.h (`_PEEKGLOBALSHADOWMODE`),
  CShadowRendererSceneData.h, CShadowClipVolume.h, CRenderStateCaptureBlock.h.
- From chunk 2014 (`prior/2014/hdr`; its README copied as hdr/README_chunk2014.md): engine/C3DAnimation2.h, C3DAnimationBuilder.h, C3DAnimationSequence.h,
  C3DAnimationSequenceData.h, C3DAnimationStats.h, C3DBone2.h, C3DKeyframe.h, C3DMesh2.h, C4DVector.h, CMatrix3x4.h, CQuaternion.h,
  CSimpleMemoryPool.h, N3DAnimationBuilder_COutputAnimation.h (the acctB_r3001 base files + chunk 2014 opt-ins).
- No file name exists in both. 0x00B4FC40 now also sees chunk 2014's C4DVector.h / CMatrix3x4.h (supersets of the repo files: additions
  under opt-in macros only); it was regraded with the merged overlay (RELOCATION_MATCH).

See /opt/fable_lane/w22/agent3004/prior/2024/hdr/README.md and /opt/fable_lane/w22/agent3004/prior/2014/hdr/README.md for the per-opt-in
evidence tables (copied unchanged).
