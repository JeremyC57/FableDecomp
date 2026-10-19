# Environment displacement interpolation

00885900 (226 bytes): genuine four-channel monochrome colour interpolation with ordered clamp to[0,1]. PDB verifies native current DisplacementParams+0x54 and previous+d4, CRGBFloatColour channel order/width and returned CIDrawEnvironmentDisplacementParameters. No generated class headers exist for these types. Main independently audited complete native x87 arithmetic and constant pools; no external callees or vtable relocations.

Exact-source fixtures test distinct positive/negative channels, endpoints/interior weights, finite and infinite clamping, signed zero, NaN weight propagation, zero-weight NaN source propagation and unchanged receiver bytes. Native offsets and return size asserted. Host-O2 and canonical VC7.1 byte/behavior verification pass. One new grade plus UNCERTAIN original reservation4; no fabricated historical count.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
