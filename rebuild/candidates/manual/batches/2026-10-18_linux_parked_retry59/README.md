# Shader-resource shutdown

00b3b0c0 (189 bytes): actual CRenderManagerCore header, documented native resource-manager/bank views where donor strides differ materially. Root independently verified bank stride0x128, resource stride8,27 entries, bases0x960/0x29c0, shared core device/texture offsets, COM stdcall slots65/100/104 and bank Close slot5 via RTTI. A natural inline unbinding helper reproduces native register lifetimes.

Verbatim-source fixtures check stream/index/texture order, null texture slots, live texture-count extension, replaced device reload, populated first/last banks, resource-before-bank closure and callback population of a later resource. Native layout assertions and preserved pointer/reference fields are checked. Ten total grades; exact variant9 retained after an already-launched nonmatching variant10. Oracle excludes three proven trailing int3 bytes.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
