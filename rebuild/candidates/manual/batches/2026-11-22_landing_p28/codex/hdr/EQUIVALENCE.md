# Combined-header equivalence

The supplied `tools/lane_merge/equiv_check.py` compared **3,629 landed translation units and 3,132 associated test sources** against the original package includes and the combined pass 28 headers. The checker reported **0 changed or errored preprocessing entries**. No available row triggered a VC7.1 regression regrade. The checker ran in exact static-assert name mode.

The combined private snapshot includes the 25 changes under `main/include` and 85 staged patch pairs across 76 header paths. It includes the final ScrollBar patch, the opt-in CWorldMap::IsPosPassable(const C3DVector&) const declaration for retail 0x004FCC40, and the opt-in CTCSpecialAbilities::LockAbilitiyMutex definition for the Divine Wrath constructor. The ScrollBar patch's SHA-256 is `21f1587b7fcecd05d020494b91d45214919194eda30f4d028899d743159b384f`. The combined input digest is `cd9640ec9d46c9ba5ee9fae1afe9053ebae20b5c7dea761b4f172438c83014aa`.

| Coverage | Count |
|---|---:|
| Entries in the package's landed source index | 57,125 |
| Obtainable landed source bodies | 52,302 |
| Current package neighbour copies | 200 |
| Index filename found in another available snapshot | 44,317 |
| Older source revisions available by address | 7,785 |
| Indexed rows without an obtainable source body | 4,823 |
| Conservatively affected headers after include closure | 1,449 |
| Checked source bodies from package neighbours | 89 |
| Checked source bodies with the index filename | 2,946 |
| Checked historical address revisions | 594 |

The candidate selection follows literal include paths through both header trees and source-to-source includes. It also selects files with macro includes. The checker ran on the selected rows with their available tests, preserving relative paths for the repository fixtures. Sources came from the package, workspace snapshots and available delivery archives. The checker uses its own stubs for missing toolchain includes.

The complete current landed source tree is missing from this workspace. Historical address revisions extend coverage for earlier source versions. Current-source equivalence for those revisions requires the current files. The 4,823 indexed rows without a local source body still need include-reachability classification on the merge host.

The first run reported 0x0093AB70 because the fallback directory was a symlink to the original headers. GCC then recorded the dead GFDebugPDynamicCast assertion's file path under different directory spellings. The final run uses an empty fallback directory that preserves source-to-source include resolution and leaves header resolution to the two compared trees.

All 273 moved assertion occurrences in the root changes retain their original typedef number and condition. The complete merge preserves all 795 moved occurrences, including duplicate conditions in separate opt-in branches. There were 0 missing or renumbered original assertions.
