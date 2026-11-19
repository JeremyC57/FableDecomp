# Landing hand4b b5 (2026-11-19): NThingShot::CTCShotDefinitionDrainLife::InitMovement (0x007BD845, 642 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS; no header changes. The landed MultiArrow twin 0x007BCF7F text in FableWin /Od order with existing
CRIGHTHANDEDSET_CTOR_INLINE (frame 0x5C, as same-TU 0x007BBE34 / 0x007BBB3B) and C3DVECTOR_SUBTRACT_XY_GROUP (`// Bracket:` -- puts up's
`lea ecx,[ebp-20h]` at 0x007BD90F; other placements measured in the line).
Queued test-fidelity fix: the landed twin test tests/00/7b/NThingShotCTCShotDefinitionMultiArrow_InitMovement_007bcf7f_test.cpp models CRightHandedSet(a,b)
as Forward=a / Up=b; retail 0x00487050 (and the header) store a at +0 (Up), b at +0xC (Forward). It passes (self-consistent) but its labels are swapped.
