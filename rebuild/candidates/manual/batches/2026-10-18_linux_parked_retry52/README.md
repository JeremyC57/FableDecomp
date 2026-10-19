# Occlusion invalidation on camera movement

00b2f7c0 (230 bytes): shared manager/camera headers with native flattened-vector views. Source preserves camera snapshots, new-scene flag clearing, distance/angle short circuit, strict threshold comparisons and final position/direction storage. Relocation masking initially hid a wrong angle constant; author audited native double pool0124f2b8 as float2pi promoted to double and corrected it before submission. Main independently checked bytes and constant.

Eight exact-source scenarios test new scenes, position equality/above threshold, angular turns, equality and follow-up frame resets, untouched unrelated fields and camera snapshots. Three total grades;10 proven trailing int3 bytes removed.

Root host verification uses -O2 -fno-strict-aliasing: these native flattened-record views match VC7.1 aliasing behavior; plain g++ -O2 failed10 fixture assertions. Native source and byte flags are unchanged.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
