# Retry chunk 535 (agent535, 2026-10-28): 0 of 10 parked rows matched; all re-parked with new evidence

- **CPersistContext::TransferVectorOfSubComponents<T> (9 rows, 254v255 / 244v245):** the RTM regrade keeps the one
  residue in the inline CDataInputStream::Read of the load count: ours `add eax,4 ; xor edx,edx` (high word in edx),
  retail `lea edx,[eax+4] ; xor eax,eax`. A scan of the ~800 inline-Read sites in retail finds no `add eax / xor edx`
  form at all. In our build the edx choice is caused by the save arm's `cdq ; idiv` (size() over a non-power-of-two
  element): removing the division, either loop or the write, or compiling /O2, gives retail's form. ~150 compile-only
  variants (Read / IsValidPosition spellings, loop / switch forms, every /O /G /Z flag, the QFE-4035 compiler) do not.
  Recorded as a lane gap; the one untested route is an LTCG (/GL + /LTCG) build of the def TU.
- **0x00B2F760 CEnginePrimitiveOcclusionManager ctor (101v95):** the parked source no longer compiled (C2512) and was
  repaired; retail reloads only X of the `C3DVector(1,0,0)` temporary and forwards zero Y/Z through edx; ~130 forms fold
  the temporary away or give the same 101 bytes.
- Names re-proven for all nine element types; 0x00B59710 is `??0CEngineComponent@@QAE@XZ` (vtable 0x012A2544).
  NAME_FIXES.tsv (16 rows, deferred): 0x004F9CFC `??1CCardPositionsDef@@UAE@XZ` (landed caller binds QAE), six shared
  callees with 3-27 invented spellings each.
