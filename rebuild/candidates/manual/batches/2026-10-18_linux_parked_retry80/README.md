# Publish current screen effects

006b8640 (233 bytes): updates screen-effect priority stacks, copies four parameter groups by value and sends each to the current spell interface with interpolation enabled. PDB field names and native offsets are retained; no generated headers exist for these types. The native stack uses smaller STL storage than the donor; exact field offsets are independently checked against native loads. CurrentDisplacementParam is the original CRGBFloatColour; the getter wraps it in the actual CDisplacementParams type.

Main independently audited all four setters884e90/884f60/884f90/884fe0, argument order and widths, native update880e50 and full parent body. Exact-source fixtures validate update-before-read, all parameter values, by-value copies surviving source mutation, fresh reads of later parameters, receiver reload after callback replacement, call order, native offsets and repeated calls. Host-O2 plus canonical VC7.1 parity/behavior pass. Three new grades (including type/header integrations), plus UNCERTAIN historical reservation4.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
