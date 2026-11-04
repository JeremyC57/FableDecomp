# Landing 2026-11-04k: chunk 748 (retry of the 8 parked hand-written SSE rows), 1 landed + two macro-isolation headers

## Result
- **Landed 1 row** (`logs/land.log`); graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  - 0x00A9D580 `CMatrix4x4::AlignedPreScaleTransposed` (the manifest called it CMatrix3x4::PostScale), grade `retail_asm`.
- **How the owner's order played out at the 4 SSE sites** (per-site tables in `pending/2026-11-04_linux_hand748/NOTES.md`):
  1. Plain C++ and 6-9 intrinsic spellings per site, with /arch:SSE and /arch:SSE2: the intrinsics choose their own xmm registers, never
     reload the homed pointer, and never keep a counter in a frame word or push/pop esi (best 197v221 here).
  2. A `__forceinline` helper function: worse than the macro at every site (operands get their own slot and a pointer reload).
  3. **Macro isolation** (owner 2026-11-04): an `__asm` macro names the caller's locals directly, which is what retail's
     "store the pointer, re-read it into edx" schedule is. It matched the SSE block byte-exactly in all 8 rows.
  The row itself has no `__asm`; it expands `GF_SSE_MATRIX_PRESCALE_TRANSPOSED` and carries its own `// Retail-asm:` line.
- **Not landed** (`pending/2026-11-04_linux_hand748/`, each with a `// Grade:` line and a passing test): the other 7. In every one the SSE
  block is exact and the residue is ordinary C++: x87 multiply operand order (VC7.1 follows function-wide value numbering, not spelling),
  slot order of a second inlined copy, a loop pointer offset, and the fast inverse-sqrt tail of 0x00988E80.

## New isolation headers (macro form)
- `engine/CMatrix4x4AlignedSSE.h`: GF_SSE_MATRIX_PRESCALE_TRANSPOSED, GF_SSE_MATRIX_PRESCALE, GF_SSE_MATRIX_MULTIPLY (+ inline
  CMatrix4x4::AlignedSetMultiply). `engine/CParticleSimulationSSE.h`: GF_SSE_SATISFY_DISTANCE_CONSTRAINTS.
- Each: `__asm` by default, a plain C++ expansion under `FABLE_PORTABLE`, `// Retail-asm:` lines with retail addresses.
- Dual-path tests `rebuild/tests/portable/test_CMatrix4x4AlignedSSE.cpp`, `test_CParticleSimulationSSE.cpp`.
  `portable_twins_check.py --no-rows` (`logs/portable_twins.log`): VC7.1 asm / dual / portable PASS, g++ portable PASS, headers OK;
  72 steps, 0 failed.

## Headers changed (behind their retail macros; unchanged in the repo since the overlay base 4e108b875)
`CMatrix4x4.h` (FABLE_RETAIL_CMATRIX4X4), `CParticleProgram.h` (FABLE_RETAIL_CPARTICLEPROGRAM), `CParticleSimulation.h`
(FABLE_RETAIL_CPARTICLESIMULATION); hdrcheck2 `default=same` (`logs/hdrcheck2_pre_merge.txt`).
- **Siblings:** `includers.py` 14 sources/tests; `ppcmp.py` 7 same, 7 different; all 7 rechecked on the merged tree (`logs/recheck.log`):
  RELOCATION_MATCH + PASS.

## Names
- `ghidra_out/labels_landing_2026-11-04k.tsv` (11 labels, FableWin + Ego_r): AlignedPreScale / AlignedPreScaleTransposed (were
  PostScale), CParticleSimulation::CopyFrom (was InitializeFromTemplate), CParticleProgram::ApplyConstraints / GetVectorBuffer /
  ClingToMesh / SetupSimulation, the two CShaderRenderManager rows. `apply_labels.py`: 2 catalog, 3 manifest renames.
- Queued PRIORITY `land0411k2`: the chunk's NAME_FIXES.tsv (binding and landed-caller respellings, const/reference overloads).
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.
