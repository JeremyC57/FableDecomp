// Grade: DIFFER(-v860) -- not authored: retail rounds with 4 bare dword `fistp` (float store/reload, no _ftol), the original hand-written assembly float-to-int helper -- known lane gap (GOTCHAS wave 25 chunks 145/150/164); byte purity forbids reproducing it
// CPSCDecalRenderer::AddToBoundingSphere(CBoundingSphere& sphere) const -- grow the bounding sphere by the decals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1DC70, 860 bytes. Parked without a body: every compiler-emitted form of the
//   rounding (a C cast -> _ftol call, /QIfist -> qword fistp) differs from the hand-written assembly dword store, so grading a draft
//   would only confirm the gap.
