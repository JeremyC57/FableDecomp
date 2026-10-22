// Grade: DIFFER(-v576) -- not authored: retail rounds with 2 bare dword `fistp` (float store/reload, no _ftol), the original hand-written assembly float-to-int helper -- known lane gap (GOTCHAS wave 25 chunks 145/150/164); byte purity forbids reproducing it
// CPSCDecalRenderer::DecalCollides(...) const -- decal overlap test
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1D550, 576 bytes. Parked without a body: every compiler-emitted form of the
//   rounding (a C cast -> _ftol call, /QIfist -> qword fistp) differs from the hand-written assembly dword store, so grading a draft
//   would only confirm the gap.
