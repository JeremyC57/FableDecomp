# 0x006B16B0 probes

| Attempt | Source strategy | Grade | Diagnosis |
|---|---|---|---|
| sol2_01_original_palette | PDB member types, real const multimap iteration, scoped theme handle and source loop condition | Main RELOCATION_MATCH581/581 | Carried GetDef body independently differed121/113: converting operator= cached the pointee. |
| sol2_02_original_handle_conversion | Genuine GetDef SetDynamicCast conversion through the existing reread body | Main RELOCATION_MATCH581/581; helper RELOCATION_MATCH113/113 | Exact whole call graph, all29 relocation operands audited. |
| sol2_03_final_header_proof | Final header comments and pinned assertions; same source strategy, verification only | Main RELOCATION_MATCH581/581; helper RELOCATION_MATCH113/113 | Final delivered overlay verified; no source-shape attempt claimed. |

Behavior: full581-byte function body and full GetDef helper body retained in standalone C++03 fixture. Exhausts16 flag masks,1..4 theme-count limits,0..3 multimap entries including duplicate keys, zero/nonzero sample IDs, null manager, invalid helper index. Checks channel/map order, arguments, flag precedence, ignored false returns, repeated count queries and balanced definition counts. VC7.1 /Od /W3 /GR- without /EHsc and g++64 -std=c++03 both print006b16b0_TEST PASS. Runtime records are in runtime/behavior_tests/SOL2_RESULTS.json.
