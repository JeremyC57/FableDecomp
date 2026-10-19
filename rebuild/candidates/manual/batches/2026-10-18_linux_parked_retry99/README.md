# Depth surface allocation and graphic frame data initialization

00f37390 (162 bytes): actual CSurface and CPixelFormat headers, native 2c resource entries and surface vtable audited. Main checked allocation, depth-surface creation, headless path and release helper identities; fixture covers success/failure/skip and replaces the resource array during allocation to verify the required fresh base reload and stride. One new grade plus UNCERTAIN historical reservation4.

009fe380 (181 bytes): RTTI establishes nested CGraphicInfoFrame::CFrameData; actual resource, list, frame and donor bank headers retained with documented native views. Main audited constructor/helper identities, native44 size, vtables, optional managed size and bank resource-list offsets. Fixture checks initialized fields and frame wraparound, and mutates the bank during SetResourceSize to confirm the later AddResource reload. Six new grades plus UNCERTAIN historical reservation4.

Both optimized host fixtures pass. Root corrected a fixture-only expanded-header assertion name stripped by the legacy lander preprocessor; native ABI assertions remain active, production unchanged. Canonical VC7.1 independent byte/behavior validation required before landing.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
