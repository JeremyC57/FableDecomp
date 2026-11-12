# 2026-11-12 landing dk -- chunk Q20: 2 of 20 parked near misses matched

Source: `pending/2026-11-12_linux_handQ20/` (README, STATUS, NOTES, LESSONS, LABELS, NAME_FIXES, ATTEMPTS, differ/, hdr README).
Landed (RELOCATION_MATCH, test PASS under VC7.1): 0x007B0360 CTCFishingSpot::FrameUpdate, 0x008B7040
CCreatureAction_JumpOutFromCover::Initialise.

Headers (11; three-way merged against d7a8bfdc9, the base Q20 started from; C3DVector.h had changed upstream in landing di and merged
cleanly; hdrcheck2 default=same everywhere, and the three DIFF macros are new): new CCreatureAction_JumpOutFromCover.h,
NThingShot_CTCShotDefinitionMultiArrow.h, NThingShot_CTCShotDefinitionDrainLife.h; opt-ins in C3DVector.h (NORMALISE_LOCALS_XYZ;
the existing NORMALISE_LOCALS `#if` becomes an `#elif` after it), CRenderManager2D.h, CPixelFormat.h, CTexture.h, CTCPhysicsBase.h,
CTVertexRHWColSpecTex1.h, CThingDrainLifeShotDef.h, NThingShot_CTCShotDefinitionBase.h.

Coordinator fixes: the agent's oracle.tsv had no header row and carried the retail int3 padding; it was rebuilt with a header and
without padding. Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
No name fixes (NAME_FIXES.tsv is header-only).
