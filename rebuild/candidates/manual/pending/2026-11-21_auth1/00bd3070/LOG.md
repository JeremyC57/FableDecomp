# 0x00BD3070 RenderMeshPrimitive -- AUTH1 log
1. b6.cpp (p25S rule-compliant base: no row asserts + landed CARRAY_INDEX_ASSERT) + p25S hdr overlay: DIFFER(11288v11288), 86 normalised
   lines in 19 hunks (x87 operand order 0xBD32F4-ish, [eax+2814h] load order x3, test/or orientation x2, a float-copy block). Reproduces p25S.
Next (p25S open item, required before tie work): move the ~15 row-local FABLE_FORCEINLINE member definitions (PeekGraphic, SetTextureFactor,
SelectVertexBuffer, FlushRender, ApplyPixelShader, ...) into their shared headers as add-only opt-ins, then regrade the ties.
