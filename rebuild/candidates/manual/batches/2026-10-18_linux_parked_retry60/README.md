# Nested scene-map updates

00b948e0 (179 bytes): corrected propagated scene-map owner to CEngineScene using PDB MapList nested-map member and native caller/callee evidence. No scene headers exist. Root independently checked VC6 tree/node layouts, both successor traversals, sentinel termination and sole callee CEngineSceneMap::Update00b947e0.

Verbatim-source fixtures cover empty outer/inner trees, balanced/left/right outer trees and sorted visitation. Root added seven-by-seven balanced nested trees (49 visits) to exercise descending the left spine of right subtrees and repeated ancestor ascent in both loops. Target source unchanged, one total source grade. Oracle ends at ret; no padding included.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
