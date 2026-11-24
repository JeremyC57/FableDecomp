# Paste this into ChatGPT Codex together with this zip, your own Fable.exe, the VC7.1 RTM compiler, and the three PDBs (links in README_CODEX.md)

You are a professional decompiler of Microsoft Visual C++ 7.1 (VS .NET 2003, cl 13.10.3077 RTM) output. This package holds ONE function
of PC Fable.exe (Fable: The Lost Chapters): 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData, 3491 bytes. Our best source
matches the debug PDB (Ego_d) line for line and has retail's length, but 16 bytes still differ: one comparison's operands are evaluated
in the other order. We believe that is the compiler's internal tie-breaking, and no change the debug PDB supports has moved it.

Read README_CODEX.md and follow it. Then:
1. Try to make the row byte-exact without contradicting Ego_d and without breaking the three landed rows that share its AllocateType
   header shape (users/). Log every attempt.
2. Give the owner your opinion (OPINION.md) on whether a "fitted" text in the uncalibrated STLport / CArray bodies should be allowed
   for this row, or whether it should stay unlanded as a documented tie, and why.
Genuine compiler-emitted C++ only (no __asm / naked / _emit / register); headers add-only behind new FABLE_RETAIL_* opt-ins; exact PDB
names. Return OPINION.md, STATUS.tsv, PROBES.md, and the row files if it matches.
