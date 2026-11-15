# Independent main verification: 009FA780

Accepted CTexture::BltToRenderTargetWithGPU after fresh MSVC7.1 RTM build main_review_01. All2028 target bytes match with relocation operands masked; all498 instruction boundaries and final ret0xc agree. Each40 operands independently resolves to its exact original binding, or exact literal data. Entire reached graph contains only the root, with zero helper debt/EH metadata. All30 named emitted sections inventoried;29 unselected functions have no progress credit.

Fresh behavior runs:4608 scenarios on VC7.1 /Od /W3 /GR- without EH, and64-bitGCC C++03, both print exactly009fa780_TEST PASS. Main independently compared the complete target and the two production state-helper branches verbatim to the fixture. Original PDB identity and donor argument/body roles agree. Both PDB base chains and field records independently checked; actual-header VC71 assertions pass5sizes11fields2basechains. Scoped capture is the original empty class, with no false inheritance.

Three owned header additions reviewed against baseline31; only manifest-owned C2DBoxI,CTVertexRHWColTex1,CTVertexRHWColSpecTex1 merged. Stale CRSM old/new files in row history are not selected. Existing68 old/default profile checks passed. Merged32 regression passes all72 targets and every emitted named C++ code section+relocation inventory; custom EH audits also pass. No commit/push.

Receipts: review/main_009fa780; history/009fa780/main_review_01; runtime/behavior_tests/009fa780/main_independent; review/MERGED_AUDIT_32.json.
