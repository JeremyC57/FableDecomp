# Two more previously skipped renderer functions

00b86b00 CEngineScreenEffectDisplacementRenderer::SetEffectQuality (163 bytes), recovered at author attempt 8. Uses the shared renderer storage header and a method-only view; video-quality input uses its verified retail option prefix. Integer local flags reproduce full-register boolean normalization. The exact-source fixture checks all 32 combinations of capability, requested modes, initialization outcome and page validity, callback receiver/order/handle, return value, enabled flags and active-state updates. It also checks an adjacent sentinel and retained page handle.

00b91dc0 CEngineSceneGrid::GetCellRenderMask (176 bytes), recovered at author attempt 2 after correcting the cell prefix. Uses the shared primitive header, and the retail grid/cell models from landed relatives. The actual-source fixture checks local mask chains, four-child recursion, zero/null leaves, cache reuse, preservation of other packed flags, nonzero x/y and the high mask bit. The target test includes the real primitive header; the host alone uses a portable storage adapter.

Both were previously SKIP/not reached. Both host and canonical VC7.1/Wine fixtures pass, with independent RELOCATION_MATCH. No source-changing coordinator grade was needed. Final landing evidence: landing_evidence.txt.
