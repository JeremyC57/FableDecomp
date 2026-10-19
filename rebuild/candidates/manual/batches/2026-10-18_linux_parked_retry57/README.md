# Handle-list insertion and static-mesh construction

007464a0 (177 bytes): corrected propagated renderer label to handle-list InsertDefault. Root audited all native calls, handle vtable identity and counted copy/release lifecycle, allocation of16-byte nodes and circular insertion links. Shared persistent-pointee layout records retained. Exact-source fixtures cover empty and middle insertion, null/last/shared ownership, allocation-time temporary mutation, copy retain and subsequent temporary/node release. Five total grades.

0077bd70 (177 bytes): PDB constructor signature restored with const vector/orientation/channel references, interpolation enum and genuine CRGBColour member/copy constructor. Native0x4c layout independently checked, including base defaults, full orientation, colour channels, lit-bit preservation and unused identifier. Exact-source fixtures cover all lit/bullet flag combinations, copied inputs, native layout and untouched padding/bits. Three total grades including review correction. Both oracles exclude15 proven trailing int3 bytes.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
