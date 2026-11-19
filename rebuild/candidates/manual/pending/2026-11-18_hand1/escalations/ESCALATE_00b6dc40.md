# ESCALATE 0x00B6DC40 CEngineWaterRenderer::ProcessSeaLoading -- best DIFFER(775v771) (probe/b6_b.cpp, /d2inlT200)

Constraint: in the empty-request block, retail's `if (HaveBackgroundSea) <ReleaseSeaBuffers inline>` false edge
(`je 0x00B6DD06`) goes to a LOCAL `return false` epilogue that the inlined body falls into (pop edi after the 0x276 store).
Ours sends that je to the final shared `return false` epilogue (0x00B6DF39) and gives the body its own epilogue copy with
`pop edi` scheduled into it (4 bytes longer). With the draft's unconditional `HaveBackgroundSea = false` (store outside the
if) the local epilogue survives but the store is unconditional (2 hunks, 771v771) -- not retail.
Ego_d: no empty_request local (only {smap_index}, {stream}); ReleaseSeaBuffers is a plain member defined earlier in the TU.
Measured, all 775: plain member ReleaseSeaBuffers; return HaveBackgroundSea; return in both arms; early `if(!Have) return`;
whole function as if/else with one final return; same-TU CloseStaticMap and LoadSea bodies above the row (both
RELOCATION_MATCH at 0x00B6DB80 / 0x00B6D420 from this text, row unchanged). Needed: why VC7.1 does not merge retail's
local false-epilogue with the final one (something non-emitting in that join, or a different inline/budget structure).

Addendum: the same-TU CloseStaticMap text matches at PLAIN flags with the same ReleaseData pair inlined, while this row needs
/d2inlT200 in our TU. One retail TU has one flag set, so the /d2inlT200 device may itself be distorting the row's block
structure (the inline budget, not the statement shape, may be the real lever).
