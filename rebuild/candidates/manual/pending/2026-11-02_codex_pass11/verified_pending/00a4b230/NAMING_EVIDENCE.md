# Identity and extent

Exact target: `?GetSquaredDistanceBetweenSphereTriangle@NBBBMaths@@YIMABVC3DVector@@M000@Z`.

1. Ego_r nameset 00b9f590 names this fastcall function returning float, with a const C3DVector reference, float radius, and three more const C3DVector references. It immediately follows CASuspendableProcess::TerminateProcess (00b9f550) and CheckTermination (00b9f580); retail has those independently landed functions at 00a4b200/00a4b220 immediately before this target. FableWin independently records the exact symbol at0323eef0.
2. Native caller00a17d40..00a17d63 pushes two vector addresses and the radius, puts sphere position in ecx and triangle origin in edx, calls00a4b230, and compares its float return with radius squared. The function itself ignores the radius argument, computes origin minus position, and evaluates the seven regions of the point-to-triangle squared-distance algorithm. Every exit pops12 stack bytes, as this signature requires.
3. C3DVector class key V and fields X/Y/Z+0/+4/+8 come from raw PDB TPI0x273e and the corresponding native loads. Shared engine/C3DVector.h is the real complete 12-byte class; the function does not depend on aggregate embedding alignment.

The span00a4b230..00a4b662 is one complete function ending in ret0xc, followed by alignment padding. No direct callees or named globals occur. Floating constants are compiler literals, not globals. Existing reloc_bindings.tsv has no binding for this target or this exact name (checked before authorship). Local algorithm variable spellings are reconstruction choices; no claim of recovered debug-local names.
