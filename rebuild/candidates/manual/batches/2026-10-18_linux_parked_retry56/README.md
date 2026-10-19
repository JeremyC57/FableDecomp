# Append a default engine handle

005ed940 (181 bytes): corrected propagated AddPrimitive label to handle-vector AppendDefault. Reuses independently reviewed native8-byte handle/base lifecycle. Root checks placement-copy branch, vector end advance, overflow004413c0 and last-reference callback followed by pointee reload. Implicit handle copy invokes counted-pointer copy ownership.

Verbatim-source fixtures check spare/full capacity, default/nil state, shared/last references, notification clearing/replacing the pointee and an additional nonempty handle copy. One total source grade.11 proven trailing int3 bytes excluded.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
