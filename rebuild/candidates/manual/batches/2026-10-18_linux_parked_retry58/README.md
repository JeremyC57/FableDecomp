# Render-layer state restoration

00b32230 (207 bytes): actual renderer, render-state and lighting-manager headers with typed native state-capture/NormalizeNormals views. Root confirmed PDB NormalizeNormals offset0x230 and native shader flags0x390/0x391 (donor0x3a0/0x3a1), all six helper identities and manager globals. Restores saved layer, tracked render state, lights, world transform and shader flags in native order.

Verbatim-source tests cover unchanged/new/already-bookmarked state, zero active mask, dirty-list insertion, all16 shader-flag combinations and callback replacement of the global shader manager. Host -O2 -fno-strict-aliasing models the VC native-layout views; canonical VC7.1 behavior test also required. Seven total source/flag grades. Oracle excludes one proven trailing int3 byte.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
