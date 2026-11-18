# 0x00B6DC40 naming evidence

Ego_r 0x00A1B7B0 and FableWin 0x02D4BF50 agree on `?ProcessSeaLoading@CEngineWaterRenderer@@AAE_NXZ`. Ego_d identifies the method and its scoped smap_index and stream locals in `Engine_Water_Renderer.obj`. Retail CEngineWaterRenderer::FrameStart calls the member at 0x00B72148 and 0x00B7216C.

The shared declarations bind the 31 caller relocations. The state-machine draft still differs around the empty-request branch. The retail branch skips the background flag store when the flag is already false, and the rebuilt branch shares an epilogue that restores edi before storing the flag. The affected displacement and instruction bytes are listed in `TAIL_AUDIT.json`.

The original ReleaseSeaBuffers body appears separately in the object. It matches all 48 retail bytes at 0x00B6D480 after restoring the two buffer-uninitialisation call operands. See `SAME_TU_HELPERS.json`.
