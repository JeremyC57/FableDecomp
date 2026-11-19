# Landing hand3c b4 (2026-11-19): TransformPointThroughMirroredFrame (0x00C1C330)

vland --land, strict headers: RELOCATION_MATCH + PASS, no warnings; g++ test PASS. No header changes (shared engine/C3DVector.h with the existing
FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23). Name descriptive (no PDB / FableWin copy of this audio-wrapper TU).

What closed it: VC7.1 reassociates a one-expression sum to z-first whatever the written order; a stepwise accumulation (`float rx = R1.x*y;
rx += R2.x*z; ...`) keeps retail's y-first order. The last store-schedule residue (y copy after the z fstp) closed by merging the last two `+=`
into `r = r + C*v + T`, which removes the one stack-copy pseudo-op c2's scheduler trace (sched_tools t20 / sim) named. The TU also carries the
landed sibling static 0x00C1C3F0 for its private register convention, spelled on the shared C3DVector.

Queued (`NAME_FIXES.tsv`): respell the catalog / file slug AIMatrixPointHelper_00c1c330; re-land 0x00C1C3F0 on the shared C3DVector header.
