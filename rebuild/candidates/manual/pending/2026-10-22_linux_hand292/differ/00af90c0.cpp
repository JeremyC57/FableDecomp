// Grade: DIFFER(-v588) -- not authored: retail rounds with 3 bare dword `fistp` (float store/reload, no _ftol), the original hand-written assembly float-to-int helper -- known lane gap (GOTCHAS wave 25 chunks 145/150/164); byte purity forbids reproducing it
// CPSCRenderSprite::GetComponentCost() const -- the sprite renderer cost estimate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF90C0, 588 bytes. Parked without a body: every compiler-emitted form of the
//   rounding (a C cast -> _ftol call, /QIfist -> qword fistp) differs from the hand-written assembly dword store, so grading a draft
//   would only confirm the gap.
