PROBE overlay (B24C5) for 0x00BE3970 -- not a hand-back overlay. = ../../hdr (B24C4: repo-current headers + FINDVERT, EDGE_HELPERS,
GETEDGESTRIP) plus:
- engine/CPatchTesselationEdgeStrip.h: FindVert body replaced by the FableWin-exact while-loop text (fwsize 48i/140b SAME); its inline
  keyword is the probe macro FABLE_PROBE_FINDVERT_INLINE (rows define it as FABLE_FORCEINLINE: plain inline leaves a call, f2).
- engine/CEdgeStipVertexArray.h (copy of the current repo header): probe FABLE_PROBE_EDGESTIP_ASSERT -- operator[] with a release
  ASSERT dead arm (FABLE_PROBE_EDGESTIP_ASSERT_NC / _C = the assert macro per overload, FABLE_PROBE_EDGESTIP_COND = condition).
  Evidence: FableWin.pdb CEdgeStipVertexArray<T>::operator[] 73i/245b (const and non-const) = plain body + one ASSERT.
