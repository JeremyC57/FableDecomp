# Probe history

| Source | Outcome |
|---|---|
| Incoming October draft | No exact Leaf line. Prior notes recorded 1,841 bytes and a scratch-register difference. The draft used numbered globals and raw offsets. Independent inspection found a color-write input that contradicted retail. |
| Shared state setters | Replaced all 18 raw-offset operations with calls on the original CRenderManager reference, in native order. Used original D3D table indices, stage numbers and the four-channel color mask. VC7.1 matched 464 instructions and 1,842 bytes after applying all 14 checked relocations. |
| Combined headers | Same result after merging other accepted header changes. The test passed under VC7.1 /Od /W3 /GR- and 64-bit GNU C++03. |

The selected result is RELOCATION_MATCH (1842 object bytes, 1842 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
