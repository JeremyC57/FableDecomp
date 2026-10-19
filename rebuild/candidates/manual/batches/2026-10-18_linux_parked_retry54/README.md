# Engine-handle vector resize

00443f90 (177 bytes): native8-byte element arithmetic and erase/insert callees correct the propagated AddPrimitive label to handle-vector resize. Real handle/base virtual destructors, PersistentPrimitive Delete and NotifyReleased(const handle&) slots audited. Cleanup reloads the pointee after notification, preserving callback replacement/clear behavior. Shared persistent/count-pointee layouts support native method view.

Exact-source fixtures cover shrink/grow/same/empty, null/shared/last ownership, notification clearing/replacing the pointee, exact decrements/deletion and final base destruction. Two total grades;15 proven trailing int3 bytes excluded. Final row of the resumed five-row Sol retry section: four matches, one capped DIFFER00bcc700.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
