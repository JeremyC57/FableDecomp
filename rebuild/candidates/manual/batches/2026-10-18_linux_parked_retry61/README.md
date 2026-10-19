# Occlusion-group cleanup and visibility checks

00b92450 (172 bytes): shared primitive/pointee/occlusion manager headers and documented native handle/virtual interfaces. Root audited last-reference notification followed by pointer reload/release, layer mask removal, slot27 occlusion lookup and Reset00b2fa40. Actual-source fixtures cover both layers, ownership counts and callback replacement, skipped group primitives and missing mask/info. Six total grades including shared-header/name and host-layout review corrections.

00b936e0 (181 bytes): shared manager/group/primitive headers, native group fields, captured successor before unlink and captured manager through the occluded-frame query. Root checked all helper identities, inclusive unsigned delay comparison, force/flag paths, group cleanup after unlink and wrapping occlusion counter. Fixtures cover empty/nonempty lists, all guards, unlink mutation, threshold equality and callback global-manager replacement. Five total grades.

Root caught and corrected host LP64 Type/EnginePrimitiveFlags overlap; host-only native-prefix padding now scales by word size and a nonoverlap assertion protects the fixture. Both verbatim-source host tests use -O2 -fno-strict-aliasing for flattened virtual/layout views. Canonical native headers and target bytes unchanged by this host correction.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
