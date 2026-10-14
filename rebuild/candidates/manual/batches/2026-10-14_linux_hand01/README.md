# Batch `2026-10-14_linux_hand01` — hand-authored C3DAnimFileXSequenceChunk .. C3DMesh2 functions: 40 bodies (37 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the second chunk of
class-labelled open functions of 17–96 bytes: animation sequence/transition data constructors and
destructors, `C3DAnimationManager` pools, `C3DGroup2`, mesh handle getters, counted-pointer
releases (`CIVCountedPointer` through a local copy of the object pointer, `CCountedPointer` through
the member), event-base destructors, a `CCharString`-by-value getter and an identity-matrix
constructor. Three TUs carry `#pragma optimize("s", on)` in-source.

Module labels in the chunk were mostly propagated garbage; rows were re-attributed from the
stored vtables and the PDB layouts (retail `CArray` is 12 bytes where Ego_r has 16, so several
members sit 8 bytes below the PDB offsets -- each source documents this). Three bodies differ only
in instruction scheduling, and one row (`0x00A3BE30`) was over-captured by the family scan: its
64-byte span holds the 20-byte `Validate` plus an unlisted 32-byte function at 0x00A3BE50 (the
agent's oracle carries the trimmed row; the second function is a follow-up).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
