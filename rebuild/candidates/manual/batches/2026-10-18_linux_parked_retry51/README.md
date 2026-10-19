# Occlusion-group render queue

00bacc50 (184 bytes): actual internal-primitive/group/manager/count-pointee headers with narrow native method views. Root audited current occlusion layer in shared data, first-render guards, native32-byte query info, counted ownership transfer, frame increment,24-byte bounding-box subprimitive and sorted renderer queue. Virtual Delete slot1 and genuine bounding-box destructor/vtable checked; no extra reference increment is introduced.

Exact-source fixtures cover both layers, every guard, allocation failures, callback-injected existing references1/3, release/delete order, frame increment before queueing, ignored arguments and null subprimitive. Host uses -fcheck-new for native nullable allocation semantics. Four total grades; only8 proven trailing int3 bytes removed.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
