# EH-funclet path includes repointed after parent renames (2026-11-02)

Landed EH unwind funclet rows (0x0121xxxx) have no code of their own: each source `#include`s its parent's source by path
(`../src/compiled/<xx>/<yy>/<file>.cpp`, resolved through rebuild/include) and is graded as a slice of the parent's funclet block. Naming waves
renamed parent files without updating these includes, so 145 landed funclet sources (80 parents) pointed at files that no longer exist and could
not compile (a VC7.1 compile error under Wine spins instead of failing). Found by a whole-tree scan for unresolved path includes after naming wave
32 broke 10 more (fixed in that landing). Each old file name ends in the parent's address; the coordinator mapped it to that address's current
Source in build_candidates.ps1 (`REPOINTED.tsv`), changed only the include line, and regraded all 145 with `lane_tools/recheck.py` (funclet offsets
from the sources): 145/145 RELOCATION_MATCH + PASS (`recheck_grades.txt`). A whole-tree scan now finds 0 unresolved path includes.
