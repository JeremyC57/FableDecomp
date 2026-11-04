# Landing 2026-11-04d: chunk 741 (CPSCEmitterGeneric::ReadBinary, GFInterpolateArray<C3DVector>)

## Result
- **Landed 2 rows** (`logs/land.log`). Before landing, both were graded read-only on the landing tree (HEAD + the merged
  `stlport/fable_containers.h`) with no overlay (`logs/new2.log`): RELOCATION_MATCH + PASS.

  | Address | Name | Grade |
  |---|---|---|
  | 0x00B149D0 | `NParticleEngine::CPSCEmitterGeneric::ReadBinary` (slot 15) | RELOCATION_MATCH |
  | 0x00B15BD0 | `NParticleEngine::GFInterpolateArray<C3DVector>` (free fastcall template; catalog said `InterpolateAnimationSequence`) | RELOCATION_MATCH, `retail_asm` via the shared GFFloatToLongNear |

  - No row source or test contains `__asm`, `naked`, `_emit` or `volatile`.
  - 0x00B15BD0's symbol `??$GFInterpolateArray@VC3DVector@@@NParticleEngine@@YI?AV?$CArray@VC3DVector@@@@ABV1@0MABVC3DVector@@@Z` is in
    both PDB name sets.
- **Not landed** (staged in `pending/2026-11-04_linux_hand741/differ/`):
  - 0x00B13530 CPSCEmitterGeneric::DatabaseSerialise, DIFFER(5268v5268), 16 masked lines:
    - two bool-default stores come one Transfer earlier in retail;
    - the operands of the NonUniformScalingY OR are swapped.
  - 0x00B10B10 GenerateParticle, DIFFER(4380v4276): retail decodes its bit-fields with the unsigned fix-up (`test ; jge ; fadd 2^32`),
    and ebx and the frame differ.
  - 0x00B0DC20 CPSCUpdateNormal::Update (retried from chunk 739), DIFFER(5601v5602):
    - one 12-byte temporary is missing (frame 0x2D0 against 0x2DC);
    - the x87 order of the cross products differs.

    The draft decodes the whole body.

## Headers
- **Merged:** `stlport/fable_containers.h`, a new FABLE_RETAIL_STLP_VECTOR_C branch. It holds STLport 4.6 `_vector.c`'s
  `operator=` body plus `capacity` / `_M_allocate_and_copy`, which ReadBinary needs.
  - With the body visible, VC7.1 still calls 0x0075B8D0 out of line, but it keeps the caller's local start pointer in a register
    across the call, as retail does.
  - Unchanged in the repo since the overlay base b31fa166c, and add-only. hdrcheck2 prints `default=same` and every other macro
    `same` (`logs/hdrcheck2_pre_merge.txt`).
  - No landed source names the macro.
- **Not taken:** `engine/C3DVector.h` FABLE_RETAIL_C3DVECTOR_EMITTER (GetRandomPointWithinUnitSphere, Clear). Only the parked
  GenerateParticle uses it, so it is kept in the staged `hdr/`.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04d.tsv`, 17 labels. GetParticleEmitterDatabase 0x00AEAAE0 is left out: it has one
  source only. `apply_labels.py` made 7 catalog renames and 9 manifest renames.
- **`reloc_bindings.tsv`:** +3 rows from 0x00B149D0, each with the PDB-proven spelling:
  - 0x007C2AB0 `vector<C3DVector>(unsigned)`;
  - 0x0075B8D0 `vector<C3DVector>::operator=`;
  - 0x0075FA30 `CCountedPointer<CCardinalSpline>::Reset`.

  0x007C2AB0 and 0x0075B8D0 already carried wrong spellings (one and five binders), now queued PRIORITY (`land0411d`) for a respell
  wave. `bindings_multi_check.py` prints 0.
- **Queued** (`land0411d`):
  - PRIORITY: 0x007C2AB0, 0x0075B8D0, 0x0075FA30, 0x0075F830 DeleteData and 0x00AEAB10 GetEnvironmentInteraction;
  - NOTE: 0x00AEAAE0 (one source), and 0x00B15BD0 (done by this landing).
- `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,560 of 49,564 (93.939%); 5,763,120 function bytes (56.347%); 54,112 landed rows (195 `retail_asm`).
